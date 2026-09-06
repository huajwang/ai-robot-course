# Setup — Arduino IDE (Week 1)

1. Install Arduino IDE 2.x from https://www.arduino.cc/en/software
2. Connect Arduino Uno with USB
3. In Arduino IDE:
   - **Tools → Board → Arduino Uno**
   - **Tools → Port →** select your board’s port
4. Open a program folder’s `.ino` file (folder name must match the `.ino` name)
5. Click **Verify**, then **Upload**

## Windows tip

Device Manager often shows the board as `COMx` (example: `COM3`).

## Mac tip

Port names often look like `/dev/cu.usbmodemXXXX`.

## Rule

Close Serial Monitor before other apps need the port (important later with Python).
