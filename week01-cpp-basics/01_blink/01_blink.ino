/*
============================================================
Program 01 — Hello Robot Brain (Blink)
Course: Build Your First AI Robot
Week 1: Meet Your Robot Brain (C++)
============================================================

PURPOSE
  Your robotics journey starts with one skill:
  upload a C++ program and make something physical happen.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - Arduino runs setup() once, then loop() forever
  - digitalWrite can turn a pin ON (HIGH) or OFF (LOW)
  - changing delay() changes the robot's timing

KEY CONCEPT
  Code → pin voltage → LED light

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  This is the first program. Nothing came before it.

WHAT THE ROBOT DOES NOW
  Blinks the built-in LED on and off.

HARDWARE
  - Arduino Uno + USB cable
  - No breadboard needed (uses built-in LED on pin 13)

HOW TO RUN
  1. Open this file in Arduino IDE
  2. Tools → Board → Arduino Uno
  3. Tools → Port → your COM port (Windows) or /dev/cu... (Mac)
  4. Click Upload
  5. Watch the LED near pin 13 blink

CONTROLS
  None yet — this program runs by itself after upload.
============================================================
*/

// Built-in LED on most Arduino Uno boards is connected to pin 13.
// Using a named constant makes the code easier to read.
const int LED_PIN = 13;

// How long the LED stays on/off (milliseconds).
// 1000 ms = 1 second.
const int BLINK_DELAY_MS = 500;

// ------------------------------------------------------------
// setup() runs ONCE when the board powers on or resets
// ------------------------------------------------------------
void setup() {
  // Tell Arduino this pin is an OUTPUT (we will send signals out)
  pinMode(LED_PIN, OUTPUT);
}

// ------------------------------------------------------------
// loop() runs AGAIN and AGAIN forever
// ------------------------------------------------------------
void loop() {
  // Turn the LED on (HIGH = ~5V on this pin)
  digitalWrite(LED_PIN, HIGH);

  // Wait so humans can see the light
  delay(BLINK_DELAY_MS);

  // Turn the LED off (LOW = 0V)
  digitalWrite(LED_PIN, LOW);

  // Wait again while it is dark
  delay(BLINK_DELAY_MS);
}

/*
============================================================
STUDENT CHALLENGE
 1. Change BLINK_DELAY_MS from 500 to 100 (faster blink).
 2. Change it to 2000 (slower blink).
 3. (Bonus) Make an SOS pattern: short, short, short, long, long, long
    by using different delay values (no need for arrays yet).

EXPECTED RESULT
  - Upload succeeds with no errors
  - The tiny LED on the Arduino blinks steadily
  - Faster/slower delay visibly changes the blink

COMMON ERRORS
  - "Board not found" / wrong port
      → Unplug/replug USB; pick the correct Port in Tools.
  - Code uploads but LED never blinks
      → Confirm Board = Arduino Uno; look at the LED marked L near pin 13.
  - Students change code but forget to Upload
      → The board keeps running the OLD program until you upload again.

INSTRUCTOR NOTE
  Celebrate: "You programmed a real computer that lives on the robot."
  Emphasize setup() once vs loop() forever — this is the Arduino heartbeat.
  Connect to Course 1: there you blinked ideas on screen; here you blink hardware.
  Do NOT introduce buttons yet — that is Program 02.
============================================================
*/
