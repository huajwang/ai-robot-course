/*
============================================================
Program 10 — Emergency Stop + Drive
Course: Build Your First AI Robot
Week 3: Teach Your Robot to Move
============================================================

PURPOSE
  Real robots need a kill switch.
  E-STOP must beat every drive command — no exceptions.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - safety checks come BEFORE motion
  - a released E-STOP is required to move
  - pressing E-STOP immediately calls stopMotors()

KEY CONCEPT
  E-STOP priority: if pressed → STOP (ignore drive)

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 09 pulsed motors on a timer.
  Program 10 only drives when the E-STOP button is released.

WHAT THE ROBOT DOES NOW
  When safe: short forward pulses.
  When E-STOP pressed: motors off immediately.

HARDWARE
  - Same motor driver wiring as Program 09
  - E-STOP pushbutton: pin 2 → button → GND (INPUT_PULLUP)
  - Use a big, easy-to-hit button if possible

WIRING
  Motors: see hardware/wiring/motors_tb6612.md
  E-STOP: see hardware/wiring/estop.md

SAFETY
  - Label the button EMERGENCY STOP
  - Practice pressing it during a pulse
  - Wheels up for first test

HOW TO RUN
  1. Upload with wheels lifted
  2. Hold E-STOP down → should NOT move
  3. Release E-STOP → short forward pulses
  4. Press E-STOP mid-move → instant stop

CONTROLS
  Release E-STOP = allow motion
  Press E-STOP = halt
============================================================
*/

const int PWMA = 3;
const int AIN1 = 7;
const int AIN2 = 8;
const int PWMB = 11;
const int BIN1 = A0;
const int BIN2 = A1;
const int STBY = A2;

const int ESTOP_PIN = 2;     // INPUT_PULLUP: pressed = LOW
const int BUZZER_PIN = 12;
const int LED_PIN = 13;
const int BAUD_RATE = 9600;

const int SPEED = 140;
const int PULSE_MS = 800;

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
  Serial.println("Program 10 — E-STOP + Drive");
  Serial.println("Pressed E-STOP = LOW = HALT");
}

void loop() {
  // --- NEW IDEA: check safety FIRST every loop ---
  if (isEStopPressed()) {
    stopMotors();
    digitalWrite(LED_PIN, HIGH);   // LED on = halted
    Serial.println("E-STOP ACTIVE — motors blocked");
    delay(150);
    return;  // skip the rest of loop (no driving)
  }

  digitalWrite(LED_PIN, LOW);
  Serial.println("SAFE — forward pulse");
  beep(60);

  // Drive in tiny slices so we can notice E-STOP quickly
  unsigned long startMs = millis();
  while (millis() - startMs < PULSE_MS) {
    if (isEStopPressed()) {
      stopMotors();
      Serial.println("E-STOP during move!");
      digitalWrite(LED_PIN, HIGH);
      return;
    }
    forward(SPEED);
    delay(20);
  }

  stopMotors();
  Serial.println("STOP (end of pulse)");
  delay(1500);
}

bool isEStopPressed() {
  // INPUT_PULLUP: pressed connects to GND → LOW
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
 1. Make the buzzer sound continuously while E-STOP is active.
 2. Change PULSE_MS and practice hitting E-STOP mid-pulse.
 3. (Bonus) Require E-STOP to be released for 1 full second
    before allowing motion again (anti-jitter arming).

EXPECTED RESULT
  - E-STOP held → no motion, LED on, Serial says blocked
  - E-STOP released → short forward pulses
  - Pressing during a pulse stops immediately

COMMON ERRORS
  - Robot never moves
      → Button wiring stuck LOW, or finger holding E-STOP.
  - E-STOP does nothing
      → Confirm pin 2 and INPUT_PULLUP; pressed must read LOW.
  - Still using delay(PULSE_MS) only
      → This program slices the pulse so E-STOP can interrupt.

INSTRUCTOR NOTE
  Make every student demonstrate one successful mid-move E-STOP.
  Rule: after Program 10, no drive demo without E-STOP wired.
  Tank steering next — keep E-STOP in every sketch.
============================================================
*/
