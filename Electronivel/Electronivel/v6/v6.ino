const int trig1 = A0;
const int echo1 = A1;
const int trig2 = A2;
const int echo2 = A3;
const int bomba = 12;
const int botonManual = 2;
const int botonAutomatico = 3;

long pingTravelTime1;
long pingTravelTime2;
float distance1;
float distance2;
bool bombaActivadaAutomatico = false;
bool bombaActivadaManual = false;
int previousDistance1 = 0;  // Variable para almacenar el valor anterior de distance1
bool bombaActivadaSecuencial = false;  // Variable para controlar si distance1 ha completado un ciclo

void setup() {
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(bomba, OUTPUT);
  pinMode(botonManual, INPUT_PULLUP);
  pinMode(botonAutomatico, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
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
    if (distance2 >= 5 && distance2 <= 20) {
      if (distance1 >= 20 && !bombaActivadaSecuencial) {
        // Activar la bomba automáticamente cuando la distancia1 sea igual a 30 cm
        digitalWrite(bomba, HIGH);
        Serial.println("Bomba automática activada");
        bombaActivadaSecuencial = true;
      } else if (distance1 <= 10 && bombaActivadaSecuencial) {
        // Desactivar la bomba automáticamente cuando la distancia1 sea igual a 10 cm
        digitalWrite(bomba, LOW);
        Serial.println("Bomba automática desactivada");
        bombaActivadaSecuencial = false;
      }
    } else {
      bombaActivadaSecuencial = false;
    }
  } else {
    bombaActivadaSecuencial = false; // Restablecer el estado si no está en modo automático
  }

  // Lógica para el modo manual
  if (botonManualPresionado) {
    if (distance1 > 10 && distance2 <= 20 && !bombaActivadaManual) {
      // Activar la bomba manualmente mientras la distancia1 sea mayor a 10 cm y distance2 esté en el rango
      digitalWrite(bomba, HIGH);
      Serial.println("Bomba manual activada");
      bombaActivadaManual = true;
    } else if (distance1 <= 10 || distance2 > 20 && bombaActivadaManual) {
      // Desactivar la bomba manualmente cuando la distancia1 sea igual a 10 cm o distance2 esté fuera del rango
      digitalWrite(bomba, LOW);
      Serial.println("Bomba manual desactivada");
      bombaActivadaManual = false;
    } else if (distance1 > 10 && distance2 > 20) {
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

  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print("  Distance2: ");
  Serial.println(distance2);

  delay(1000);
  digitalWrite(trig1, LOW);
  digitalWrite(trig2, LOW);
}

