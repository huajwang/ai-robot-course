# Week 3 — Teach Your Robot to Move

**Big idea:** Motors are actuators. With a motor driver and careful safety rules, C++ can make the robot drive.

**Pipeline:**

```text
C++ → motor driver → wheels → motion
```

**Safety first:**

1. Test with **wheels lifted** (or robot on a stand) until the move looks correct  
2. Keep hands clear of wheels  
3. Use a **separate motor battery** when possible (do not run motors from USB alone)  
4. Program 10 adds a hardware E-STOP — treat it as required after that  

## Programs

| # | Folder | What you learn |
|---|--------|----------------|
| 09 | `09_motor_pulse/` | Safe short forward pulse, then stop |
| 10 | `10_estop_and_drive/` | Emergency stop priority |
| 11 | `11_tank_steer/` | Forward / back / turn (tank steer) |
| 12 | `12_pwm_speed/` | Speed control with PWM |

**Week 3 complete** when Program 12 works and students can explain:

> Motors need direction + speed. E-STOP always wins. Timed moves are safer than endless drive.

**Ready now:** Programs 09–12 (all of Week 3).

## Driver note

Default code targets a **TB6612** dual motor driver.  
If your kit uses **L298N**, see `hardware/wiring/motors_tb6612.md` for pin mapping tips and change the pin constants.
