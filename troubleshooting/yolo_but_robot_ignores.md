# YOLO works on screen but the robot ignores commands

1. Is **Program 16 / 20** firmware uploaded?
2. Is **Serial Monitor closed**?
3. Is `serial_port` correct?
4. Is **E-STOP released**?
5. Does **Program 17 teleop** move the robot?  
   - If no → fix serial/motors first  
   - If yes → debug the YOLO decision / armed state
