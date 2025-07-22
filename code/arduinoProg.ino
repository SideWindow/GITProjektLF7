// --- Motorsteuerung ---
int IN1 = 3;    // Motor Links Richtung 1
int IN2 = 2;    // Motor Links Richtung 2
int ENA = 11;   // Motor Links Geschwindigkeit (PWM)
 
int IN3 = 4;    // Motor Rechts Richtung 1
int IN4 = 5;    // Motor Rechts Richtung 2
int ENB = 12;   // Motor Rechts Geschwindigkeit (PWM)
 
// --- Ultraschallsensor ---
int trigPin = 7;
int echoPin = 6;
 
// --- Piezo ---
int piezo = 8;
 
// --- LEDs ---
int ledGruen = 9;
int ledRot = 10;
 
void setup() {
  // Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
 
  // Ultraschall
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  // Piezo
  pinMode(piezo, OUTPUT);
 
  // LEDs
  pinMode(ledGruen, OUTPUT);
  pinMode(ledRot, OUTPUT);
 
  // Startsignal: beide LEDs für 1 Sekunde
  ledAlleAn();
 
  // Zufallszahlengenerator initialisieren
  randomSeed(analogRead(0));
}
 
void loop() {
  long duration, distance;
 
  // Entfernung messen
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
 
  if (distance < 15) {
    // Hindernis: Rotlicht, piepsen, warten, ausweichen
    ledStatus(true); // rot
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(piezo, HIGH);
    delay(2000);
    digitalWrite(piezo, LOW);
 
    // Zufällig links oder rechts ausweichen
    if (random(2) == 0) {
      // Links abbiegen: rechter Motor schnell, linker langsam
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, 150);
      analogWrite(ENB, 255);
    } else {
      // Rechts abbiegen: linker Motor schnell, rechter langsam
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENA, 150);
      analogWrite(ENB, 255);
    }
 
    delay(3000); // Ausweichdauer
 
  } else {
    // Geradeausfahren
    ledStatus(false); // grün
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 255);
    analogWrite(ENB, 255);
  }
 
  delay(100); // kleine Pause zur Sensorstabilität
}
 
// === LED-Funktionen ===
 
// Startsignal: beide LEDs an für 1 Sekunde
void ledAlleAn() {
  digitalWrite(ledGruen, HIGH);
  digitalWrite(ledRot, HIGH);
  delay(1000);
  digitalWrite(ledGruen, LOW);
  digitalWrite(ledRot, LOW);
}
 
// LED je nach Zustand setzen
void ledStatus(boolean hindernis) {
  if (hindernis) {
    digitalWrite(ledGruen, LOW);
    digitalWrite(ledRot, HIGH);
  } else {
    digitalWrite(ledGruen, HIGH);
    digitalWrite(ledRot, LOW);
  }
}