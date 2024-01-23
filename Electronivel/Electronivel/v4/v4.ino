const int trig = A0;
const int echo = A1;
const int bomba = 0;      // Puerto de salida para la bomba
const int botonManual = 1;   // Pin del botón manual
const int botonAutomatico = 2;  // Pin del botón automático

long pingTravelTime;
float distance;
bool bombaActivadaAutomatico = false;
bool bombaActivadaManual = false;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(bomba, OUTPUT);
  pinMode(botonManual, INPUT_PULLUP);  // Habilitar resistencia pull-up interna para el botón manual
  pinMode(botonAutomatico, INPUT_PULLUP);  // Habilitar resistencia pull-up interna para el botón automático
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  pingTravelTime = pulseIn(echo, HIGH);

  // Convertir tiempo a distancia en centímetros
  distance = (pingTravelTime * 0.0343) / 2.0;

  // Verificar el estado de los botones
  bool botonManualPresionado = digitalRead(botonManual) == HIGH;
  bool botonAutomaticoPresionado = digitalRead(botonAutomatico) == HIGH;

  // Lógica para el modo automático
  if (botonAutomaticoPresionado) {
    if (distance >= 30 && !bombaActivadaAutomatico) {
      // Activar la bomba automáticamente cuando la distancia sea igual a 30 cm
      digitalWrite(bomba, LOW);
      Serial.println("Bomba automática activada");
      bombaActivadaAutomatico = true;
    } else if (distance <= 10 && bombaActivadaAutomatico) {
      // Desactivar la bomba automáticamente cuando la distancia sea igual a 10 cm
      digitalWrite(bomba, HIGH);
      Serial.println("Bomba automática desactivada");
      bombaActivadaAutomatico = false;
    }
  } else {
    bombaActivadaAutomatico = false; // Restablecer el estado si no está en modo automático
  }

  // Lógica para el modo manual
  if (botonManualPresionado) {
    if (distance > 10 && !bombaActivadaManual) {
      // Activar la bomba manualmente mientras la distancia sea mayor a 10 cm
      digitalWrite(bomba, LOW);
      Serial.println("Bomba manual activada");
      bombaActivadaManual = true;
    } else if (distance <= 10 && bombaActivadaManual) {
      // Desactivar la bomba manualmente cuando la distancia sea igual a 10 cm
      digitalWrite(bomba, HIGH);
      Serial.println("Bomba manual desactivada");
      bombaActivadaManual = false;
    }
  } else {
    bombaActivadaManual = false; // Restablecer el estado si no está en modo manual
  }

  // Verificar si ninguno de los modos está activado y apagar la bomba
  if (!bombaActivadaAutomatico && !bombaActivadaManual) {
    digitalWrite(bomba, HIGH);
  }

  Serial.println(distance);

  delay(1000);  // Puedes ajustar el tiempo de espera según sea necesario
  digitalWrite(trig, LOW);
}
