# Week 1 — Meet Your Robot Brain (C++)

**Big idea:** An Arduino is a tiny computer. Your C++ program can turn electrical pins on and off — that is the start of robotics.

**This week’s pipeline:**

```text
C++ code → Upload → Pin ON/OFF → LED / buzzer / button
```

No motors yet. No AI yet. First: prove **you changed the code → something physical changed.**

## Programs

| # | Folder | What you learn |
|---|--------|----------------|
| 01 | `01_blink/` | Upload C++ and blink the LED |
| 02 | `02_button_led/` | Digital input — button controls LED |
| 03 | `03_serial_hello/` | Talk to the computer (Serial) |
| 04 | `04_functions_buzzer/` | Reusable actions with functions |

**Week 1 complete** when Program 04 works and students can explain:

> setup/loop, digital I/O, Serial messages, and functions are enough C++ to start controlling a robot.

**Ready now:** Programs 01–04 (all of Week 1).

## Hardware

| Program | Needs |
|---------|--------|
| 01 | Uno + USB (built-in LED) |
| 02 | + pushbutton (pin 2 → GND), `INPUT_PULLUP` |
| 03 | Uno + USB only |
| 04 | + active buzzer on pin 8 |
