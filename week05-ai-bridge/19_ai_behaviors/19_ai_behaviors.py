"""
============================================================
Program 19 — AI Behavior Modes
Course: Build Your First AI Robot
Week 5: Teach Your Robot to See (AI Bridge)
============================================================

PURPOSE
  One robot, several AI "personalities":
  GUARD, FOLLOW, and PATROL — switched with keys.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - the same YOLO detections can drive different policies
  - modes make a project feel like a product
  - disarm still forces STOP for safety

KEY CONCEPT
  Mode switch → different Decision map → different robot actions

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 18 had one policy (stop for person).
  Program 19 adds follow + simple patrol behaviors.

WHAT THE ROBOT DOES NOW
  GUARD: crawl if clear, STOP if person
  FOLLOW: STOP if clear, creep FWD if person (gentle follow-ish)
  PATROL: alternate FWD and LEFT when clear; STOP if person

HARDWARE
  - Program 16 firmware
  - Webcam + robot + E-STOP
  - Serial Monitor closed

HOW TO RUN
  1. Upload Program 16; close Serial Monitor
  2. Set serial_port
  3. python 19_ai_behaviors.py
  4. A arm, D disarm, 1/2/3 mode, Q quit

CONTROLS
  A — arm
  D — disarm (STOP)
  1 — GUARD mode
  2 — FOLLOW mode
  3 — PATROL mode
  Q — quit
============================================================
"""

import time

import cv2
from ultralytics import YOLO

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    print("ERROR: pyserial is not installed. Fix: pip install pyserial")
    raise SystemExit(1)

# ------------------------------------------------------------
# SETTINGS
# ------------------------------------------------------------
camera_index = 0
serial_port = "COM3"
baud_rate = 9600

allowed_labels = {"person"}
confidence_threshold = 0.50
frames_required = 4

MODE_GUARD = "GUARD"
MODE_FOLLOW = "FOLLOW"
MODE_PATROL = "PATROL"

# ------------------------------------------------------------
# Serial
# ------------------------------------------------------------
def print_available_ports():
    ports = list(list_ports.comports())
    if not ports:
        print("  (No serial ports found.)")
        return
    print("Available serial ports:")
    for port in ports:
        print(f"  - {port.device}: {port.description}")


def send_command(board, command, last_command):
    if command == last_command:
        return last_command
    board.write((command + "\n").encode("ascii"))
    print(f"Sent: {command}")
    return command


print(f"Opening {serial_port}...")
try:
    board = serial.Serial(serial_port, baud_rate, timeout=0.1)
except serial.SerialException as error:
    print(f"ERROR: Could not open {serial_port}")
    print(f"Details: {error}")
    print_available_ports()
    raise SystemExit(1)

time.sleep(2)
last_command = send_command(board, "STOP", "")
last_command = send_command(board, "SPEED:110", last_command)

camera = cv2.VideoCapture(camera_index)
if not camera.isOpened():
    print("ERROR: Could not open the camera.")
    board.close()
    raise SystemExit(1)

print("Loading YOLO (yolov8n)...")
model = YOLO("yolov8n.pt")

armed = False
mode = MODE_GUARD
person_streak = 0
patrol_phase = "FWD"
patrol_phase_started = time.time()
patrol_fwd_seconds = 2.0
patrol_turn_seconds = 0.8

print("Modes: 1=GUARD 2=FOLLOW 3=PATROL | A=arm D=disarm Q=quit")

