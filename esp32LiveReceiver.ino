/*
 * FLUG-EMPFÄNGER (ESP32) - V-LEITWERK MIXER
 * -----------------------------------------
 * Hardware:
 * nRF24: CE(4), CSN(5)
 * Gas (ESC):       Pin 25
 * V-Tail Links:    Pin 26
 * V-Tail Rechts:   Pin 27
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP32Servo.h>

RF24 radio(4, 5); 
const uint64_t pipeAddress = 0xE8E8F0F0E1LL;

Servo escGas;
Servo servoLeft;  // V-Leitwerk Links
Servo servoRight; // V-Leitwerk Rechts

// Pins
#define PIN_ESC      25
#define PIN_V_LEFT   26
#define PIN_V_RIGHT  27

struct ControlData {
  byte gas;
  byte pitch;
  byte roll; // Wird beim V-Leitwerk als "Seite/Lenken" genutzt
};
ControlData data;

unsigned long lastRecvTime = 0;
const int TIMEOUT = 1000; // 1 Sekunde

void setup() {
  Serial.begin(115200);

  // Funk
  if (!radio.begin()) { Serial.println("nRF Fehler"); while(1); }
  radio.setChannel(125);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS); 
  radio.setPALevel(RF24_PA_MAX);   
  radio.openReadingPipe(1, pipeAddress);
  radio.startListening();

  // Servos
  escGas.attach(PIN_ESC, 1000, 2000); 
  servoLeft.attach(PIN_V_LEFT);
  servoRight.attach(PIN_V_RIGHT);

  // Sicherer Startzustand
  escGas.write(0);
  servoLeft.write(90);  // Mitte
  servoRight.write(90); // Mitte
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(ControlData));
    lastRecvTime = millis();
  }

  // Fail-Safe
  if (millis() - lastRecvTime > TIMEOUT) {
    data.gas = 0;     // Motor AUS
    data.pitch = 127; // Mitte
    data.roll = 127;  // Mitte
  }

  // --- V-TAIL MIXER LOGIK ---
  
  // 1. Eingaben normalisieren (von 0..255 zu -50..+50 Grad Ausschlag)
  // Wir nehmen +/- 50 Grad, damit wir bei Vollausschlag (Mix) nicht über 180 kommen
  int inputPitch = map(data.pitch, 0, 255, -50, 50);
  int inputRoll  = map(data.roll, 0, 255, -50, 50);

  // 2. Mischen
  // Links:  Höhe + Seite
  // Rechts: Höhe - Seite
  // (Vorzeichen ggf. tauschen, je nach Servo-Einbaulage)
  int angleLeft  = 90 + inputPitch + inputRoll;
  int angleRight = 90 + inputPitch - inputRoll;

  // 3. Begrenzen (Constrain) auf Servo-Bereich 0-180
  angleLeft  = constrain(angleLeft, 0, 180);
  angleRight = constrain(angleRight, 0, 180);

  // 4. Ausgabe
  escGas.write(map(data.gas, 0, 255, 0, 180));
  servoLeft.write(angleLeft);
  servoRight.write(angleRight);
}