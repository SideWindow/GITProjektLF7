# 🚗 Autonomes RC-Car mit Arduino

## 📋 Aufgabenverteilung

| Aufgabe                  | Verantwortlich       | Geschätzte Zeit | Tatsächliche Zeit | Bemerkung          |
|--------------------------|----------------------|------------------|--------------------|---------------------|
| Projektdokumentation     | Güray                | 10 UE            | 10 UE              | ✅ erledigt         |
| Programmierung / Sensorik| Alle                 | 12 UE            | 12 UE              | ✅ erledigt         |
| Technik / Hardwareaufbau | Dejan / Özkan        | 12 UE            | 12 UE              | ✅ erledigt         |
| Balkendiagramm           | Dejan                | 2 UE             | 2 UE               | ✅ erledigt         |
| Präsentation vorbereiten | Güray, Muhammad      | 2 UE             | 2 UE               | ✅ erledigt         |
| Struktogramm             | Nikita               | 1 UE             | 1 UE               | ✅ erledigt         |
| Präsentation             | Özkan                | -                | 15 Min             | ✅ erledigt         |

---

## 🛠️ Projektbeschreibung

Ein motorisiertes RC-Car (Robot-Car) soll über einen Ultraschall-Abstandssensor selbständig fahren und Hindernisse umgehen können.

- Hinderniserkennung < 15 cm
- Automatische Richtungsänderung bei Hindernis
- Startsignal: beide LEDs blinken kurz, dann nur grün aktiv
- Grüne LED: freie Fahrt / Rote LED: Hindernis erkannt
- Piezo warnt akustisch bei Hindernis

---

## 🔩 Benötigte Bauteile

| Bauteil                   | Beschreibung |
|---------------------------|--------------|
| Arduino UNO R3            | Steuerung & Stromversorgung |
| Breadboard                | Verbindung aller Komponenten |
| 2x Widerstände            | Für die LEDs |
| LEDs (Grün/Rot)           | Statusanzeigen |
| DC-Motor + H-Brücke       | Antrieb und Steuerung |
| Ultraschallsensor         | Hinderniserkennung |
| Piezo                     | Warnton |
| 6x 1,5V Batterien         | Externe Stromversorgung |

---

## 🔧 Benötigte Methoden

### Motor-Funktionen
- `geradeAus()`
- `rechtsAbbiegen()`
- `linksAbbiegen()`

### LED-Funktionen
- `ledAlleAn()`
- `ledStatus(boolean hindernis)`

### Ultraschallmessung
- Abstandsmessung in der `loop()` mit `if (distance < 15)`

---

## 💡 Simulationsfunktion

- Start: LEDs blinken, Motoren starten
- Bei Hindernis: Rote LED, Piezo-Ton, Motor-Stopp, Ausweichmanöver
- Ausweichrichtung zufällig (links oder rechts)
- Fortsetzung der Fahrt bei freiem Weg

⚠️ Hinweis: 6x 1,5 V Batterien erforderlich zur Motorversorgung. Eine 9V Blockbatterie ist nicht geeignet.

---

## 🛠️ Aufbau-Anleitung

1. Arduino GND & 5V mit Breadboard verbinden
2. Ultraschallsensor an Pins 6 (ECHO), 7 (TRIG) anschließen
3. L293D Motortreiber auf Breadboard einsetzen
4. Motoranschlüsse auf Pins 2–5 konfigurieren
5. LEDs mit Vorwiderständen auf Pins 9 (grün), 10 (rot)
6. Piezo an Pin 8 anschließen
7. 6x Batterien extern anschließen für Motorstromversorgung
8. DC-Motoren korrekt mit H-Brücke verdrahten

---

## 💻 Beispielcode

```cpp
// --- Motorsteuerung ---
int IN1 = 3, IN2 = 2, ENA = 11;
int IN3 = 4, IN4 = 5, ENB = 12;

// --- Ultraschallsensor ---
int trigPin = 7, echoPin = 6;

// --- Piezo & LEDs ---
int piezo = 8, ledGruen = 9, ledRot = 10;

void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);
  pinMode(piezo, OUTPUT); pinMode(ledGruen, OUTPUT); pinMode(ledRot, OUTPUT);
  ledAlleAn();
  randomSeed(analogRead(0));
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10); digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < 15) {
    ledStatus(true);
    analogWrite(ENA, 0); analogWrite(ENB, 0);
    digitalWrite(piezo, HIGH); delay(2000); digitalWrite(piezo, LOW);
    if (random(2) == 0) linksAbbiegen(); else rechtsAbbiegen();
    delay(3000);
  } else {
    ledStatus(false);
    geradeAus();
  }
  delay(100);
}

// === Methoden ===
void geradeAus() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 255); analogWrite(ENB, 255);
}

void rechtsAbbiegen() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 150); analogWrite(ENB, 255);
}

void linksAbbiegen() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 150); analogWrite(ENB, 255);
}

void ledAlleAn() {
  digitalWrite(ledGruen, HIGH); digitalWrite(ledRot, HIGH);
  delay(1000);
  digitalWrite(ledGruen, LOW); digitalWrite(ledRot, LOW);
}

void ledStatus(boolean hindernis) {
  digitalWrite(ledGruen, !hindernis);
  digitalWrite(ledRot, hindernis);
}