while True:
    success, frame = camera.read()
    if not success:
        print("ERROR: Could not read a frame.")
        break

    results = model(frame, verbose=False)
    result = results[0]
    names = result.names
    boxes = result.boxes

    display = frame.copy()
    person_count = 0

    if boxes is not None:
        for box in boxes:
            label = names[int(box.cls[0])]
            confidence = float(box.conf[0])
            if label not in allowed_labels or confidence < confidence_threshold:
                continue
            person_count += 1
            x1, y1, x2, y2 = [int(v) for v in box.xyxy[0]]
            cv2.rectangle(display, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(
                display,
                f"{label} {confidence:.2f}",
                (x1, y1 - 10 if y1 > 20 else y1 + 20),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (255, 255, 255),
                2,
            )

    if person_count > 0:
        person_streak += 1
    else:
        person_streak = 0
    person_confirmed = person_streak >= frames_required

    # --- Choose command from mode policy ---
    desired = "STOP"
    if not armed:
        desired = "STOP"
        mode_color = (160, 160, 160)
        headline = f"DISARMED | {mode}"
    else:
        mode_color = (0, 200, 255)
        if mode == MODE_GUARD:
            # Clear → crawl; person → stop
            desired = "STOP" if person_confirmed else "FWD"
            headline = f"GUARD | {'STOP' if person_confirmed else 'CRAWL'}"
            mode_color = (0, 0, 255) if person_confirmed else (0, 200, 0)
        elif mode == MODE_FOLLOW:
            # Person → creep toward; no person → wait
            desired = "FWD" if person_confirmed else "STOP"
            headline = f"FOLLOW | {'TRACK' if person_confirmed else 'WAIT'}"
            mode_color = (0, 200, 0) if person_confirmed else (0, 200, 255)
        else:  # PATROL
            if person_confirmed:
                desired = "STOP"
                headline = "PATROL | PERSON STOP"
                mode_color = (0, 0, 255)
            else:
                now = time.time()
                if patrol_phase == "FWD" and now - patrol_phase_started >= patrol_fwd_seconds:
                    patrol_phase = "LEFT"
                    patrol_phase_started = now
                elif patrol_phase == "LEFT" and now - patrol_phase_started >= patrol_turn_seconds:
                    patrol_phase = "FWD"
                    patrol_phase_started = now
                desired = patrol_phase
                headline = f"PATROL | {patrol_phase}"
                mode_color = (255, 200, 0)

    last_command = send_command(board, desired, last_command)

    cv2.rectangle(display, (0, 0), (display.shape[1], 110), (0, 0, 0), -1)
    cv2.putText(display, headline, (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.95, mode_color, 2)
    cv2.putText(
        display,
        f"people={person_count} streak={person_streak}/{frames_required} cmd={last_command}",
        (20, 75),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.6,
        (255, 255, 255),
        2,
    )
    cv2.putText(
        display,
        "1 GUARD  2 FOLLOW  3 PATROL  |  A arm  D disarm  Q quit",
        (20, 100),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.5,
        (200, 200, 200),
        1,
    )

    cv2.imshow("Program 19 - AI Behaviors", display)
    key = cv2.waitKey(1) & 0xFF

    if key in (ord("a"), ord("A")):
        armed = True
        print("ARMED")
    if key in (ord("d"), ord("D")):
        armed = False
        last_command = send_command(board, "STOP", last_command)
        print("DISARMED")
    if key == ord("1"):
        mode = MODE_GUARD
        print("Mode → GUARD")
    if key == ord("2"):
        mode = MODE_FOLLOW
        print("Mode → FOLLOW")
    if key == ord("3"):
        mode = MODE_PATROL
        patrol_phase = "FWD"
        patrol_phase_started = time.time()
        print("Mode → PATROL")
    if key in (ord("q"), ord("Q")):
        print("Quit.")
        break

board.write(b"STOP\n")
board.close()
camera.release()
cv2.destroyAllWindows()
print("Week 5 behaviors ready. Next: Program 20 final starter kit.")

# ============================================================
# STUDENT CHALLENGE
# 1. Add mode 4 = "SPIN SEARCH": LEFT while no person, STOP when person.
# 2. Change allowed_labels to {"bottle"} for a recycling scout.
# 3. (Bonus) In FOLLOW, send BEEP when person first becomes confirmed.
#
# EXPECTED RESULT
#  - Keys 1/2/3 change on-screen mode and robot policy
#  - Disarm always stops
#  - PATROL cycles crawl/turn when the path looks clear
#
# COMMON ERRORS
#  - FOLLOW seems to chase too aggressively
#      → Lower SPEED on Arduino; raise frames_required; keep E-STOP.
#  - PATROL never turns
#      → Check phase timers; confirm LEFT works in Program 17.
#  - Ethics / safety
#      → Demo FOLLOW only with consent in a taped arena.
#
# INSTRUCTOR NOTE
#  This is the menu students will remix in Program 20 / Demo Day.
#  Emphasize: modes are just if/else policies on the same detections.
# ============================================================
