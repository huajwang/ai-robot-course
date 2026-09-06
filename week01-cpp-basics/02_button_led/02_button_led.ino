/*
============================================================
Program 02 — Input Makes Decisions (Button → LED)
Course: Build Your First AI Robot
Week 1: Meet Your Robot Brain (C++)
============================================================

PURPOSE
  Robots do not only OUTPUT (blink). They also INPUT.
  A button is a simple sensor: pressed or not pressed.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - pins can be INPUT as well as OUTPUT
  - digitalRead tells you HIGH or LOW
  - if statements let the robot decide what to do

KEY CONCEPT
  Button (input) → if decision → LED (output)

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 01 only blinked on a timer.
  Program 02 reacts to a human pressing a button.

WHAT THE ROBOT DOES NOW
  LED turns on while the button is held down.

HARDWARE
  - Arduino Uno + USB
  - Pushbutton
  - 10kΩ resistor (pull-down) OR use INPUT_PULLUP (recommended)

WIRING (INPUT_PULLUP method — fewer parts)
  - One button leg → Arduino pin 2
  - Other button leg → GND
  - No external resistor needed (Arduino pull-up is ON)

HOW TO RUN
  1. Wire the button as above
  2. Open this sketch in Arduino IDE
  3. Upload
  4. Hold the button — LED should light
  5. Release — LED should turn off

CONTROLS
  Hold button = LED ON
  Release button = LED OFF
============================================================
*/

const int LED_PIN = 13;      // built-in LED
const int BUTTON_PIN = 2;    // button connected here

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // INPUT_PULLUP means:
  // - the pin reads HIGH when the button is NOT pressed
  // - the pin reads LOW when the button IS pressed (connects to GND)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Read the button once each loop
  int buttonState = digitalRead(BUTTON_PIN);

  // With INPUT_PULLUP: pressed = LOW
  if (buttonState == LOW) {
    digitalWrite(LED_PIN, HIGH);   // button pressed → light ON
  } else {
    digitalWrite(LED_PIN, LOW);    // button released → light OFF
  }
}

/*
============================================================
STUDENT CHALLENGE
 1. Swap the if/else so the LED is ON when released and OFF when pressed.
 2. Add a short delay(50) after reading to reduce button "bounce" flicker.
 3. (Bonus) Toggle mode: each press flips the LED and keeps it that way
    until the next press (harder — ask instructor for a hint).

EXPECTED RESULT
  - Holding the button lights the built-in LED
  - Releasing turns it off
  - No upload errors

COMMON ERRORS
  - LED always on or always off
      → Check wiring to GND; confirm BUTTON_PIN matches your wire.
  - Thinking pressed should be HIGH with INPUT_PULLUP
      → With pull-up, pressed connects to GND → reads LOW.
  - Floating pin (no pull-up / bad wiring)
      → LED flickers randomly; use INPUT_PULLUP as in this sketch.

INSTRUCTOR NOTE
  Say: "This is Sense → Think → Act in miniature."
  Preview: the E-STOP button in Week 3 uses the same idea.
  Keep Serial for Program 03.
============================================================
*/
