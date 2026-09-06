/*
============================================================
Program 13 — Obstacle Stop (Sense → Think → Act)
Course: Build Your First AI Robot
Week 4: Teach Your Robot to React
============================================================

PURPOSE
  Combine Week 2 sensing with Week 3 driving.
  If something is too close → STOP. Otherwise → creep forward.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - a control loop repeatedly senses, decides, and acts
  - ultrasonic distance can gate motor motion
  - E-STOP still overrides everything

KEY CONCEPT
  Sense (distance) → Think (threshold) → Act (forward / stop)

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Week 3 moved on timers/sequences.
  Program 13 moves based on the ENVIRONMENT.

WHAT THE ROBOT DOES NOW
  Creeps forward while clear; stops and warns when too close.

HARDWARE
  - Motors + E-STOP (Week 3)
  - HC-SR04 TRIG 9, ECHO 10 (Week 2)
  - LED 13 + buzzer 12

WIRING
  Motors/E-STOP: hardware/wiring/motors_tb6612.md + estop.md
  Ultrasonic: hardware/wiring/ultrasonic.md

SAFETY
  Low speed. E-STOP ready. Soft targets (hand/foam), not fragile objects.

HOW TO RUN
  1. Upload
  2. Release E-STOP
  3. Let it creep; put your hand in front → should STOP
  4. Remove hand → may resume forward

CONTROLS
  E-STOP = immediate halt
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
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int BUZZER_PIN = 12;
const int LED_PIN = 13;
const int BAUD_RATE = 9600;

const int SPEED = 120;
const float STOP_CM = 25.0;
const float CM_PER_US = 0.0343;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(ESTOP_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(STBY, HIGH);
  digitalWrite(TRIG_PIN, LOW);
  stopMotors();

  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 13 — Obstacle Stop");
  Serial.print("Stop if closer than ");
  Serial.print(STOP_CM);
  Serial.println(" cm");
}

void loop() {
  // 1) SAFETY first
  if (isEStopPressed()) {
    stopMotors();
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("E-STOP ACTIVE");
    delay(100);
    return;
  }

  // 2) SENSE
  float distanceCm = readDistanceCm();

  // 3) THINK + 4) ACT
  if (distanceCm > 0 && distanceCm < STOP_CM) {
    stopMotors();
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.print("STOP  dist=");
    Serial.println(distanceCm);
  } else {
    forward(SPEED);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.print("FWD   dist=");
    Serial.println(distanceCm);
  }

  delay(80);
}

bool isEStopPressed() {
  return digitalRead(ESTOP_PIN) == LOW;
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long durationUs = pulseIn(ECHO_PIN, HIGH, 30000);
  if (durationUs == 0) {
    return -1.0;
  }
  return (durationUs * CM_PER_US) / 2.0;
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

/*
============================================================
STUDENT CHALLENGE
 1. Tune STOP_CM for your arena (try 15 and 35).
 2. Pulse the buzzer while stopped instead of solid on.
 3. (Bonus) If distance is -1 (no echo), treat it as STOP for safety.

EXPECTED RESULT
  - Open space → slow forward crawl
  - Hand/wall close → stop + LED/buzzer
  - E-STOP still freezes everything

COMMON ERRORS
  - Never stops
      → Ultrasonic wiring; print distance; lower STOP_CM only after readings look real.
  - Stops forever at -1
      → Sensor timeout; aim sensor; check 5V/GND/TRIG/ECHO.
  - Oscillates at the threshold
      → Add a second "resume" distance later (hysteresis) — Program 14 energy.

INSTRUCTOR NOTE
  Say out loud each loop: Sense → Think → Act.
  This is the reflex layer AI will sit on top of in Week 5.
  Avoiding (turn) is Program 14 — keep this file stop-only.
============================================================
*/
