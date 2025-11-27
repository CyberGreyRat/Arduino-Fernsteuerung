/*
 * SENDER TEST & VISUALISIERUNG (V2 - Invertiert & HighSpeed)
 * ----------------------------
 * Fixes:
 * - Gyro Roll invertiert
 * - Joystick Rechts (Roll) invertiert
 * - Joystick Links (Gas) invertiert
 * - Serial Speed auf 20ms (50Hz) erhöht für flüssige Grafik
 * * Hardware Pinout (Dein Nano):
 * nRF24:     D9, D10, D11, D12, D13
 * MPU6050:   A4, A5
 * Joysticks: A0, A1, A2, A3
 * Rotary:    D2, D3
 * Button:    D4
 * LED:       D5
 Perfekter Testcode
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

// --- KONFIGURATION ---
RF24 radio(9, 10); 
const uint64_t pipeAddress = 0xE8E8F0F0E1LL;

#define PIN_JOY_L_Y  A0 
#define PIN_JOY_L_X  A1 
#define PIN_JOY_R_X  A2 
#define PIN_JOY_R_Y  A3 

#define PIN_ROT_CLK  2
#define PIN_ROT_DT   3
#define PIN_BTN_MODE 4
#define PIN_LED_MODE 5

const int MPU_ADDR = 0x68;

struct ControlData {
  byte gas;
  byte pitch;
  byte roll;
};
ControlData data;

volatile int encoderPos = 0;
int lastEncoded = 0;
bool gyroMode = false;       
bool lastBtnState = HIGH;
unsigned long lastTxTime = 0;
unsigned long lastSerialTime = 0; 

void updateEncoder() {
  int MSB = digitalRead(PIN_ROT_CLK); 
  int LSB = digitalRead(PIN_ROT_DT);
  int encoded = (MSB << 1) | LSB; 
  int sum  = (lastEncoded << 2) | encoded; 
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPos++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPos--;
  lastEncoded = encoded;
}

void setup() {
  Serial.begin(115200); 
  
  pinMode(PIN_BTN_MODE, INPUT_PULLUP);
  pinMode(PIN_LED_MODE, OUTPUT);
  pinMode(PIN_ROT_CLK, INPUT_PULLUP);
  pinMode(PIN_ROT_DT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_ROT_CLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ROT_DT), updateEncoder, CHANGE);

  if (!radio.begin()) {
     for(int i=0; i<5; i++) { digitalWrite(PIN_LED_MODE, HIGH); delay(100); digitalWrite(PIN_LED_MODE, LOW); delay(100); }
  }
  radio.setChannel(125);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(pipeAddress);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  // --- Toggle Logik ---
  int btnState = digitalRead(PIN_BTN_MODE);
  if (lastBtnState == HIGH && btnState == LOW) {
    gyroMode = !gyroMode; 
    digitalWrite(PIN_LED_MODE, gyroMode ? HIGH : LOW);
    if (gyroMode) encoderPos = 0; 
    delay(50); 
  }
  lastBtnState = btnState;

  // --- Daten erfassen ---
  if (gyroMode) {
    // Gyro + Rotary
    if (encoderPos < 0) encoderPos = 0;
    if (encoderPos > 10) encoderPos = 10;
    data.gas = encoderPos * 25; 
    if (data.gas > 255) data.gas = 255;

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 4, true); 
    int16_t accX = (Wire.read() << 8 | Wire.read());
    int16_t accY = (Wire.read() << 8 | Wire.read());
    
    // GYRO ROLL INVERTIERT: map(..., 255, 0) statt 0, 255
    data.roll  = map(accX, -17000, 17000, 255, 0); 
    data.pitch = map(accY, -17000, 17000, 0, 255); // Pitch war okay? Sonst hier auch tauschen
  } else {
    // Joystick
    // JOYSTICK GAS INVERTIERT: 255, 0
    data.gas   = map(analogRead(PIN_JOY_L_Y), 0, 1023, 255, 0);
    
    // JOYSTICK ROLL INVERTIERT: 255, 0
    data.roll  = map(analogRead(PIN_JOY_R_X), 0, 1023, 255, 0);
    
    data.pitch = map(analogRead(PIN_JOY_R_Y), 0, 1023, 0, 255);
  }
  
  // Constrain für Sicherheit
  data.roll = constrain(data.roll, 0, 255);
  data.pitch = constrain(data.pitch, 0, 255);

  // --- Senden (Funk) ---
  unsigned long now = millis();
  if (now - lastTxTime > 20) {
    radio.write(&data, sizeof(ControlData));
    lastTxTime = now;
  }

  // --- Senden (Serial Visualisierung - JSON) ---
  // JETZT SCHNELL: 20ms = 50 Updates pro Sekunde
  if (now - lastSerialTime > 20) {
    Serial.print("{\"m\":\"");
    Serial.print(gyroMode ? "GYRO" : "JOY");
    Serial.print("\",\"g\":");
    Serial.print(data.gas);
    Serial.print(",\"p\":");
    Serial.print(data.pitch);
    Serial.print(",\"r\":");
    Serial.print(data.roll);
    Serial.println("}");
    
    lastSerialTime = now;
  }
}