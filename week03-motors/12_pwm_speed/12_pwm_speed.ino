/*
============================================================
Program 12 — Speed Control (PWM)
Course: Build Your First AI Robot
Week 3: Teach Your Robot to Move
============================================================

PURPOSE
  Direction is not enough — robots also need SPEED control.
  PWM (Pulse Width Modulation) lets you choose how hard the
  motors push (0 = stop, 255 = full, we keep a classroom cap).

LEARNING OBJECTIVE
  After this program, you should understand that:
  - analogWrite(pin, value) sets motor effort
  - higher PWM usually means faster wheels
  - capping max speed is a safety feature

KEY CONCEPT
  PWM speed value → how fast the robot crawls or rolls

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 11 used one fixed SPEED.
  Program 12 compares slow vs medium vs faster pulses.

WHAT THE ROBOT DOES NOW
  Drives forward three times: SLOW, MEDIUM, FASTER (still capped),
  with E-STOP respected the whole time.

HARDWARE
  - Same motor + E-STOP wiring as Programs 10–11

SAFETY
  Wheels up first. MAX_SPEED is intentionally limited.
  E-STOP ready before floor tests.

HOW TO RUN
  1. Upload
  2. Release E-STOP
  3. Watch/listen to three different forward speeds
  4. Press E-STOP anytime

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

// Classroom ceiling — do not raise above this in class without instructor OK
const int MAX_SPEED = 180;

const int SPEED_SLOW = 90;
const int SPEED_MEDIUM = 130;
const int SPEED_FASTER = 170;  // still under MAX_SPEED

const int MOVE_MS = 900;
const int PAUSE_MS = 700;

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
  Serial.println("Program 12 — PWM Speed Control");
  Serial.print("Classroom MAX_SPEED = ");
  Serial.println(MAX_SPEED);
}

void loop() {
  if (isEStopPressed()) {
    haltForEStop();
    return;
  }

  Serial.println("SLOW");
  beep(40);
  driveForwardFor(MOVE_MS, SPEED_SLOW);
  pauseBetweenMoves();

  if (isEStopPressed()) {
    haltForEStop();
    return;
  }

  Serial.println("MEDIUM");
  beep(40);
  driveForwardFor(MOVE_MS, SPEED_MEDIUM);
  pauseBetweenMoves();

  if (isEStopPressed()) {
    haltForEStop();
    return;
  }

  Serial.println("FASTER (still capped)");
  beep(40);
  driveForwardFor(MOVE_MS, SPEED_FASTER);
  pauseBetweenMoves();

  Serial.println("Cycle done — pausing");
  stopMotors();
  delay(2000);
}

void driveForwardFor(int moveMs, int requestedSpeed) {
  int speed = requestedSpeed;
  if (speed > MAX_SPEED) {
    speed = MAX_SPEED;
  }
  if (speed < 0) {
    speed = 0;
  }

  unsigned long startMs = millis();
  while (millis() - startMs < (unsigned long)moveMs) {
    if (isEStopPressed()) {
      stopMotors();
      digitalWrite(LED_PIN, HIGH);
      Serial.println("E-STOP during move!");
      return;
    }
    forward(speed);
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

void forward(int speed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);

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
 1. Find the slowest SPEED_SLOW that still moves your robot.
 2. Try requesting 255 and confirm MAX_SPEED caps it.
 3. (Bonus) Make left and right use different speeds to curve gently.

EXPECTED RESULT
  - Three visibly different forward speeds
  - Faster is still controlled (not full-blast)
  - E-STOP still works during each pulse

COMMON ERRORS
  - No difference between speeds
      → Values too close, or battery too low for higher PWM.
  - Robot too fast in class
      → Lower MAX_SPEED and SPEED_FASTER together.
  - Thinking analogWrite means "analog voltage pin mode"
      → On motor PWM pins it means duty-cycle speed control.

INSTRUCTOR NOTE
  Exit ticket Week 3: "I can drive, turn, set speed, and E-STOP."
  Connect: Week 4 will choose WHEN to move using sensors.
  Keep MAX_SPEED as a shared classroom rule.
============================================================
*/
