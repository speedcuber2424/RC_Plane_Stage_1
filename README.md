# Stage 1 — Prove It Works ✅

**Status: COMPLETE**
**Cost: ₹0 (using components already owned)**
**Time: 1 weekend**

---

## 🎯 Goal

ESP32 + BLE. Browser sends a command. Servo moves.
That's it. No flying. No frame. Just proving that electronics obey a phone.
This one step is the foundation of every RC vehicle ever built.

---

## 🔌 Wiring

| MG995 Wire | Connect To |
|------------|------------|
| Orange (Signal) | ESP32 GPIO 18 |
| Red (Power) | External 5V supply (+) |
| Brown (GND) | External GND **AND** ESP32 GND |

> ⚠️ **NEVER** power the MG995 from the ESP32's 5V pin.
> The MG995 draws up to 1A under load — this will crash/damage your ESP32.
> Always use an external 5V supply and share the GND.

---

## 📦 Parts List

- ESP32 development board × 1
- MG995 servo motor × 1
- External 5V power supply (≥1A) × 1 — USB power bank works great
- Jumper wires × 3
- Breadboard × 1

---

## 💻 Libraries to Install

Open Arduino IDE → Sketch → Include Library → Manage Libraries:

- **ESP32Servo** by Kevin Harrington ← install this

BLE libraries are built into the ESP32 Arduino core — no install needed.

---

## 📱 Controller

Open `PaperPlaneController.html` in **Google Chrome** on your laptop/PC.

| Button | Key | Command | Action |
|--------|-----|---------|--------|
| LEFT | ← Arrow | `L` | Servo → 0° |
| CENTER | ↑ / Space | `C` | Servo → 90° |
| RIGHT | → Arrow | `R` | Servo → 180° |

---

## 🎬 YouTube Video

**Title:** I Built a Bluetooth Servo Controller From Scratch | ARYAN BUILDS EP.1

**Description:**
> In Stage 1 of our RC plane build, we prove the concept — an ESP32 microcontroller receives commands over Bluetooth from a browser and moves a servo motor. No app needed, no phone required — just Chrome and a file we built ourselves. This is the foundation of every RC vehicle ever built. Follow along — we're going all the way to autonomous flight. #arduino #esp32 #robotics #DIY #aryanbuilds

---

## 🔧 Troubleshooting

| Problem | Fix |
|---------|-----|
| "PaperPlane" not found in Chrome | Make sure ESP32 is powered and code is uploaded |
| Servo jitters | Power it from external supply, not ESP32 5V pin |
| Can't upload code | Hold BOOT button on ESP32 while clicking Upload |
| "No Services matching UUID" error | Make sure you uploaded the latest code from this repo |
