# Smart Desk Buddy

An expressive desk companion with animated eyes, reactive sounds, and articulated arms. Built with ESP32, it detects your presence and responds with personality.

## Features

- **Animated OLED Eyes** — Multiple expressions including happy, annoyed, angry, surprised, sleepy, and more
- **Reactive Sounds** — Piezo buzzer plays melodies and sound effects matching the mood
- **Articulated Arms** — Two servo-driven arms with gestures like waving, shrugging, and crossed arms
- **Presence Detection** — ESP32-CAM detects when someone approaches
- **Touch Interaction** — Three capacitive touch zones trigger different reactions
- **Status LED** — RGB LED shows battery level and system status
- **Volume Control** — Potentiometer adjusts sound volume

## How It Works

The Smart Desk Buddy uses two ESP32 boards working together:

1. **ESP32-CAM** captures frames and detects motion/presence using frame differencing
2. **ESP32 DevKit** receives proximity data and controls all outputs (display, sound, servos, LED)

When someone approaches or touches the buddy, it reacts with coordinated eye expressions, sounds, and arm movements. When idle, it exhibits subtle behaviors like looking around, blinking, and occasional sighs.

## Personality

The buddy has a slightly grumpy personality:
- Gets startled or annoyed when you get too close
- Doesn't like being touched on the head
- Occasionally shows rare moments of happiness
- Sighs and looks bored when left alone
- Rolls its eyes when you walk away

## Hardware

| Component | Purpose |
|-----------|---------|
| ESP32 DevKit | Main controller |
| ESP32-CAM | Presence detection |
| SSD1306 OLED 128×64 | Animated eyes |
| Piezo Buzzer | Sound effects |
| 2× Micro Servos | Arm movement |
| RGB LED | Status indicator |
| Potentiometer | Volume control |
| Touch pads | User interaction |

## Repository Structure

```
smart-desk-buddy/
├── README.md
├── arduino/
│   ├── smart_desk_buddy/
│   │   ├── smart_desk_buddy.ino    # Main controller code
│   │   ├── desk_buddy_eyes.h       # OLED eye animations
│   │   ├── desk_buddy_sounds.h     # Sound effects library
│   │   ├── desk_buddy_arms.h       # Servo arm gestures
│   │   └── desk_buddy_status.h     # RGB LED & battery
│   └── esp32cam_presence/
│       └── esp32cam_presence.ino   # Camera presence detection
├── docs/
│   └── WIRING.md                   # Complete wiring guide
└── images/
```

## Quick Start

### 1. Install Dependencies

In Arduino IDE, install these libraries via Library Manager:
- Adafruit SSD1306
- Adafruit GFX Library
- ESP32Servo

### 2. Wire It Up

See [docs/WIRING.md](docs/WIRING.md) for complete wiring instructions.

**Key connections:**
```
OLED:   SDA → GPIO 21, SCL → GPIO 22
Buzzer: (+) → GPIO 32
Servos: Left → GPIO 26, Right → GPIO 27
Touch:  Head → GPIO 4, Left → GPIO 15, Right → GPIO 13
```

### 3. Upload Code

**To ESP32 DevKit:**
1. Open `arduino/smart_desk_buddy/smart_desk_buddy.ino`
2. Select Board: "ESP32 Dev Module"
3. Upload

**To ESP32-CAM:**
1. Open `arduino/esp32cam_presence/esp32cam_presence.ino`
2. Select Board: "AI Thinker ESP32-CAM"
3. Connect GPIO 0 to GND for programming mode
4. Upload, then disconnect GPIO 0
5. Press reset

### 4. Connect & Test

Wire the ESP32-CAM's TX (U0T) to the DevKit's GPIO 16, share GND and 5V, and power on. The buddy should wake up with a stretch and startup melody.

## Customization

### Adjusting Sensitivity

In `smart_desk_buddy.ino`:
```cpp
#define TOUCH_THRESHOLD 40      // Lower = more sensitive
#define PROXIMITY_CLOSE 70      // Motion % to trigger close reaction
```

### Adding Expressions

In `desk_buddy_eyes.h`, create new functions following the pattern:
```cpp
void eyes_myExpression() {
  currentMood = MOOD_CUSTOM;
  drawEyes(lidTop, lidBottom, lidTop, lidBottom, lookX, lookY);
}
```

### Adding Sounds

In `desk_buddy_sounds.h`:
```cpp
void sound_mySound() {
  playTone(NOTE_C5, 200);
  playTone(NOTE_E5, 200);
}
```

### Adding Arm Gestures

In `desk_buddy_arms.h`:
```cpp
void arms_myGesture() {
  moveArmsBoth(ARM_UP, ARM_NEUTRAL, 10);
  delay(300);
  moveArmsBoth(ARM_REST, ARM_REST, 10);
}
```

## Expressions Reference

### Eye Expressions
| Function | Description |
|----------|-------------|
| `eyes_neutral()` | Default relaxed eyes |
| `eyes_happy()` | Curved happy arc eyes |
| `eyes_annoyed()` | Half-lidded annoyed look |
| `eyes_angry()` | Furrowed brows, intense |
| `eyes_sad()` | Droopy, looking down |
| `eyes_skeptical()` | One eyebrow raised |
| `eyes_surprised()` | Wide open, small pupils |
| `eyes_sleepy()` | Nearly closed |
| `eyes_love()` | Heart-shaped eyes |

### Eye Animations
| Function | Description |
|----------|-------------|
| `eyes_blink()` | Quick blink |
| `eyes_roll()` | Full eye roll |
| `eyes_wakeUp()` | Slowly open eyes |
| `eyes_goToSleep()` | Slowly close eyes |
| `eyes_squint()` | Suspicious squint |
| `eyes_lookLeft/Right/Up/Down()` | Look in direction |

### Arm Gestures
| Function | Description |
|----------|-------------|
| `arms_wave()` | Friendly wave |
| `arms_shrug()` | "I don't know" shrug |
| `arms_crossed()` | Angry crossed arms |
| `arms_excited()` | Arms up, wiggling |
| `arms_startled()` | Quick arms up |
| `arms_dismissive()` | Flick away gesture |
| `arms_thinking()` | Scratching head |
| `arms_whatever()` | Limp dismissive wave |

## Power Options

- **USB Power**: Easiest for development
- **4×AA Batteries**: ~6V, good for portable use
- **LiPo Battery**: Use with proper regulation to 5V

The status LED will indicate battery level when running on batteries.

## Future Ideas

- [ ] Add microphone for sound reaction
- [ ] WiFi connectivity for remote control
- [ ] More complex presence tracking (face direction)
- [ ] Programmable personality modes
- [ ] Integration with calendar/notifications

## License

Open source for personal and educational use. Have fun building your own desk buddy!
