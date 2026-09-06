/*
============================================================
Program 05 — Measure Distance (Ultrasonic)
Course: Build Your First AI Robot
Week 2: Teach Your Robot to Sense
============================================================

PURPOSE
  Robots need to know what is in front of them.
  An ultrasonic sensor measures distance using sound pulses.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - TRIG sends a pulse; ECHO listens for the bounce
  - time-of-flight can be converted into centimeters
  - sensor values are just numbers your code can use

KEY CONCEPT
  Sound pulse → echo time → distance in cm

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Week 1 controlled outputs (LED/buzzer) and Serial.
  Program 05 reads a real SENSOR.

WHAT THE ROBOT DOES NOW
  Prints the distance to the nearest object, about twice per second.

HARDWARE
  - Arduino Uno + USB
  - HC-SR04 ultrasonic sensor

WIRING (HC-SR04)
  VCC  → 5V
  GND  → GND
  TRIG → pin 9
  ECHO → pin 10

  Point the sensor at a wall or your hand.

HOW TO RUN
  1. Wire the sensor carefully (5V, not 3.3V)
  2. Upload this sketch
  3. Open Serial Monitor at 9600
  4. Move your hand closer/farther and watch cm change

CONTROLS
  None — wave your hand in front of the sensor.
============================================================
*/

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int BAUD_RATE = 9600;

// Speed of sound approx. 0.0343 cm per microsecond
// distance = (time * 0.0343) / 2  (divide by 2 = out and back)
const float CM_PER_US = 0.0343;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("Program 05 — Ultrasonic Distance");
  Serial.println("Move your hand in front of the sensor...");
}

void loop() {
  float distanceCm = readDistanceCm();

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  delay(500);
}

// ------------------------------------------------------------
// Read one distance sample from HC-SR04
// ------------------------------------------------------------
float readDistanceCm() {
  // 1) Make sure TRIG is LOW briefly
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 2) Send a 10 microsecond HIGH pulse to start measurement
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 3) pulseIn waits for ECHO to go HIGH, then measures how long it stays HIGH
  //    timeout 30000 us (~5 meters) prevents infinite waiting if no echo
  long durationUs = pulseIn(ECHO_PIN, HIGH, 30000);

  // No echo received
  if (durationUs == 0) {
    return -1.0;
  }

  // 4) Convert time to centimeters
  float distanceCm = (durationUs * CM_PER_US) / 2.0;
  return distanceCm;
}

/*
============================================================
STUDENT CHALLENGE
 1. Print a warning line when distanceCm is between 0 and 20.
 2. Take 3 readings and print the average (hint: call readDistanceCm
    three times and divide by 3.0).
 3. (Bonus) If distance is -1, print "No echo" instead of the number.

EXPECTED RESULT
  - Serial shows changing cm values as you move your hand
  - Closer hand → smaller number
  - Far / angled → larger number or -1 (timeout)

COMMON ERRORS
  - Always -1 or weird values
      → Check TRIG/ECHO pins; sensor must see a target; try 5V power.
  - Sensor gets warm / board resets
      → Confirm GND is shared; avoid powering motors from USB yet.
  - Mixing up TRIG and ECHO wires
      → TRIG is output from Arduino; ECHO is input to Arduino.

INSTRUCTOR NOTE
  Emphasize: sensors give NUMBERS; decisions come next (Program 06).
  Demo with a ruler for rough calibration trust-building.
  Keep motor talk out — wheels are Week 3.
============================================================
*/
