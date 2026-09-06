/*
============================================================
Program 20 — Robot Brain Firmware (Final Starter)
Course: Build Your First AI Robot
Week 6: Build Your AI Robot
============================================================

PURPOSE
  Final Arduino firmware for the student project.
  Listens for laptop commands; E-STOP always wins.

PAIR WITH
  20_ai_robot_starter.py

COMMANDS
  FWD BACK LEFT RIGHT STOP
  SPEED:120
  BEEP LED_ON LED_OFF

HOW TO RUN
  1. Upload this sketch
  2. Close Serial Monitor
  3. Run the Python starter
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
int driveSpeed = 120;

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
  Serial.println("Program 20 — Robot Brain");
}

void loop() {
  if (isEStopPressed()) {
    stopMotors();
    digitalWrite(LED_PIN, HIGH);
    static unsigned long lastMsg = 0;
    if (millis() - lastMsg > 500) {
      Serial.println("ESTOP:1");
      lastMsg = millis();
    }
    return;
  }
  digitalWrite(LED_PIN, LOW);

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();
    if (command.length() > 0) {
      handleCommand(command);
    }
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
 1. Lower MAX_SPEED for a safer Demo Day crawl.
 2. Add a custom command like SPIN that turns LEFT.
 3. Keep E-STOP demo as part of your presentation.

INSTRUCTOR NOTE
  Students should not need to rewrite firmware for most projects —
  Python SETTINGS/modes are the main remix surface.
============================================================
*/
