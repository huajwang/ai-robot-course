/*
============================================================
Program 15 — Follow the Path (Line Follow)
Course: Build Your First AI Robot
Week 4: Teach Your Robot to React
============================================================

PURPOSE
  Use floor sensors to stay on a black line —
  a classic robot behavior for delivery / track missions.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - L/C/R readings can steer left and right motors differently
  - "lost line" needs a recovery idea
  - Sense → Think → Act works with line sensors too

KEY CONCEPT
  Line position → steering decision → differential drive

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 14 avoided obstacles with ultrasonic.
  Program 15 follows a path with IR line sensors.

WHAT THE ROBOT DOES NOW
  Follows black tape; tries a short search if the line is lost.

HARDWARE
  - Motors + E-STOP (Week 3)
  - Line sensors L/C/R on pins 4/5/6 (Week 2)
  - LED 13

WIRING
  Line: hardware/wiring/line_sensors.md
  Motors/E-STOP: Week 3 docs

SAFETY
  Slow speed. E-STOP ready. Flat taped track in the arena.

HOW TO RUN
  1. Make a simple black-tape path on light floor/paper
  2. Calibrate INVERT_LINE_LOGIC if needed (Program 07)
  3. Upload, release E-STOP, place center sensor on the line
  4. Watch it track; press E-STOP to halt

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
const int LINE_LEFT_PIN = 4;
const int LINE_CENTER_PIN = 5;
const int LINE_RIGHT_PIN = 6;
const int LED_PIN = 13;
const int BAUD_RATE = 9600;

const bool INVERT_LINE_LOGIC = false;

const int SPEED_FORWARD = 110;
const int SPEED_TURN = 120;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(ESTOP_PIN, INPUT_PULLUP);
  pinMode(LINE_LEFT_PIN, INPUT);
  pinMode(LINE_CENTER_PIN, INPUT);
  pinMode(LINE_RIGHT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(STBY, HIGH);
  stopMotors();

  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 15 — Line Follow");
}

void loop() {
  if (isEStopPressed()) {
    stopMotors();
    digitalWrite(LED_PIN, HIGH);
    Serial.println("E-STOP ACTIVE");
    delay(100);
    return;
  }

  int left = readLineSensor(LINE_LEFT_PIN);
  int center = readLineSensor(LINE_CENTER_PIN);
  int right = readLineSensor(LINE_RIGHT_PIN);

  Serial.print("LCR=");
  Serial.print(left);
  Serial.print(center);
  Serial.print(right);
  Serial.print("  ");

  // --- Think: simple steering rules ---
  if (center == 1 && left == 0 && right == 0) {
    Serial.println("on line → forward");
    digitalWrite(LED_PIN, LOW);
    drive(SPEED_FORWARD, SPEED_FORWARD);
  } else if (left == 1 && right == 0) {
    Serial.println("line left → steer left");
    digitalWrite(LED_PIN, LOW);
    drive(0, SPEED_TURN);            // pivot/steer left
  } else if (right == 1 && left == 0) {
    Serial.println("line right → steer right");
    digitalWrite(LED_PIN, LOW);
    drive(SPEED_TURN, 0);            // pivot/steer right
  } else if (left == 1 && center == 1 && right == 0) {
    Serial.println("soft left");
    drive(SPEED_FORWARD / 2, SPEED_FORWARD);
  } else if (right == 1 && center == 1 && left == 0) {
    Serial.println("soft right");
    drive(SPEED_FORWARD, SPEED_FORWARD / 2);
  } else if (left == 0 && center == 0 && right == 0) {
    Serial.println("LOST → search");
    digitalWrite(LED_PIN, HIGH);
    recoverLostLine();
  } else {
    // e.g. all sensors on a wide blob of tape
    Serial.println("wide/other → creep");
    drive(SPEED_FORWARD / 2, SPEED_FORWARD / 2);
  }

  delay(30);
}

void recoverLostLine() {
  // Simple recovery: wiggle search while checking E-STOP
  driveFor(200, false, true);   // spin left a bit
  if (isEStopPressed()) {
    stopMotors();
    return;
  }
  driveFor(400, true, false);   // spin right more
  stopMotors();
}

void driveFor(int moveMs, bool leftForward, bool rightForward) {
  unsigned long startMs = millis();
  while (millis() - startMs < (unsigned long)moveMs) {
    if (isEStopPressed()) {
      stopMotors();
      return;
    }
    setLeft(SPEED_TURN, leftForward);
    setRight(SPEED_TURN, rightForward);
    delay(20);
  }
  stopMotors();
}

bool isEStopPressed() {
  return digitalRead(ESTOP_PIN) == LOW;
}

int readLineSensor(int pin) {
  int raw = digitalRead(pin);
  bool onLine = (raw == LOW);
  if (INVERT_LINE_LOGIC) {
    onLine = !onLine;
  }
  return onLine ? 1 : 0;
}

// Drive with independent left/right speeds (both forward)
void drive(int leftSpeed, int rightSpeed) {
  setLeft(leftSpeed, true);
  setRight(rightSpeed, true);
}

void setLeft(int speed, bool goForward) {
  if (speed < 0) {
    speed = 0;
  }
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
  if (speed < 0) {
    speed = 0;
  }
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

/*
============================================================
STUDENT CHALLENGE
 1. Tune SPEED_FORWARD until following is smooth but controllable.
 2. Improve recoverLostLine() with your own search pattern.
 3. (Bonus) If center sees the line, beep once (pin 12 buzzer).

EXPECTED RESULT
  - Robot stays near the black tape
  - Steers back when it drifts left/right
  - Lost line triggers a short search wiggle
  - E-STOP still works

COMMON ERRORS
  - Immediately leaves the line
      → Invert logic; lower speed; check sensor height.
  - Oscillates hard left/right
      → Soften turns; reduce SPEED_TURN; widen tape.
  - "Works on desk, fails on floor"
      → Lighting/reflectance changed — recalibrate invert/height.

INSTRUCTOR NOTE
  Exit ticket Week 4: "My robot can react with sensors — stop, avoid, follow."
  Preview Week 5: Python/YOLO will send FWD/STOP like a smarter sensor.
  Keep PID out of core path — innovation lab stretch only.
============================================================
*/
