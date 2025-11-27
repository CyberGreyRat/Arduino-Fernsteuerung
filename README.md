✈️ DIY RC Flight Controller & Telemetrie System

Ein selbstentwickeltes, hybrides Fernsteuerungssystem für RC-Flugzeuge (V-Leitwerk), basierend auf Arduino Nano und ESP32. Das System bietet eine Dual-Mode Steuerung (Joystick & Gyroskop-Handsteuerung) sowie eine Echtzeit-3D-Visualisierung im Webbrowser.

Die selbstgebaute Fernsteuerung im Einsatz.

✨ Features

Dual-Mode Steuerung: Umschaltbar zwischen klassischem Joystick-Flug und intuitiver Gyroskop-Steuerung (Handneigung).

Long Range: Optimiert für hohe Reichweite (>300m) durch RF24_PA_MAX und 250kbps Datenrate.

Web Visualisierung: 3D-Telemetrie im Browser via Web Serial API (Three.js).

Fail-Safe: Automatische Motorabschaltung und Ruder-Zentrierung bei Signalverlust (>1s).

V-Tail Support: Kompatibel mit V-Leitwerken und externen Flight Stabilizern (Pass-Through Modus).

Smart Gas: Motorsteuerung über Joystick oder präzisen Rotary Encoder (im Gyro-Modus).

🛠️ Hardware Aufbau

1. Schaltplan (Sender & Empfänger)

Die Kommunikation erfolgt über nRF24L01+ Module mit PA+LNA für maximale Sendeleistung.

Schematische Darstellung der Verkabelung (Arduino Nano Sender & ESP32 Empfänger).

Komponenten Liste

Sender (TX)

Empfänger (RX)

Arduino Nano (ATmega328P)

ESP32 Development Board

nRF24L01+ (PA+LNA)

nRF24L01+ (PA+LNA)

MPU-6050 (Gyroskop)

2x Servos (V-Tail)

2x Analog Joysticks

1x ESC (Motorregler)

Rotary Encoder (KY-040)

Flight Stabilizer (Optional)

Mode-Button & Status LED



🖥️ Web Visualisierung

Das System sendet Telemetrie-Daten (JSON) per USB an den PC. Eine begleitende Web-App visualisiert die Fluglage in Echtzeit mit einem 3D-Modell.

Echtzeit-Darstellung von Pitch und Roll im Browser.

Die Tankanzeige und der Modus-Indikator reagieren live auf Eingaben.

🕹️ Bedienung

Modi Wechseln

Ein Druck auf den Mode-Button schaltet zwischen den Modi um:

Joystick Modus (LED AUS): Klassische Steuerung über die Sticks. Gas liegt auf dem linken Stick.

Gyro Modus (LED AN): Steuerung durch Neigen der Fernbedienung. Gas wird über den Rotary Encoder in 10%-Schritten geregelt.

Detailansicht der Bedienelemente.

Fail-Safe Test

Das System schaltet den Motor automatisch ab, wenn 1 Sekunde lang keine Pakete empfangen werden.

Test: Sender ausschalten -> Motor stoppt sofort, Servos gehen in Neutralstellung.

📦 Installation & Code

Bibliotheken

Folgende Libraries werden in der Arduino IDE benötigt:

RF24 (TMRh20)

ESP32Servo (für den Empfänger)

Wire & SPI (Standard)

Setup

Sender: Sender_Live_Final.ino auf den Arduino Nano flashen.

Empfänger: Receiver_ESP32_Final.ino auf den ESP32 flashen.

Visualisierung: Flight_Visualizer_GLB.html und plane.glb in einen Ordner legen und öffnen (via Live Server oder lokalem Python Server).

⚠️ Sicherheitshinweis

Dieses Projekt arbeitet mit leistungsstarken Motoren und Propellern.

Propeller erst montieren, wenn Fail-Safe und Wirkrichtungen der Ruder erfolgreich getestet wurden!

Achte auf lokale Gesetze bezüglich Sendeleistung (2.4GHz) und Modellflug.

Status: ✅ Live Betrieb bereit

Version: 1.0.0
