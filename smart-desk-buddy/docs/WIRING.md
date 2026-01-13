# Smart Desk Buddy - Wiring Guide

Complete wiring reference for building the Smart Desk Buddy.

## Components Needed

| Component | Qty | Notes |
|-----------|-----|-------|
| ESP32 DevKit | 1 | Main controller |
| ESP32-CAM (AI Thinker) | 1 | Presence detection |
| SSD1306 OLED 128x64 | 1 | I2C, 0.96" |
| Piezo Buzzer | 1 | Passive, 5V |
| Micro Servo (SG90) | 2 | For arms |
| RGB LED (Common Cathode) | 1 | Status indicator |
| 10KΩ Potentiometer | 1 | Volume control |
| 10KΩ Resistor | 2 | Battery voltage divider |
| 220Ω Resistor | 3 | LED current limiting |
| Jumper Wires | Many | Various colors recommended |
| Mini Breadboard | 2 | 1.9" × 1.4" |

## Wiring Diagrams

### OLED Display (I2C)

```
OLED Display          ESP32 DevKit
────────────          ────────────
VCC          ────────→ 3.3V
GND          ────────→ GND
SDA          ────────→ GPIO 21
SCL          ────────→ GPIO 22
```

### Piezo Buzzer

```
Piezo Buzzer          ESP32 DevKit
────────────          ────────────
(+) Positive ────────→ GPIO 32
(-) Negative ────────→ GND
```

### Volume Potentiometer

```
Potentiometer         ESP32 DevKit
─────────────         ────────────
Left Pin     ────────→ 3.3V
Middle Pin   ────────→ GPIO 34
Right Pin    ────────→ GND
```

### Touch Sensors (Capacitive)

```
Touch Inputs          ESP32 DevKit
────────────          ────────────
Head Touch   ────────→ GPIO 4
Left Touch   ────────→ GPIO 15
Right Touch  ────────→ GPIO 13
```

Note: ESP32 has built-in capacitive touch on these pins. Just connect a wire or conductive pad.

### Servo Arms

```
Left Arm Servo        ESP32 DevKit
──────────────        ────────────
Red (power)  ────────→ 5V (VIN)
Brown (gnd)  ────────→ GND
Orange (sig) ────────→ GPIO 26

Right Arm Servo
───────────────
Red (power)  ────────→ 5V (VIN)
Brown (gnd)  ────────→ GND
Orange (sig) ────────→ GPIO 27
```

### RGB LED (Common Cathode)

```
RGB LED               ESP32 DevKit
───────               ────────────
Red pin      ──[220Ω]──→ GPIO 33
GND (long)   ─────────→ GND
Green pin    ──[220Ω]──→ GPIO 14
Blue pin     ──[220Ω]──→ GPIO 12
```

### Battery Monitor (Optional, for battery power)

```
4×AA Battery Pack (6V)
──────────────────────

Battery (+) ──┬──────────→ ESP32 VIN
              │
           [10KΩ]
              │
              ├──────────→ GPIO 35
              │
           [10KΩ]
              │
Battery (-) ──┴──────────→ GND
```

### ESP32-CAM Connection

```
ESP32-CAM             ESP32 DevKit
─────────             ────────────
5V           ←───────→ 5V (VIN)
GND          ←───────→ GND
U0T (TX)     ─────────→ GPIO 16
```

## Complete Pin Reference

### ESP32 DevKit Pin Usage

| GPIO | Function | Notes |
|------|----------|-------|
| 4 | Touch Head | Capacitive touch input |
| 12 | LED Blue | Via 220Ω resistor |
| 13 | Touch Right | Capacitive touch input |
| 14 | LED Green | Via 220Ω resistor |
| 15 | Touch Left | Capacitive touch input |
| 16 | CAM Serial RX | Receives data from ESP32-CAM |
| 21 | OLED SDA | I2C data |
| 22 | OLED SCL | I2C clock |
| 26 | Left Arm Servo | PWM signal |
| 27 | Right Arm Servo | PWM signal |
| 32 | Piezo Buzzer | Tone output |
| 33 | LED Red | Via 220Ω resistor |
| 34 | Volume Pot | Analog input |
| 35 | Battery Monitor | Analog input (optional) |

### Power Requirements

- ESP32 DevKit: 5V via USB or VIN
- ESP32-CAM: 5V (from DevKit VIN)
- Servos: 5V (from DevKit VIN)
- OLED: 3.3V (from DevKit 3.3V)
- Total current: ~500mA typical, ~1A peak (during servo movement)

## Assembly Tips

1. **Test each subsystem separately** before combining
2. **Use different colored wires** for power (red), ground (black), and signals
3. **Keep servo wires short** to reduce noise
4. **Mount OLED securely** - it's the face!
5. **Touch sensors work best** with larger conductive surfaces (coins, foil)
6. **Volume pot orientation** - test which way increases volume

## Troubleshooting

| Issue | Check |
|-------|-------|
| OLED not working | I2C address (try 0x3C or 0x3D), SDA/SCL swapped |
| No sound | Volume pot position, GPIO 32 connection |
| Servos jittering | Insufficient power, add capacitor across servo power |
| Touch too sensitive | Increase TOUCH_THRESHOLD in code |
| Touch not working | Ensure GPIO supports touch (4, 13, 15 do) |
| No CAM data | Check TX→RX connection, baud rate 115200 |
| CAM not detected | Ensure CAM has separate upload, GPIO 0 not grounded |
