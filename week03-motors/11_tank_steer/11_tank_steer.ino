/*
============================================================
Program 11 — Tank Steer (Forward, Back, Turn)
Course: Build Your First AI Robot
Week 3: Teach Your Robot to Move
============================================================

PURPOSE
  A 2-wheel robot steers by spinning left and right motors
  differently — like a tank.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - forward = both wheels same direction
  - turn = wheels opposite directions
  - every move still respects E-STOP

KEY CONCEPT
  Differential drive (tank steering)

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 10 only pulsed forward with E-STOP.
  Program 11 adds back, left, and right moves.

WHAT THE ROBOT DOES NOW
  Runs a demo sequence: FWD → BACK → LEFT → RIGHT → pause
  (skips motion while E-STOP is pressed)

HARDWARE
  - Same as Program 10 (motors + E-STOP on pin 2)

SAFETY
  Wheels up first. Clear arena. E-STOP ready.

HOW TO RUN
  1. Upload
  2. Release E-STOP to allow the sequence
  3. Watch Serial labels match the motion
  4. Hit E-STOP anytime to freeze

CONTROLS
  E-STOP press = halt / block
============================================================
*/

const int PWMA = 3;
const int AIN1 = 7;
const int AIN2 = 8;
const int PWMB = 11;
const int BIN1 = A0;
const int BIN2 = A1;
const int STBY = A2;
const int ESTOP_PIN = 2;
const int BUZZER_PIN = 12;
const int LED_PIN = 13;
const int BAUD_RATE = 9600;

const int SPEED = 140;
const int MOVE_MS = 700;
const int PAUSE_MS = 500;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(ESTOP_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(STBY, HIGH);
  stopMotors();

  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 11 — Tank Steer");
}

void loop() {
  if (isEStopPressed()) {
    haltForEStop();
    return;
  }

  Serial.println("FORWARD");
  beep(40);
  driveFor(MOVE_MS, true, true);   // leftFwd, rightFwd
  pauseBetweenMoves();

  if (isEStopPressed()) {
    haltForEStop();
    return;
  }

  Serial.println("BACK");
  beep(40);
  driveFor(MOVE_MS, false, false);
  pauseBetweenMoves();

  if (isEStopPressed()) {
    haltForEStop();
    return;
  }

  Serial.println("LEFT");
  beep(40);
  // Left back + right forward = spin left
  driveFor(MOVE_MS, false, true);
  pauseBetweenMoves();

  if (isEStopPressed()) {
    haltForEStop();
    return;
  }

  Serial.println("RIGHT");
  beep(40);
  driveFor(MOVE_MS, true, false);
  pauseBetweenMoves();

  Serial.println("Sequence done — pausing");
  stopMotors();
  delay(2000);
}

// Drive while checking E-STOP every 20 ms
void driveFor(int moveMs, bool leftForward, bool rightForward) {
  unsigned long startMs = millis();
  while (millis() - startMs < (unsigned long)moveMs) {
    if (isEStopPressed()) {
      stopMotors();
      digitalWrite(LED_PIN, HIGH);
      Serial.println("E-STOP during move!");
      return;
    }
    setLeft(SPEED, leftForward);
    setRight(SPEED, rightForward);
    delay(20);
  }
  stopMotors();
}

void pauseBetweenMoves() {
  stopMotors();
  delay(PAUSE_MS);
}

void haltForEStop() {
  stopMotors();
  digitalWrite(LED_PIN, HIGH);
  Serial.println("E-STOP ACTIVE");
  delay(150);
}

bool isEStopPressed() {
  return digitalRead(ESTOP_PIN) == LOW;
}

void setLeft(int speed, bool goForward) {
  if (goForward) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  analogWrite(PWMA, speed);
}

void setRight(int speed, bool goForward) {
  if (goForward) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
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
 1. If LEFT/RIGHT are swapped on your chassis, exchange those two blocks.
 2. Make turns shorter: use MOVE_MS / 2 for LEFT and RIGHT only.
 3. (Bonus) Add a "wiggle": quick left then quick right after FORWARD.

EXPECTED RESULT
  - Clear sequence: forward, back, left, right with pauses
  - E-STOP still blocks or interrupts motion
  - Serial names match what the robot does

COMMON ERRORS
  - Turns the wrong direction
      → Swap the LEFT/RIGHT blocks, or swap one motor's wires.
  - Curves instead of spinning in place
      → Uneven battery/traction; try a clean floor and equal SPEED.
  - Robot never starts sequence
      → Release E-STOP (pin 2 must read HIGH when released).

INSTRUCTOR NOTE
  Draw tank-steer on the board: same way = straight, opposite = turn.
  Keep E-STOP sacred. PWM speed tuning is Program 12.
============================================================
*/
