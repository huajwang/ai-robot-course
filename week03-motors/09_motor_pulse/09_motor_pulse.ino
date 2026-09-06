/*
============================================================
Program 09 — First Motion (Safe Motor Pulse)
Course: Build Your First AI Robot
Week 3: Teach Your Robot to Move
============================================================

PURPOSE
  Make the wheels move — safely.
  A SHORT timed pulse forward, then an automatic STOP.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - a motor driver needs direction pins + PWM speed pins
  - timed moves are safer than "drive forever" loops
  - STOP is a first-class action

KEY CONCEPT
  Brief motor command → then stop

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Week 2 only sensed.
  Program 09 introduces ACTUATORS (motors).

WHAT THE ROBOT DOES NOW
  Beeps ready, drives forward briefly, stops, waits, repeats.

HARDWARE
  - Arduino Uno
  - TB6612 motor driver (or L298N with pin edits)
  - 2× DC motors + wheels
  - Motor battery + shared GND
  - Optional buzzer on pin 12 (pin 8 may be used by some drivers)

SAFETY
  1. First tests: WHEELS UP (robot on a stand / held carefully)
  2. Clear the table edge
  3. Be ready to unplug battery/USB
  4. Do not run motors from USB 5V

HOW TO RUN
  1. Wire driver using hardware/wiring/motors_tb6612.md
  2. Set pin numbers if your kit differs
  3. Upload with wheels lifted
  4. Power motor battery
  5. Confirm short forward spin, then stop
  6. Only then place robot on the floor in a clear arena

CONTROLS
  None — timed behavior. Unplug power to halt immediately.
============================================================
*/

// --- Pin map (avoids Week 2 sensor pins 4/5/6/9/10) ---
const int PWMA = 3;    // Left motor PWM
const int AIN1 = 7;    // Left direction
const int AIN2 = 8;

const int PWMB = 11;   // Right motor PWM
const int BIN1 = A0;   // Right direction
const int BIN2 = A1;

const int STBY = A2;   // TB6612 standby (HIGH = enabled)

const int BUZZER_PIN = 12;
const int BAUD_RATE = 9600;

// Classroom speed cap (0–255). Keep low for first tests.
const int SPEED = 140;

// How long to drive before auto-stop
const int PULSE_MS = 800;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(STBY, HIGH);  // enable driver
  stopMotors();

  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 09 — Safe Motor Pulse");
  Serial.println("Wheels up for first test!");
}

void loop() {
  Serial.println("READY beep");
  beep(100);
  delay(300);

  Serial.println("FORWARD pulse");
  forward(SPEED);
  delay(PULSE_MS);

  Serial.println("STOP");
  stopMotors();
  delay(2000);
}

// ------------------------------------------------------------
// Motor helpers
// ------------------------------------------------------------

void forward(int speed) {
  // Left forward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);

  // Right forward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, speed);
}

void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);
}

void beep(int ms) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

/*
============================================================
STUDENT CHALLENGE
 1. Change PULSE_MS to 400 and 1500. Which feels safer on the floor?
 2. Lower SPEED to 100 for a crawl.
 3. (Bonus) If the robot drives backward, swap AIN1/AIN2 (or motor wires).

EXPECTED RESULT
  - Short forward motion, then a clear stop
  - Serial prints READY / FORWARD / STOP
  - Robot does not keep running forever

COMMON ERRORS
  - Motors silent
      → STBY must be HIGH; check VM battery; shared GND.
  - One wheel only
      → Check that side's PWM/dir pins and motor connector.
  - Uploads OK but random resets when motors start
      → Motor power from USB — use a real motor battery.

INSTRUCTOR NOTE
  Do a class "wheels-up" chorus before any floor time.
  Emphasize STOP as a function students will call constantly.
  E-STOP hardware comes immediately in Program 10.
============================================================
*/
