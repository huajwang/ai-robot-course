# Serial command protocol (Course 2)

Python (or Serial Monitor) sends **one command per line**.  
Arduino replies with short status lines.

## Drive commands

| Command | Meaning |
|---------|---------|
| `FWD` | Drive forward |
| `BACK` | Drive backward |
| `LEFT` | Spin left |
| `RIGHT` | Spin right |
| `STOP` | Stop motors |
| `SPEED:120` | Set speed 0–180 (classroom capped) |

## Extra

| Command | Meaning |
|---------|---------|
| `BEEP` | Short buzzer beep |
| `LED_ON` / `LED_OFF` | Built-in LED |

## Status lines (Arduino → laptop)

Examples:

```text
OK FWD
OK STOP
DIST:24.50
ESTOP:1
ERR UNKNOWN SPIN
```

## Rules

1. Baud rate **9600**
2. Close Arduino Serial Monitor before running Python
3. E-STOP always wins — even if Python sends `FWD`
