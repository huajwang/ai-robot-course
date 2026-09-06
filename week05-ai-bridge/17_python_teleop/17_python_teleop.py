"""
============================================================
Program 17 — Drive from Python (Teleop)
Course: Build Your First AI Robot
Week 5: Teach Your Robot to See (AI Bridge)
============================================================

PURPOSE
  Prove the laptop can drive the robot body.
  Keyboard → Python → Serial command → Arduino motors.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - Python can send the same commands you typed in Serial Monitor
  - closing Serial Monitor is required so Python can open the port
  - teleop is the bridge before YOLO takes the wheel

KEY CONCEPT
  Keyboard → pyserial → FWD/STOP/... → robot motion

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 16 made Arduino listen.
  Program 17 makes Python speak.

WHAT THE ROBOT DOES NOW
  Moves when you hold drive keys; stops when you press space/X.

HARDWARE
  - Program 16 firmware uploaded and running
  - E-STOP wired
  - USB connected; Serial Monitor CLOSED

HOW TO RUN
  1. pip install pyserial opencv-python
  2. Upload 16_command_firmware.ino
  3. Close Serial Monitor
  4. Set serial_port below
  5. python 17_python_teleop.py
  6. Click the window, then use keys

CONTROLS
  W — FWD
  S — BACK
  A — LEFT
  D — RIGHT
  SPACE or X — STOP
  1 / 2 / 3 — SPEED:100 / 130 / 160
  B — BEEP
  Q — quit (sends STOP)
============================================================
"""

import time

import cv2
import numpy as np

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    print("ERROR: pyserial is not installed.")
    print("Fix:  pip install pyserial")
    raise SystemExit(1)

# ------------------------------------------------------------
# SETTINGS
# ------------------------------------------------------------
serial_port = "COM3"  # change to your port
baud_rate = 9600

# ------------------------------------------------------------
# Connect
# ------------------------------------------------------------
def print_available_ports():
    ports = list(list_ports.comports())
    if not ports:
        print("  (No serial ports found.)")
        return
    print("Available serial ports:")
    for port in ports:
        print(f"  - {port.device}: {port.description}")


print(f"Opening {serial_port}...")
try:
    board = serial.Serial(serial_port, baud_rate, timeout=0.1)
except serial.SerialException as error:
    print(f"ERROR: Could not open {serial_port}")
    print(f"Details: {error}")
    print("Is Serial Monitor still open?")
    print_available_ports()
    raise SystemExit(1)

time.sleep(2)  # Arduino USB reset pause
board.write(b"STOP\n")
print("Connected. Click the window, then drive with WASD. Q quits.")

last_command = "STOP"
status_line = "STOP"

# ------------------------------------------------------------
# Teleop loop (OpenCV window for key focus)
# ------------------------------------------------------------
while True:
    canvas = np.zeros((300, 640, 3), dtype=np.uint8)
    cv2.putText(canvas, "PYTHON TELEOP", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1.1, (255, 255, 255), 2)
    cv2.putText(canvas, f"Last command: {status_line}", (20, 110), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 255), 2)
    cv2.putText(canvas, "W/A/S/D drive   SPACE/X stop   1/2/3 speed", (20, 170), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (200, 200, 200), 1)
    cv2.putText(canvas, "B beep   Q quit", (20, 210), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (200, 200, 200), 1)
    cv2.putText(canvas, f"port: {serial_port}   firmware: Program 16", (20, 260), cv2.FONT_HERSHEY_SIMPLEX, 0.55, (140, 140, 140), 1)

    cv2.imshow("Program 17 - Python Teleop", canvas)
    key = cv2.waitKey(30) & 0xFF

    command = None
    if key in (ord("w"), ord("W")):
        command = "FWD"
    elif key in (ord("s"), ord("S")):
        command = "BACK"
    elif key in (ord("a"), ord("A")):
        command = "LEFT"
    elif key in (ord("d"), ord("D")):
        command = "RIGHT"
    elif key in (ord(" "), ord("x"), ord("X")):
        command = "STOP"
    elif key == ord("1"):
        command = "SPEED:100"
    elif key == ord("2"):
        command = "SPEED:130"
    elif key == ord("3"):
        command = "SPEED:160"
    elif key in (ord("b"), ord("B")):
        command = "BEEP"
    elif key in (ord("q"), ord("Q")):
        board.write(b"STOP\n")
        print("Quit.")
        break

    # Send only when the command changes (except BEEP which is an event)
    if command is not None:
        if command == "BEEP" or command != last_command or command.startswith("SPEED:"):
            board.write((command + "\n").encode("ascii"))
            print(f"Sent: {command}")
            status_line = command
            if command != "BEEP":
                last_command = command

board.write(b"STOP\n")
board.close()
cv2.destroyAllWindows()
print("Serial closed. Done.")
print("Next: Program 18 — YOLO decides when to STOP/FWD.")

# ============================================================
# STUDENT CHALLENGE
# 1. Add key E that sends LED_ON and key R that sends LED_OFF.
# 2. Hold-to-drive idea: on key-up always send STOP
#    (harder with waitKey — discuss with instructor).
# 3. (Bonus) Read and print any Arduino reply lines each loop.
#
# EXPECTED RESULT
#  - WASD moves the robot (E-STOP released, Program 16 loaded)
#  - SPACE/X stops
#  - Q quits cleanly with STOP
#
# COMMON ERRORS
#  - Port busy
#      → Close Serial Monitor / other Python serial apps.
#  - Keys do nothing
#      → Click the OpenCV window first.
#  - Robot ignores FWD
#      → E-STOP pressed, or Program 16 not uploaded.
#
# INSTRUCTOR NOTE
#  Do not add YOLO until teleop works for every station.
#  Same debugging split as Course 1 LED week: link first, AI second.
# ============================================================
