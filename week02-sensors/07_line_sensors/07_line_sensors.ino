/*
============================================================
Program 07 — See the Line (IR Line Sensors)
Course: Build Your First AI Robot
Week 2: Teach Your Robot to Sense
============================================================

PURPOSE
  Many robots follow a path on the floor.
  IR reflectance sensors can tell black line vs light floor.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - each IR sensor usually reads LOW or HIGH (or a thresholded value)
  - left / center / right sensors give a simple "picture" of the line
  - calibration may be needed for your tape and lighting

KEY CONCEPT
  IR sensors → L / C / R bits → where is the line?

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 06 decided from distance.
  Program 07 reads a different sense: the floor line.

WHAT THE ROBOT DOES NOW
  Prints left, center, and right line readings over Serial.

HARDWARE
  - Arduino Uno + USB
  - 3× IR reflectance sensors (TCRT5000-style modules are common)
    If you only have 2 sensors, wire Left + Right and ignore Center in code.

WIRING (digital OUT modules — most beginner kits)
  Each module: VCC→5V, GND→GND
  Left   OUT → pin 4
  Center OUT → pin 5
  Right  OUT → pin 6

  Many modules: ON LINE (black) = LOW, OFF LINE = HIGH
  If yours is inverted, use the challenge to flip the logic.

HOW TO RUN
  1. Wire the three sensors facing the floor
  2. Place black electrical tape as a line on light floor/paper
  3. Upload and open Serial Monitor at 9600
  4. Slide the sensors across the tape and watch 0/1 values

CONTROLS
  Move the robot (or the tape) under the sensors.
============================================================
*/

const int LINE_LEFT_PIN = 4;
const int LINE_CENTER_PIN = 5;
const int LINE_RIGHT_PIN = 6;
const int BAUD_RATE = 9600;

// Set true if your modules read HIGH on black tape instead of LOW
const bool INVERT_LINE_LOGIC = false;

void setup() {
  pinMode(LINE_LEFT_PIN, INPUT);
  pinMode(LINE_CENTER_PIN, INPUT);
  pinMode(LINE_RIGHT_PIN, INPUT);

  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 07 — Line Sensors");
  Serial.println("Format: L C R   (1 = on line, 0 = off line)");
}

void loop() {
  int left = readLineSensor(LINE_LEFT_PIN);
  int center = readLineSensor(LINE_CENTER_PIN);
  int right = readLineSensor(LINE_RIGHT_PIN);

  Serial.print("LINE: ");
  Serial.print(left);
  Serial.print(" ");
  Serial.print(center);
  Serial.print(" ");
  Serial.println(right);

  // Helpful hint for beginners
  if (center == 1 && left == 0 && right == 0) {
    Serial.println("  → center on line");
  } else if (left == 1 && right == 0) {
    Serial.println("  → line toward LEFT");
  } else if (right == 1 && left == 0) {
    Serial.println("  → line toward RIGHT");
  } else if (left == 0 && center == 0 && right == 0) {
    Serial.println("  → line lost");
  }

  delay(200);
}

// Returns 1 if sensor sees the line, 0 otherwise
int readLineSensor(int pin) {
  int raw = digitalRead(pin);

  // Default kit assumption: LOW means black line detected
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
 1. If values look backwards, set INVERT_LINE_LOGIC = true.
 2. Make the built-in LED light only when center == 1.
 3. (Bonus) With only 2 sensors, comment out center and print L R only.

EXPECTED RESULT
  - Sliding across black tape changes the 0/1 pattern
  - Center-on-line often shows 0 1 0 (after correct invert setting)
  - Serial hints match what you see

COMMON ERRORS
  - All zeros or all ones always
      → Height above floor wrong (usually ~2–10 mm); adjust mounts.
  - Inverted readings
      → Flip INVERT_LINE_LOGIC.
  - Analog-only modules (no digital OUT)
      → Use analog pins + threshold (ask instructor for variant).

INSTRUCTOR NOTE
  Do a 5-minute calibration circus: every kit sets invert + height.
  Preview Week 4 line-follow: same L/C/R will steer motors.
  Keep dashboard combine for Program 08.
============================================================
*/
