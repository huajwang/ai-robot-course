# Build Your First AI Robot

**Teach a robot to see, think, and move.**

This is **Course 2** in the AI/Robotics sequence.

| Course | Title | Pipeline |
|--------|--------|----------|
| 1 | Build Your First AI Computer Vision System | Camera → AI → Decision → Action |
| **2** | **Build Your First AI Robot** | **Camera → AI → Decision → Robot Action** |

## Architecture

- **Robot body:** Arduino Uno (C++) — sensors, motors, E-STOP
- **AI brain:** Student laptop (Python + OpenCV + YOLO from Course 1)
- **Link:** USB serial commands (`FWD`, `STOP`, …)

## Start here

**Week 1 (C++):**

1. Install Arduino IDE (`setup/arduino_ide.md`)
2. Open `week01-cpp-basics/01_blink/01_blink.ino`
3. Upload to Arduino Uno

**Week 5+ (Python):**

```bash
pip install -r requirements.txt
```

## Course map

| Week | Focus | Status |
|------|--------|--------|
| 1 | Meet your robot brain (C++) | Programs 01–04 ready |
| 2 | Sensors | Programs 05–08 ready |
| 3 | Motors + safety | Programs 09–12 ready |
| 4 | Autonomy (Sense → Think → Act) | Programs 13–15 ready |
| 5 | AI vision → robot motion | Programs 16–19 ready |
| 6 | Final AI robot project | Program 20 + Demo Day ready |

## Final project

```bash
# 1) Upload week06-final-project/20_ai_robot_starter/20_robot_brain.ino
# 2) Close Serial Monitor
# 3) Edit SETTINGS, then:
cd week06-final-project/20_ai_robot_starter
python 20_ai_robot_starter.py
```
