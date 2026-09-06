"""
============================================================
Program 18 — AI Stops the Robot (YOLO Guard)
Course: Build Your First AI Robot
Week 5: Teach Your Robot to See (AI Bridge)
============================================================

PURPOSE
  Connect Course 1 AI vision to Course 2 robot motion:
  Camera → YOLO → Decision → Serial → Motors

LEARNING OBJECTIVE
  After this program, you should understand that:
  - YOLO can replace the teleop keyboard as the decision maker
  - AI is slower than ultrasonic reflexes (keep E-STOP!)
  - person detected → STOP is a classic safety/guard behavior

KEY CONCEPT
  Person detected → send STOP  |  else → send FWD (creep)

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 17 used keyboard commands.
  Program 18 uses YOLO person detection to choose FWD/STOP.

WHAT THE ROBOT DOES NOW
  Creeps forward while no person is confirmed; stops when a person is seen.

HARDWARE
  - Program 16 firmware on Arduino
  - Webcam (Course 1)
  - Robot + E-STOP
  - Serial Monitor CLOSED

HOW TO RUN
  1. Upload Program 16 firmware; close Serial Monitor
  2. pip install -r requirements.txt  (opencv, ultralytics, pyserial)
  3. Set serial_port below
  4. python 18_yolo_robot_guard.py
  5. Arm with A; disarm with D; quit with Q

CONTROLS
  A — arm AI driver (allows FWD/STOP commands)
  D — disarm (forces STOP)
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
frames_required = 4          # reduce flicker before STOP
crawl_speed_command = "SPEED:110"

# ------------------------------------------------------------
# Serial helpers
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
    """Send only when command changes (reduces serial spam)."""
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
send_command(board, crawl_speed_command, "")
print("Arduino connected.")

# ------------------------------------------------------------
# Camera + YOLO
# ------------------------------------------------------------
camera = cv2.VideoCapture(camera_index)
if not camera.isOpened():
    print("ERROR: Could not open the camera.")
    board.close()
    raise SystemExit(1)

print("Loading YOLO (yolov8n)...")
model = YOLO("yolov8n.pt")
print("AI Guard ready. A=arm  D=disarm  Q=quit")

armed = False
person_streak = 0

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
            if label not in allowed_labels:
                continue
            if confidence < confidence_threshold:
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

    # --- AI decision → robot command ---
    if not armed:
        mode_text = "DISARMED"
        mode_color = (160, 160, 160)
        last_command = send_command(board, "STOP", last_command)
    elif person_confirmed:
        mode_text = "GUARD STOP (person)"
        mode_color = (0, 0, 255)
        last_command = send_command(board, "STOP", last_command)
    else:
        mode_text = "GUARD CRAWL (clear)"
        mode_color = (0, 200, 0)
        last_command = send_command(board, "FWD", last_command)

    cv2.rectangle(display, (0, 0), (display.shape[1], 100), (0, 0, 0), -1)
    cv2.putText(display, mode_text, (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1.0, mode_color, 2)
    cv2.putText(
        display,
        f"people={person_count} streak={person_streak}/{frames_required} cmd={last_command}",
        (20, 80),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.6,
        (255, 255, 255),
        2,
    )

    cv2.imshow("Program 18 - YOLO Robot Guard", display)
    key = cv2.waitKey(1) & 0xFF

    if key in (ord("a"), ord("A")):
        armed = True
        print("AI driver ARMED")
    if key in (ord("d"), ord("D")):
        armed = False
        last_command = send_command(board, "STOP", last_command)
        print("AI driver DISARMED")
    if key in (ord("q"), ord("Q")):
        print("Quit.")
        break

board.write(b"STOP\n")
board.close()
camera.release()
cv2.destroyAllWindows()
print("Done. Next: Program 19 adds behavior modes (guard/follow/patrol).")

# ============================================================
# STUDENT CHALLENGE
# 1. Invert the policy: person → FWD (follow-ish) and no person → STOP.
# 2. Raise confidence_threshold to 0.65 for fewer false stops.
# 3. (Bonus) If person_count >= 2, send BEEP once.
#
# EXPECTED RESULT
#  - Disarmed: robot stays stopped
#  - Armed + no person: crawls (FWD)
#  - Armed + person confirmed: STOP
#  - On-screen boxes from YOLO (Course 1 skill)
#
# COMMON ERRORS
#  - Robot crawls into people / latency
#      → Keep E-STOP; raise frames_required; aim camera carefully.
#  - YOLO works on screen but robot silent
#      → Recheck Program 16/17 first.
#  - Slow loop
#      → Normal on some laptops; crawl speed already low.
#
# INSTRUCTOR NOTE
#  Say: "Ultrasonic is the reflex; YOLO is the strategy."
#  Ethics: no chasing classmates; demo with consent in the arena.
# ============================================================
