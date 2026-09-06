# Motor driver wiring (TB6612 preferred)

## Power safety

| Power | Connects to |
|-------|-------------|
| Arduino logic | USB (or regulated 5V) |
| Motors | **Separate battery pack** through the driver VM / motor power terminals |
| Common ground | Battery GND ↔ driver GND ↔ Arduino GND (**required**) |

Never power drive motors from the Arduino 5V pin.

## TB6612 pin map (Week 3+ default — avoids sensor pins)

Chosen to leave Week 2 sensors free:

- Line sensors stay on **4, 5, 6**
- Ultrasonic stays on **9 (TRIG), 10 (ECHO)**
- E-STOP button stays on **2**

```text
TB6612        Arduino Uno
------        -----------
PWMA      →   pin 3        (Left PWM)
AIN1      →   pin 7
AIN2      →   pin 8
PWMB      →   pin 11       (Right PWM)
BIN1      →   A0
BIN2      →   A1
STBY      →   A2           (HIGH = enabled)
VCC       →   5V
GND       →   GND
VM        →   motor battery +
GND       →   motor battery − (shared GND)
```

Optional buzzer → pin 12, built-in LED → pin 13.

## L298N kits

Map similarly:

- ENA → PWM left, IN1/IN2 → left direction  
- ENB → PWM right, IN3/IN4 → right direction  

Edit the pin constants at the top of each Week 3 sketch to match your kit’s labels.
