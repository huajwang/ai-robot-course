/*
============================================================
Program 14 — Obstacle Avoid (Turn Away)
Course: Build Your First AI Robot
Week 4: Teach Your Robot to React
============================================================

PURPOSE
  Stopping is safe. Avoiding is smarter.
  When blocked, the robot turns, then tries to continue.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - autonomous behavior can be a simple strategy (not AI yet)
  - stop → turn → go is a classic robot pattern
  - E-STOP still overrides the whole strategy

KEY CONCEPT
  If blocked → turn away → resume forward

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 13 only stopped when close.
  Program 14 turns, then continues exploring.

WHAT THE ROBOT DOES NOW
  Creeps forward while clear; if too close, stops, turns, resumes.

HARDWARE
  Same as Program 13 (motors, E-STOP, ultrasonic, LED/buzzer)

SAFETY
  Low speed. Soft arena. E-STOP ready. Watch for table edges.

HOW TO RUN
  1. Upload
  2. Release E-STOP
  3. Place a soft obstacle in the path
  4. Robot should turn and try another direction

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
const int TURN_SPEED = 130;
const float STOP_CM = 25.0;
const int TURN_MS = 450;       // how long to turn when blocked
const float CM_PER_US = 0.0343;

// true = prefer turning left when blocked; false = prefer right
const bool PREFER_LEFT_TURN = true;

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
  Serial.println("Program 14 — Obstacle Avoid");
}

void loop() {
  if (isEStopPressed()) {
    stopMotors();
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("E-STOP ACTIVE");
    delay(100);
    return;
  }

  float distanceCm = readDistanceCm();
  Serial.print("dist=");
  Serial.print(distanceCm);
  Serial.print("  ");

  if (distanceCm > 0 && distanceCm < STOP_CM) {
    Serial.println("BLOCKED → avoid");
    avoidObstacle();
  } else {
    Serial.println("CLEAR → forward");
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    forward(SPEED);
    delay(80);
  }
}

void avoidObstacle() {
  stopMotors();
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);

  // Backup briefly so the turn has space
  Serial.println("  backup");
  driveFor(250, false, false);

  if (isEStopPressed()) {
    stopMotors();
    return;
  }

  if (PREFER_LEFT_TURN) {
    Serial.println("  turn LEFT");
    driveFor(TURN_MS, false, true);   // spin left
  } else {
    Serial.println("  turn RIGHT");
    driveFor(TURN_MS, true, false);   // spin right
  }

  stopMotors();
  digitalWrite(LED_PIN, LOW);
  delay(100);
}

void driveFor(int moveMs, bool leftForward, bool rightForward) {
  unsigned long startMs = millis();
  while (millis() - startMs < (unsigned long)moveMs) {
    if (isEStopPressed()) {
      stopMotors();
      digitalWrite(LED_PIN, HIGH);
      Serial.println("E-STOP during avoid!");
      return;
    }
    setLeft(TURN_SPEED, leftForward);
    setRight(TURN_SPEED, rightForward);
    delay(20);
  }
  stopMotors();
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

void forward(int speed) {
  setLeft(speed, true);
  setRight(speed, true);
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
 1. Set PREFER_LEFT_TURN = false and compare behavior.
 2. Change TURN_MS to make bigger or smaller escapes.
 3. (Bonus) Alternate left/right on each avoidance (use a bool flip).

EXPECTED RESULT
  - Open space → forward
  - Obstacle → stop, short backup, turn, then try forward again
  - E-STOP still wins

COMMON ERRORS
  - Gets stuck turning into the same wall
      → Increase TURN_MS; flip prefer side; add more backup time.
  - Corner traps
      → Normal for simple logic — discuss better strategies later.
  - Ignores obstacle
      → Retune STOP_CM; verify ultrasonic aim is forward.

INSTRUCTOR NOTE
  Celebrate "strategy" without calling it AI yet.
  Corner cases motivate Week 5 vision / smarter modes.
  Line follow is next — different sense, same Sense→Think→Act idea.
============================================================
*/
