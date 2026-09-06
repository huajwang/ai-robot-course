/*
============================================================
Program 16 — Robot Listens (Serial Command Firmware)
Course: Build Your First AI Robot
Week 5: Teach Your Robot to See (AI Bridge)
============================================================

PURPOSE
  Give the robot a simple language.
  Python (next programs) will speak this language to drive the body.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - Serial text commands can control motors
  - the Arduino parses a line and calls motion helpers
  - E-STOP still blocks motion even if a command says FWD

KEY CONCEPT
  Text command line → robot action

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Week 4 decided motion inside Arduino from sensors.
  Program 16 lets an EXTERNAL brain (laptop) request motion.

WHAT THE ROBOT DOES NOW
  Waits for commands like FWD / STOP / LEFT over USB Serial.

HARDWARE
  - Motors + E-STOP (Week 3)
  - USB to laptop
  - Optional buzzer pin 12

HOW TO RUN
  1. Upload this sketch
  2. Open Serial Monitor at 9600
  3. Set line ending to "Newline" (IMPORTANT)
  4. Type: FWD   then Enter
  5. Type: STOP  then Enter
  6. Try LEFT, RIGHT, BACK, SPEED:100, BEEP
  7. Close Serial Monitor before Program 17 Python

CONTROLS (type in Serial Monitor)
  FWD BACK LEFT RIGHT STOP
  SPEED:120
  BEEP LED_ON LED_OFF
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

const int MAX_SPEED = 180;
int driveSpeed = 130;

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
  Serial.println("READY");
  Serial.println("Program 16 — Command Firmware");
  Serial.println("Commands: FWD BACK LEFT RIGHT STOP SPEED:120 BEEP");
}

void loop() {
  // Always honor E-STOP
  if (isEStopPressed()) {
    stopMotors();
    digitalWrite(LED_PIN, HIGH);
    // Avoid flooding Serial — print occasionally
    static unsigned long lastMsg = 0;
    if (millis() - lastMsg > 500) {
      Serial.println("ESTOP:1");
      lastMsg = millis();
    }
    return;
  }
  digitalWrite(LED_PIN, LOW);

  // Read one full command line if available
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();

    if (command.length() == 0) {
      return;
    }

    handleCommand(command);
  }
}

void handleCommand(String command) {
  if (command == "FWD") {
    forward(driveSpeed);
    Serial.println("OK FWD");
  } else if (command == "BACK") {
    drive(driveSpeed, false, false);
    Serial.println("OK BACK");
  } else if (command == "LEFT") {
    drive(driveSpeed, false, true);
    Serial.println("OK LEFT");
  } else if (command == "RIGHT") {
    drive(driveSpeed, true, false);
    Serial.println("OK RIGHT");
  } else if (command == "STOP") {
    stopMotors();
    Serial.println("OK STOP");
  } else if (command == "BEEP") {
    beep(120);
    Serial.println("OK BEEP");
  } else if (command == "LED_ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("OK LED_ON");
  } else if (command == "LED_OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("OK LED_OFF");
  } else if (command.startsWith("SPEED:")) {
    int value = command.substring(6).toInt();
    if (value < 0) value = 0;
    if (value > MAX_SPEED) value = MAX_SPEED;
    driveSpeed = value;
    Serial.print("OK SPEED:");
    Serial.println(driveSpeed);
  } else {
    Serial.print("ERR UNKNOWN ");
    Serial.println(command);
  }
}

bool isEStopPressed() {
  return digitalRead(ESTOP_PIN) == LOW;
}

void drive(int speed, bool leftForward, bool rightForward) {
  setLeft(speed, leftForward);
  setRight(speed, rightForward);
}

void forward(int speed) {
  drive(speed, true, true);
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
 1. Add a SPIN command that turns longer conceptually
    (same as LEFT for now is OK — just parse the word).
 2. After OK FWD, also print the current driveSpeed.
 3. (Bonus) Auto-STOP if no new command arrives for 2 seconds
    (fail-safe for lost laptop link).

EXPECTED RESULT
  - Typing FWD starts motors (E-STOP released)
  - Typing STOP ends motion
  - Unknown words print ERR UNKNOWN ...
  - ESTOP:1 appears when the button is held

COMMON ERRORS
  - Commands do nothing
      → Serial Monitor line ending must include Newline.
  - Garbled text
      → Baud 9600 both sides.
  - FWD while E-STOP pressed
      → Motors stay off; release E-STOP.

INSTRUCTOR NOTE
  This firmware is the contract for Programs 17–20.
  Practice closing Serial Monitor — Python needs the port next.
============================================================
*/
