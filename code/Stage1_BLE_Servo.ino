// ============================================================
//  ARYAN BUILDS — RC Plane Project
//  Stage 1: Prove It Works
//  ESP32 Built-in BLE + MG995 Servo
//
//  Libraries needed: ESP32Servo by Kevin Harrington
//  BLE libraries are built into ESP32 Arduino core
//
//  Wiring:
//    MG995 Signal (Orange) → GPIO 18
//    MG995 Power  (Red)    → External 5V supply (+)
//    MG995 GND    (Brown)  → External GND + ESP32 GND (shared)
//
//  Commands (send via BLE):
//    L → Servo LEFT   (0°)
//    C → Servo CENTER (90°)
//    R → Servo RIGHT  (180°)
// ============================================================

// Libraries needed: ESP32Servo by Kevin Harrington
#include <ESP32Servo.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ── Nordic UART Service UUIDs ────────────────────────────────
#define SERVICE_UUID  "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHAR_UUID_RX  "6e400002-b5a3-f393-e0a9-e50e24dcca9e"  // Phone → ESP32
#define CHAR_UUID_TX  "6e400003-b5a3-f393-e0a9-e50e24dcca9e"  // ESP32 → Phone

// ── Pin Definitions ──────────────────────────────────────────
const int SERVO_PIN = 18;

// ── Servo Angle Settings ─────────────────────────────────────
const int SERVO_LEFT   = 0;
const int SERVO_CENTER = 90;
const int SERVO_RIGHT  = 180;

// ── Objects ──────────────────────────────────────────────────
Servo mg995;
BLECharacteristic* pTxChar;
bool deviceConnected = false;

// ── BLE Connection Callbacks ─────────────────────────────────
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("✅ Phone connected!");
  }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("❌ Disconnected! Restarting advertising...");
    delay(500);
    BLEDevice::startAdvertising();
  }
};

// ── BLE Data Receive Callbacks ───────────────────────────────
class RxCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pChar) {
    String value = pChar->getValue();
    if (value.length() > 0) {
      char cmd = value[0];
      Serial.print("CMD received: ");
      Serial.println(cmd);

      if (cmd == 'L') {
        mg995.write(SERVO_LEFT);
        Serial.println("→ Servo LEFT (0°)");
      }
      else if (cmd == 'R') {
        mg995.write(SERVO_RIGHT);
        Serial.println("→ Servo RIGHT (180°)");
      }
      else if (cmd == 'C') {
        mg995.write(SERVO_CENTER);
        Serial.println("→ Servo CENTER (90°)");
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("=== ARYAN BUILDS — Stage 1 ===");
  Serial.println("PaperPlane BLE starting...");

  // ── Servo Setup ──────────────────────────────────────────
  mg995.setPeriodHertz(50);             // Standard servo: 50 Hz PWM
  mg995.attach(SERVO_PIN, 500, 2400);   // MG995 pulse range
  mg995.write(SERVO_CENTER);
  Serial.println("Servo centered at 90°");

  // ── BLE Setup ────────────────────────────────────────────
  BLEDevice::init("PaperPlane");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  // RX characteristic — receives commands FROM the browser/phone
  BLECharacteristic* pRxChar = pService->createCharacteristic(
    CHAR_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_WRITE_NR
  );
  pRxChar->setCallbacks(new RxCallbacks());

  // TX characteristic — sends data back TO the browser/phone
  pTxChar = pService->createCharacteristic(
    CHAR_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_READ
  );
  pTxChar->addDescriptor(new BLE2902());

  pService->start();

  // ── Start Advertising ────────────────────────────────────
  BLEAdvertising* pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);   // Broadcasts UUID so browser can find it
  pAdv->setScanResponse(true);
  pAdv->setMinPreferred(0x06);
  pAdv->setMaxPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("✅ BLE ready! Device name: PaperPlane");
  Serial.println("Open PaperPlaneController.html in Chrome to connect.");
}

void loop() {
  delay(10);
}
