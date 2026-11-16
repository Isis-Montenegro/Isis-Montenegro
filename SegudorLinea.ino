// --- Pines de control del Driver L298N ---
const int ENA = 10; // Velocidad del motor izquierdo (Pin PWM)
const int IN1 = 9;  // Dirección motor izquierdo (Avanzar)
const int IN2 = 8;  // Dirección motor izquierdo (Retroceder)
const int IN3 = 7;  // Dirección motor derecho (Avanzar)
const int IN4 = 6;  // Dirección motor derecho (Retroceder)
const int ENB = 5;  // Velocidad del motor derecho (Pin PWM)

// --- Pines de los Sensores Infrarrojos ---
const int sensorIzq = 2; // Sensor izquierdo
const int sensorDer = 3; // Sensor derecho

// --- Configuración de Velocidad ---
// Ajusta este valor (0 a 255) según la potencia de tus motores y batería.
int velocidad = 120; 

void setup() {
  // Configuración de pines de motores como salida
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Configuración de pines de sensores como entrada
  pinMode(sensorIzq, INPUT);
  pinMode(sensorDer, INPUT);
}

void loop() {
  // Leer el estado de los sensores (HIGH = Línea Negra, LOW = Fondo Blanco)
  int estadoIzq = digitalRead(sensorIzq);
  int estadoDer = digitalRead(sensorDer);

  if (estadoIzq == LOW && estadoDer == LOW) {
    // Ambos sensores en blanco: La línea está en el centro.
    avanzar();
  }
  else if (estadoIzq == HIGH && estadoDer == LOW) {
    // El sensor izquierdo tocó la línea negra: El robot se desvió a la derecha.
    girarIzquierda();
  }
  else if (estadoIzq == LOW && estadoDer == HIGH) {
    // El sensor derecho tocó la línea negra: El robot se desvió a la izquierda.
    girarDerecha();
  }
  else if (estadoIzq == HIGH && estadoDer == HIGH) {
    // Ambos sensores en línea negra: Intersección o final de la pista.
    detener();
  }
}

// --- Funciones de Movimiento ---

void avanzar() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void girarIzquierda() {
  // El motor derecho avanza y el izquierdo retrocede para un giro rápido
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void girarDerecha() {
  // El motor izquierdo avanza y el derecho retrocede
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}