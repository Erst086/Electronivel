const int trig1 = A0;
const int echo1 = A1;
const int trig2 = A4;
const int echo2 = A5;
const int bomba = 12;
const int botonManual = 2;
const int botonAutomatico = 3;
const int TL1 = 11;
const int TL2 = 5;
const int TL3 = 6;
const int TL4 = 7;
const int CL1 = 8;
const int CL2 = 9;
const int CL3 = 10;

long pingTravelTime1;
long pingTravelTime2;
float distance1;
float distance2;
bool bombaActivadaAutomatico = false;
bool bombaActivadaManual = false;
int previousDistance1 = 0;
bool bombaActivadaSecuencial = false;

unsigned long previousMillis = 0;
const long interval = 1000;  // Intervalo de lectura de sensores

void setup() {
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(bomba, OUTPUT);
  pinMode(botonManual, INPUT_PULLUP);
  pinMode(botonAutomatico, INPUT_PULLUP);
  pinMode(TL1, OUTPUT);
  pinMode(TL2, OUTPUT);
  pinMode(TL3, OUTPUT);
  pinMode(TL4, OUTPUT);
  pinMode(CL1, OUTPUT);
  pinMode(CL2, OUTPUT);
  pinMode(CL3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  // Sensor ultrasónico 1
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);

  pingTravelTime1 = pulseIn(echo1, HIGH);
  if (pingTravelTime1 == 0) {
    Serial.println("Error: No se detectó el eco del sensor 1");
    distance1 = 0;
  } else {
    distance1 = (pingTravelTime1 * 0.0343) / 2.0;
  }

  // Sensor ultrasónico 2
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);

  pingTravelTime2 = pulseIn(echo2, HIGH);
  if (pingTravelTime2 == 0) {
    Serial.println("Error: No se detectó el eco del sensor 2");
    distance2 = 0;
  } else {
    distance2 = (pingTravelTime2 * 0.0343) / 2.0;
  }

  // Verificar el estado de los botones
  bool botonManualPresionado = digitalRead(botonManual) == HIGH;
  bool botonAutomaticoPresionado = digitalRead(botonAutomatico) == HIGH;

  // Lógica para el modo automático
  if (botonAutomaticoPresionado) {
      if (distance1 > 40  && distance2 <= 100 && !bombaActivadaSecuencial) {
        // Activar la bomba automáticamente cuando la distancia1 sea mayor a 100
        digitalWrite(bomba, HIGH);
        Serial.println("Bomba automática activada");
        bombaActivadaSecuencial = true;
      } else if (distance1 <= 20 && distance2 >= 100 && bombaActivadaSecuencial) {
        // Desactivar la bomba automáticamente cuando la distancia1 sea menor o igual a 100
        digitalWrite(bomba, LOW);
        Serial.println("Bomba automática desactivada");
        bombaActivadaSecuencial = false;
      } else {
      bombaActivadaSecuencial = false;
    }
  } else {
    bombaActivadaSecuencial = false; // Restablecer el estado si no está en modo automático
  }

  // Lógica para el modo manual
  if (botonManualPresionado) {
    if (distance1 > 20 && distance2 <= 106 && !bombaActivadaManual) {
      // Activar la bomba manualmente mientras la distancia1 sea mayor a 10 cm y distance2 esté en el rango
      digitalWrite(bomba, HIGH);
      Serial.println("Bomba manual activada");
      bombaActivadaManual = true;
    } else if (distance1 <= 20 || distance2 > 106 && bombaActivadaManual) {
      // Desactivar la bomba manualmente cuando la distancia1 sea igual a 10 cm o distance2 esté fuera del rango
      digitalWrite(bomba, LOW);
      Serial.println("Bomba manual desactivada");
      bombaActivadaManual = false;
    } else if (distance1 > 20 && distance2 > 106) {
      // Desactivar la bomba si distance1 es mayor a 10 y distance2 es mayor a 20
      digitalWrite(bomba, LOW);
      Serial.println("Bomba desactivada, cisterna vacía");
      bombaActivadaSecuencial = false;
    }
  } else {
    bombaActivadaManual = false; // Restablecer el estado si no está en modo manual
  }

  // Verificar si ninguno de los modos está activado y apagar la bomba
  if (!bombaActivadaSecuencial && !bombaActivadaManual) {
    digitalWrite(bomba, LOW);
  }

  // Logica de los led TINACO 
  if (distance1 <= 20) {
    digitalWrite(TL1, LOW);
    digitalWrite(TL2, LOW);
    digitalWrite(TL3, LOW);
    digitalWrite(TL4, LOW);
  } else if (distance1 <= 52.5 && distance1 > 20) {
    digitalWrite(TL1, HIGH);
    digitalWrite(TL2, LOW);
    digitalWrite(TL3, LOW);
    digitalWrite(TL4, LOW);
  } else if (distance1 <= 85 && distance1 > 52.5) {
    digitalWrite(TL1, HIGH);
    digitalWrite(TL2, HIGH);
    digitalWrite(TL3, LOW);
    digitalWrite(TL4, LOW);
  } else if (distance1 <= 117.5 && distance1 > 85) {
    digitalWrite(TL1, HIGH);
    digitalWrite(TL2, HIGH);
    digitalWrite(TL3, HIGH);
    digitalWrite(TL4, LOW);
  } else {
    digitalWrite(TL1, HIGH);
    digitalWrite(TL2, HIGH);
    digitalWrite(TL3, HIGH);
    digitalWrite(TL4, HIGH);
  }

  // Logica de los led CISTERNA
  if (distance2 <= 20) {
    digitalWrite(CL1, LOW);
    digitalWrite(CL2, LOW);
    digitalWrite(CL3, LOW);
  } else if (distance2 <= 63.4 && distance2 > 20) {
    digitalWrite(CL1, HIGH);
    digitalWrite(CL2, LOW);
    digitalWrite(CL3, LOW);
  } else if (distance2 <= 106.7 && distance2 > 63.4) {
    digitalWrite(CL1, HIGH);
    digitalWrite(CL2, HIGH);
    digitalWrite(CL3, LOW);
  } else {
    digitalWrite(CL1, HIGH);
    digitalWrite(CL2, HIGH);
    digitalWrite(CL3, HIGH);
  }

  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print("  Distance2: ");
  Serial.println(distance2);
  delay(1500);

  // Actualizar el tiempo para la próxima lectura
  previousMillis = currentMillis;
}