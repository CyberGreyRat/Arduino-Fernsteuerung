# DIY RC Flight Controller & Telemetrie System 
## Arduino NANO    nRF24L01+    MPU6050
![IMG_1229](https://github.com/user-attachments/assets/c8b44c54-5937-405c-8065-a9cfee117a0c)

Ein selbstentwickeltes, hybrides Fernsteuerungssystem für RC-Flugzeuge , basierend auf Arduino Nano und ESP32. Das System bietet eine Dual-Mode Steuerung (Joystick & Gyroskop-Handsteuerung) sowie eine Echtzeit-3D-Visualisierung im Webbrowser.

Die selbstgebaute Fernsteuerung im Einsatz.

## Features

Dual-Mode Steuerung: Umschaltbar zwischen klassischem Joystick-Flug und intuitiver Gyroskop-Steuerung (Handneigung).

Long Range: Optimiert für hohe Reichweite (>300m) durch RF24_PA_MAX und 250kbps Datenrate.

Web Visualisierung: 3D-Telemetrie im Browser via Web Serial API (Three.js).

Fail-Safe: Automatische Motorabschaltung und Ruder-Zentrierung bei Signalverlust (>1s).

V-Tail Support: Kompatibel mit V-Leitwerken und externen Flight Stabilizern (Pass-Through Modus).

Smart Gas: Motorsteuerung über Joystick oder präzisen Rotary Encoder (im Gyro-Modus).

## Hardware Aufbau
<img width="1393" height="853" alt="Screenshot 2025-11-27 143243" src="https://github.com/user-attachments/assets/cfcd3dfd-9b2b-49cc-9541-1cf90122bba5" />
1. Schaltplan (Sender & Empfänger)

Die Kommunikation erfolgt über nRF24L01+ Module mit PA+LNA für maximale Sendeleistung.

Schematische Darstellung der Verkabelung (Arduino Nano Sender & ESP32 Empfänger).

### Komponenten Liste
Arduino Nano
nRF24L01+ (PA+LNA)
MPU-6050 (Gyroskop)
2x Analog Joysticks
Rotary Encoder
Mode-Button & Status LED
AMS1117 5.0
11Volt 1400mah Akku


## Web Visualisierung
<img width="1902" height="942" alt="Screenshot 2025-11-27 132516" src="https://github.com/user-attachments/assets/7fc34cfe-5e52-4ef2-b448-dfc3772cde15" />
<img width="1907" height="944" alt="Screenshot 2025-11-27 132434" src="https://github.com/user-attachments/assets/0d97022c-b981-4e30-951c-c6a617755996" />
Das System sendet Telemetrie-Daten (JSON) per USB an den PC. Eine begleitende Web-App visualisiert die Fluglage in Echtzeit mit einem 3D-Modell.

Echtzeit-Darstellung von Pitch und Roll im Browser.

Die Tankanzeige und der Modus-Indikator reagieren live auf Eingaben.

## Bedienung


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
