# Emergency stop button wiring

```text
Arduino pin 2  ----  pushbutton  ----  GND
```

Use `INPUT_PULLUP` in code (no external resistor required).

- **Released:** pin reads HIGH → motion allowed  
- **Pressed:** pin reads LOW → motors must STOP  

## Classroom tips

- Use a large, obvious button
- Label it **E-STOP**
- Mount where a student can slap it quickly
- Practice mid-move stops before any audience demo
