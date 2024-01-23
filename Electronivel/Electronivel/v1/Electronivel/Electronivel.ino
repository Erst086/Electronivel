const int trigPin = A4;
const int echoPin = A5;
long pingTravelTime;
float distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  pingTravelTime = pulseIn(echoPin, HIGH);
  
  // Convertir tiempo a distancia en centímetros
  distance = (pingTravelTime * 0.0343) / 2.0;

  Serial.println(distance);

  delay(500);  // Puedes ajustar el tiempo de espera según sea necesario
  digitalWrite(trigPin, LOW);
}
