/*
============================================================
Program 03 — Talk to the Computer (Serial Hello)
Course: Build Your First AI Robot
Week 1: Meet Your Robot Brain (C++)
============================================================

PURPOSE
  Robots need to send messages back to your laptop.
  Serial communication is how Arduino "talks" over USB —
  and later how Python will send drive commands.

LEARNING OBJECTIVE
  After this program, you should understand that:
  - Serial.begin starts the USB text channel
  - Serial.println prints a line you can read on the computer
  - int variables can count loop cycles

KEY CONCEPT
  Arduino → Serial message → Serial Monitor on laptop

WHAT CHANGED FROM THE PREVIOUS PROGRAM
  Program 02 reacted to a button with an LED.
  Program 03 sends text status to your computer.

WHAT THE ROBOT DOES NOW
  Prints a hello message and a rising counter over Serial.

HARDWARE
  - Arduino Uno + USB cable
  - No extra wiring

HOW TO RUN
  1. Upload this sketch
  2. Tools → Serial Monitor
  3. Set baud rate to 9600 (must match Serial.begin)
  4. Watch messages appear
  5. Close Serial Monitor when finished (important later!)

CONTROLS
  None — it prints automatically.
============================================================
*/

// Baud rate = how fast Serial sends bits.
// 9600 is common and reliable for beginners.
const int BAUD_RATE = 9600;

// Count how many times loop() has run
int loopCount = 0;

void setup() {
  // Start the Serial channel
  Serial.begin(BAUD_RATE);

  // Wait briefly so the Serial Monitor can connect after upload
  delay(1000);

  Serial.println("==============================");
  Serial.println("Program 03 — Serial Hello");
  Serial.println("Robot brain is online.");
  Serial.println("==============================");
}

void loop() {
  loopCount = loopCount + 1;

  // Print text and a number on one line
  Serial.print("Loop count: ");
  Serial.println(loopCount);

  // Slow enough to read in Serial Monitor
  delay(500);
}

/*
============================================================
STUDENT CHALLENGE
 1. Change the greeting to include your name.
 2. Print loopCount only every 2 seconds (change delay).
 3. (Bonus) Also blink the built-in LED each loop (pin 13)
    while printing — combine Program 01 + 03.

EXPECTED RESULT
  - Serial Monitor shows the hello banner once
  - Then "Loop count: 1", "2", "3", ... forever
  - Baud set to 9600

COMMON ERRORS
  - Garbage characters in Serial Monitor
      → Baud rate mismatch (set Monitor to 9600).
  - Nothing appears
      → Open Serial Monitor after upload; check USB cable/port.
  - Leaving Serial Monitor open later
      → Python cannot use the same COM port (Course 1 lesson).

INSTRUCTOR NOTE
  Connect to Course 1: same USB serial idea as LED_ON commands.
  Preview: Week 5 Python will send FWD/STOP over this channel.
  Keep functions/buzzer for Program 04.
============================================================
*/
