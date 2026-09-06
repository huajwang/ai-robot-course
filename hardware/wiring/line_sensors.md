# IR line sensor wiring

## Typical 3-sensor digital modules

```text
Each module:  VCC → 5V    GND → GND

Left   OUT → Arduino pin 4
Center OUT → Arduino pin 5
Right  OUT → Arduino pin 6
```

## Mounting

- Sensors face the floor
- Height often works around a few millimeters to ~1 cm (kit-dependent)
- Use matte black tape on a light surface

## Logic

Many modules: **LOW = black line**. If yours is opposite, set in code:

```cpp
const bool INVERT_LINE_LOGIC = true;
```
