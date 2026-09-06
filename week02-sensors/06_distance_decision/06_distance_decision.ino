/*
============================================================
Program 06 — Too Close? (Distance Decision)
Course: Build Your First AI Robot
Week 2: Teach Your Robot to Sense
============================================================

PURPOSE
  A number from a sensor is not enough.
  Robots need a RULE: if too close → warn / stop later.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - a threshold is a cutoff you choose (example: 20 cm)
  - if/else turns sensor data into a decision
  - Sense → Think → Act can happen without motors yet

KEY CONCEPT
  Distance → compare to threshold → LED/buzzer action

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 05 only printed distance.
  Program 06 DECIDES when the reading is dangerous.

WHAT THE ROBOT DOES NOW
  Beeps/lights when something is closer than the threshold.

HARDWARE
  - HC-SR04 (same as Program 05)
  - Built-in LED pin 13
  - Active buzzer on pin 8 (optional but recommended)

WIRING
  Ultrasonic: VCC→5V, GND→GND, TRIG→9, ECHO→10
  Buzzer: + → pin 8, − → GND

HOW TO RUN
  1. Upload this sketch
  2. Open Serial Monitor at 9600
  3. Move your hand closer than 20 cm → warning
  4. Move farther → clear

CONTROLS
  None — your hand is the input.
============================================================
*/

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int LED_PIN = 13;
const int BUZZER_PIN = 8;
const int BAUD_RATE = 9600;

// Tune this for your classroom!
const float DANGER_CM = 20.0;

const float CM_PER_US = 0.0343;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 06 — Distance Decision");
  Serial.print("Danger threshold: ");
  Serial.print(DANGER_CM);
  Serial.println(" cm");
}

void loop() {
  float distanceCm = readDistanceCm();

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm  →  ");

  // --- NEW IDEA: decision from a threshold ---
  if (distanceCm > 0 && distanceCm < DANGER_CM) {
    Serial.println("TOO CLOSE");
    warnOn();
  } else {
    Serial.println("CLEAR");
    warnOff();
  }

  delay(200);
}

void warnOn() {
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
}

void warnOff() {
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
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

/*
============================================================
STUDENT CHALLENGE
 1. Change DANGER_CM to 10 and to 40. Which feels better?
 2. Make the buzzer pulse (on/off) while TOO CLOSE instead of solid on.
 3. (Bonus) Add a middle zone: 20–40 cm prints "CAUTION" with LED only.

EXPECTED RESULT
  - Far hand → Serial says CLEAR, quiet/dark
  - Close hand → TOO CLOSE, LED + buzzer on
  - Threshold is easy to find at the top of the file

COMMON ERRORS
  - Always CLEAR even when close
      → Check wiring; print raw distance first (Program 05).
  - Always TOO CLOSE
      → Lower DANGER_CM, or sensor stuck reading small values.
  - -1 distance treated as danger
      → This program requires distanceCm > 0 before warning.

INSTRUCTOR NOTE
  Say: "Later the Act will be STOP motors — same if statement."
  Let students argue about the best threshold for their arena.
  Line sensors are next — keep wheels powered off.
============================================================
*/
