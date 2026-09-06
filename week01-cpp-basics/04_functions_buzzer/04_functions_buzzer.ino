/*
============================================================
Program 04 — Reusable Actions (Functions + Buzzer)
Course: Build Your First AI Robot
Week 1: Meet Your Robot Brain (C++)
============================================================

PURPOSE
  Real robots repeat actions: beep for alarm, blink for ready.
  Functions let you name an action once and reuse it.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - a function is a reusable block of code with a name
  - constants keep pin numbers and timings easy to change
  - you can build different "behaviors" from small actions

KEY CONCEPT
  Function = named action the robot can perform

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 03 printed status over Serial.
  Program 04 adds functions that play LED/buzzer patterns.

WHAT THE ROBOT DOES NOW
  Plays a "ready" beep pattern, then an "alarm" pattern, forever.

HARDWARE
  - Arduino Uno + USB
  - Active buzzer: + to pin 8, - to GND
  - Built-in LED on pin 13 (no wiring)

WIRING
        Arduino pin 8 ----(+) ACTIVE BUZZER (-)---- GND

  (Passive buzzers need tone() and sound quieter/different —
   this course uses an ACTIVE buzzer when possible.)

HOW TO RUN
  1. Wire the active buzzer
  2. Upload this sketch
  3. Listen/watch: ready pattern, then alarm pattern, repeat
  4. Optional: open Serial Monitor at 9600 to see labels

CONTROLS
  None — patterns run automatically.
============================================================
*/

const int LED_PIN = 13;
const int BUZZER_PIN = 8;
const int BAUD_RATE = 9600;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 04 — Functions + Buzzer");
}

void loop() {
  Serial.println("Behavior: READY");
  playReady();
  delay(800);

  Serial.println("Behavior: ALARM");
  playAlarm();
  delay(1200);
}

// ------------------------------------------------------------
// Small reusable actions
// ------------------------------------------------------------

// Turn buzzer on for onTimeMs, then off for offTimeMs
void beep(int onTimeMs, int offTimeMs) {
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(onTimeMs);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  delay(offTimeMs);
}

// Friendly short chirps = "I'm ready"
void playReady() {
  beep(100, 100);
  beep(100, 100);
}

// Urgent longer beeps = "alarm"
void playAlarm() {
  beep(300, 150);
  beep(300, 150);
  beep(300, 400);
}

/*
============================================================
STUDENT CHALLENGE
 1. Create playHappy() with three very short beeps and call it in loop().
 2. Change playAlarm() to five beeps.
 3. (Bonus) Call playReady() only once in setup(), and playAlarm()
    in loop() — notice setup vs loop again.

EXPECTED RESULT
  - LED and buzzer pulse together
  - Distinct READY vs ALARM rhythms
  - Serial labels match what you hear

COMMON ERRORS
  - No sound
      → Check active buzzer polarity (+ to pin 8); try swapping legs once.
  - Buzzer always on
      → Confirm you digitalWrite LOW in beep(); check wiring short.
  - Forgetting to declare functions before use
      → In this sketch, helpers are below loop(); Arduino IDE allows that
        for functions in the same .ino file.

INSTRUCTOR NOTE
  Exit ticket Week 1: "I can output, input, print Serial, and write functions."
  Say: later alarms/modes will call functions like these.
  Preview Week 2: sensors will choose WHICH function to call.
============================================================
*/
