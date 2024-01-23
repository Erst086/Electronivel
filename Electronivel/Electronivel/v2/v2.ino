const int trigPin = A0;
const int echoPin = A1;
const int bombaPin = 0; // Puerto de salida para la bomba
long pingTravelTime;
float distance;
bool bombaActivada = true;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(bombaPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  pingTravelTime = pulseIn(echoPin, HIGH);
  
  // Convertir tiempo a distancia en centímetros
  distance = (pingTravelTime * 0.0343) / 2.0;

  // Verificar las condiciones para activar/desactivar la bomba
  if (distance >= 160 && !bombaActivada) {
    // Activar la bomba
    digitalWrite(bombaPin, LOW);
    Serial.println("Bomba activada");
    bombaActivada = true;
  } else if (distance <= 20 && bombaActivada) {
    // Desactivar la bomba
    digitalWrite(bombaPin, HIGH);
    Serial.println("Bomba desactivada");
    bombaActivada = false;
  }

  Serial.println(distance);

  delay(300);  // Puedes ajustar el tiempo de espera según sea necesario
  digitalWrite(trigPin, LOW);
}

