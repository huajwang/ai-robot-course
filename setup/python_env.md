# Python environment (Week 5+)

Course 2 reuses Course 1 packages when possible.

```bash
pip install -r requirements.txt
```

Typical packages:

- `pyserial` — talk to Arduino
- `opencv-python` — camera (Programs 18+)
- `ultralytics` — YOLO (Programs 18+)

## Port reminder

1. Upload Arduino firmware  
2. **Close Serial Monitor**  
3. Set `serial_port` in the Python file (`COM3`, etc.)  
4. Run the `.py` program  
