"""
============================================================
Program 20 — AI Robot Starter Kit
Course: Build Your First AI Robot
Week 6: Build Your AI Robot
============================================================

PURPOSE
  Remixable final project: edit SETTINGS to build YOUR AI robot.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - one pipeline supports many student projects
  - C++ handles the body; Python/YOLO handles vision strategy
  - Demo Day story: Camera → AI → Decision → Robot Action

KEY CONCEPT
  Configurable AI mobile robot

PAIR WITH
  Upload 20_robot_brain.ino first, then run this file.

CONTROLS
  A arm | D disarm | 1 GUARD | 2 FOLLOW | 3 PATROL | Q quit
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

# ============================================================
# SETTINGS — change these for YOUR project
# ============================================================
project_title = "MY AI ROBOT"
camera_index = 0
serial_port = "COM3"
baud_rate = 9600

allowed_labels = {"person"}   # try {"bottle"} or {"dog", "cat"}
confidence_threshold = 0.50
frames_required = 4

# Starting mode: "GUARD", "FOLLOW", or "PATROL"
start_mode = "GUARD"
crawl_speed = "SPEED:110"

use_arduino = True            # False = on-screen decisions only (backup demo)
# ============================================================

MODE_GUARD = "GUARD"
MODE_FOLLOW = "FOLLOW"
MODE_PATROL = "PATROL"


def print_available_ports():
    ports = list(list_ports.comports())
    if not ports:
        print("  (No serial ports found.)")
        return
    print("Available serial ports:")
    for port in ports:
        print(f"  - {port.device}: {port.description}")


def send_command(board, command, last_command):
    if board is None:
        return command
    if command == last_command:
        return last_command
    board.write((command + "\n").encode("ascii"))
    print(f"Sent: {command}")
    return command


board = None
last_command = "STOP"

if use_arduino:
    print(f"Opening {serial_port}...")
    try:
        board = serial.Serial(serial_port, baud_rate, timeout=0.1)
    except serial.SerialException as error:
        print(f"ERROR: Could not open {serial_port}")
        print(f"Details: {error}")
        print_available_ports()
        print("Tip: set use_arduino = False for on-screen-only backup.")
        raise SystemExit(1)
    time.sleep(2)
    last_command = send_command(board, "STOP", "")
    last_command = send_command(board, crawl_speed, last_command)
else:
    print("Arduino disabled — on-screen decisions only.")

camera = cv2.VideoCapture(camera_index)
if not camera.isOpened():
    print("ERROR: Could not open the camera.")
    if board is not None:
        board.close()
    raise SystemExit(1)

print("Loading YOLO (yolov8n)...")
model = YOLO("yolov8n.pt")

armed = False
mode = start_mode if start_mode in {MODE_GUARD, MODE_FOLLOW, MODE_PATROL} else MODE_GUARD
person_streak = 0
patrol_phase = "FWD"
patrol_phase_started = time.time()
patrol_fwd_seconds = 2.0
patrol_turn_seconds = 0.8

print(f"Project: {project_title}")
print(f"Watching for: {sorted(allowed_labels)}")
print("A arm | D disarm | 1/2/3 modes | Q quit")

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
    target_count = 0

    if boxes is not None:
        for box in boxes:
            label = names[int(box.cls[0])]
            confidence = float(box.conf[0])
            if label not in allowed_labels or confidence < confidence_threshold:
                continue
            target_count += 1
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

    if target_count > 0:
        person_streak += 1
    else:
        person_streak = 0
    target_confirmed = person_streak >= frames_required

    desired = "STOP"
    if not armed:
        headline = f"{project_title} | DISARMED | {mode}"
        mode_color = (160, 160, 160)
        desired = "STOP"
    else:
        if mode == MODE_GUARD:
            desired = "STOP" if target_confirmed else "FWD"
            headline = f"{project_title} | GUARD | {'STOP' if target_confirmed else 'CRAWL'}"
            mode_color = (0, 0, 255) if target_confirmed else (0, 200, 0)
        elif mode == MODE_FOLLOW:
            desired = "FWD" if target_confirmed else "STOP"
            headline = f"{project_title} | FOLLOW | {'TRACK' if target_confirmed else 'WAIT'}"
            mode_color = (0, 200, 0) if target_confirmed else (0, 200, 255)
        else:
            if target_confirmed:
                desired = "STOP"
                headline = f"{project_title} | PATROL | TARGET STOP"
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
                headline = f"{project_title} | PATROL | {patrol_phase}"
                mode_color = (255, 200, 0)

    last_command = send_command(board, desired, last_command)

    cv2.rectangle(display, (0, 0), (display.shape[1], 115), (0, 0, 0), -1)
    cv2.putText(display, headline, (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.8, mode_color, 2)
    cv2.putText(
        display,
        f"count={target_count} streak={person_streak}/{frames_required} cmd={last_command}",
        (20, 75),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.6,
        (255, 255, 255),
        2,
    )
    cv2.putText(
        display,
        "1 GUARD  2 FOLLOW  3 PATROL  |  A arm  D disarm  Q quit",
        (20, 105),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.5,
        (200, 200, 200),
        1,
    )

    cv2.imshow("Program 20 - AI Robot Starter", display)
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

if board is not None:
    board.write(b"STOP\n")
    board.close()
camera.release()
cv2.destroyAllWindows()
print("Done. Customize SETTINGS and make it yours for Demo Day!")

# ============================================================
# STUDENT CHALLENGE
# 1. Rename project_title and pick labels from project_ideas.md
# 2. Set start_mode to your favorite behavior
# 3. Write notes.md in student_template/ explaining your robot
#
# EXPECTED RESULT
#  - Full Camera → AI → Decision → Robot Action demo
#  - Easy backup with use_arduino = False
#
# INSTRUCTOR NOTE
#  Grade story + intentional SETTINGS changes + safe E-STOP habit.
# ============================================================
