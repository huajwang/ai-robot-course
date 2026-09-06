# Week 5 — Teach Your Robot to See (AI Bridge)

**Big idea:** Course 1 AI vision becomes the robot’s high-level brain.  
C++ keeps reflexes (motors, E-STOP, optional sensors).

**Pipeline:**

```text
Laptop (Python / YOLO) --serial commands--> Arduino (C++ motion)
```

## Programs

| # | Folder | What you learn |
|---|--------|----------------|
| 16 | `16_command_firmware/` | Arduino obeys `FWD` / `STOP` / … |
| 17 | `17_python_teleop/` | Drive from Python keyboard |
| 18 | `18_yolo_robot_guard/` | YOLO decision moves/stops the robot |
| 19 | `19_ai_behaviors/` | Guard / follow / patrol modes |

**Week 5 nearly complete** — Program 20 wraps this into a remixable final starter.

**Ready now:** Programs 16–19. Program 20 lives in `week06-final-project/`.

See also: `../protocol/serial_commands.md`
