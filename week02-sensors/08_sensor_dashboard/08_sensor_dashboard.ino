/*
============================================================
Program 08 — Robot Dashboard (Multi-Sensor Report)
Course: Build Your First AI Robot
Week 2: Teach Your Robot to Sense
============================================================

PURPOSE
  Real robots combine many sensors into one status stream.
  Later, Python (and you) will read this dashboard to decide.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - one loop can read several sensors
  - a simple text format makes data easy to parse later
  - Week 2 skills plug together into a robot "instrument panel"

KEY CONCEPT
  Many sensors → one Serial dashboard line

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Programs 05–07 each focused on one sense.
  Program 08 reports distance AND line sensors together.

WHAT THE ROBOT DOES NOW
  Continuously prints: DIST and LINE values (plus a danger flag).

HARDWARE
  - HC-SR04: TRIG 9, ECHO 10
  - Line sensors: L 4, C 5, R 6
  - Optional: LED 13 + buzzer 8 for danger hint

WIRING
  Same as Programs 05–07 (see hardware/wiring/)

HOW TO RUN
  1. Wire ultrasonic + line sensors
  2. Upload this sketch
  3. Open Serial Monitor at 9600
  4. Wave your hand and slide over tape — watch both update

CONTROLS
  Your hand + the floor line are the inputs.
============================================================
*/

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int LINE_LEFT_PIN = 4;
const int LINE_CENTER_PIN = 5;
const int LINE_RIGHT_PIN = 6;
const int LED_PIN = 13;
const int BUZZER_PIN = 8;

const int BAUD_RATE = 9600;
const float DANGER_CM = 20.0;
const float CM_PER_US = 0.0343;
const bool INVERT_LINE_LOGIC = false;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LINE_LEFT_PIN, INPUT);
  pinMode(LINE_CENTER_PIN, INPUT);
  pinMode(LINE_RIGHT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 08 — Sensor Dashboard");
  Serial.println("Format: DIST:<cm> LINE:<L>,<C>,<R> DANGER:<0|1>");
}

void loop() {
  float distanceCm = readDistanceCm();
  int left = readLineSensor(LINE_LEFT_PIN);
  int center = readLineSensor(LINE_CENTER_PIN);
  int right = readLineSensor(LINE_RIGHT_PIN);

  int danger = 0;
  if (distanceCm > 0 && distanceCm < DANGER_CM) {
    danger = 1;
  }

  // Machine-friendly single line (Python can parse this later)
  Serial.print("DIST:");
  Serial.print(distanceCm);
  Serial.print(" LINE:");
  Serial.print(left);
  Serial.print(",");
  Serial.print(center);
  Serial.print(",");
  Serial.print(right);
  Serial.print(" DANGER:");
  Serial.println(danger);

  if (danger == 1) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(200);
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

int readLineSensor(int pin) {
  int raw = digitalRead(pin);
  bool onLine = (raw == LOW);
  if (INVERT_LINE_LOGIC) {
    onLine = !onLine;
  }
  if (onLine) {
    return 1;
  }
  return 0;
}

/*
============================================================
STUDENT CHALLENGE
 1. Add ESTOP:0 at the end of the line (hardcode for now).
 2. Change DANGER_CM for your arena and note your best value.
 3. (Bonus) Print a second human line under the machine line,
    like "Hand is close!" when DANGER=1.

EXPECTED RESULT
  - Serial lines like: DIST:25.40 LINE:0,1,0 DANGER:0
  - Hand close → DANGER:1 and LED/buzzer on
  - Crossing tape changes LINE bits

COMMON ERRORS
  - One sensor works, the other doesn't
      → Check each wiring loom separately (5V/GND shared).
  - Messy Serial if delay is too small
      → Keep delay(200) for readability.
  - Format typos break future Python parsers
      → Keep DIST: LINE: DANGER: labels exact.

INSTRUCTOR NOTE
  Exit ticket Week 2: "My robot can measure distance and see a line."
  Emphasize the text protocol — Week 5 Python will read similar lines.
  Preview Week 3: these numbers will control motors (carefully!).
============================================================
*/
