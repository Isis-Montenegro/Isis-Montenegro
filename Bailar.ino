#include <Servo.h>

Servo miServo;

// Pines del L298N
const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 9;
const int IN4 = 10;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  miServo.attach(3);
  miServo.write(90); // Mirar al frente
  
  delay(3000); 
}

void loop() {
  cabeceoDoble();
  delay(300);
  
  pasoAdelanteAtras();
  
  girosLocos();
  
  detenerse();
  cabeceoDoble();
  
  pasoZigZag();
  
  detenerse();
  miServo.write(90);
  delay(1000); 
}

// --- PASOS DE BAILE: ESTILO "POP & LOCK" (Para tarima de 55cm) ---

void pasoAdelanteAtras() {
  // Movimientos muy explosivos pero con "freno seco" para no patinar.
  for(int i = 0; i < 4; i++) {
    avanzar();
    delay(100);  // Tironcito hacia adelante
    detenerse();
    delay(50);   // Freno seco (bloqueo robótico)
    
    retroceder();
    delay(100);  // Tironcito hacia atrás
    detenerse();
    delay(50);   // Freno seco
  }
}

void girosLocos() {
  // Los giros ahora tienen pausas para que parezcan "cortes" musicales.
  // Evita que la fuerza centrífuga desplace al robot.
  girarDerecha();
  delay(200);
  detenerse();
  delay(100);
  
  girarIzquierda();
  delay(200);
  detenerse();
  delay(100);
  
  girarDerecha();
  delay(450); // Un giro de 360 más controlado
  detenerse();
}

void pasoZigZag() {
  // Micro-ZigZag. Reducimos el voltaje máximo (de 255 a 220) solo 
  // para este paso, así evitamos que se escape por los bordes.
  
  // Hacia adelante
  for(int i = 0; i < 2; i++) {
    analogWrite(IN1, 100); digitalWrite(IN2, LOW);
    analogWrite(IN3, 220); digitalWrite(IN4, LOW);
    delay(120);
    detenerse(); delay(40); // Freno
    
    analogWrite(IN1, 200); digitalWrite(IN2, LOW);
    analogWrite(IN3, 100); digitalWrite(IN4, LOW);
    delay(120);
    detenerse(); delay(40); // Freno
  }
  
  // Hacia atrás (regresa exactamente al centro de la tarima)
  for(int i = 0; i < 2; i++) {
    digitalWrite(IN1, LOW); analogWrite(IN2, 100);
    digitalWrite(IN3, LOW); analogWrite(IN4, 220);
    delay(120);
    detenerse(); delay(40); // Freno
    
    digitalWrite(IN1, LOW); analogWrite(IN2, 200);
    digitalWrite(IN3, LOW); analogWrite(IN4, 100);
    delay(120);
    detenerse(); delay(40); // Freno
  }
}

void cabeceoDoble() {
  // Igual de rápido, sin ocupar espacio
  miServo.write(60); 
  delay(150);
  miServo.write(120);
  delay(150);
  miServo.write(60);
  delay(150);
  miServo.write(120);
  delay(150);
  miServo.write(90); 
}

// --- MOVIMIENTOS BASE ---

void avanzar() {
  analogWrite(IN1, 200); digitalWrite(IN2, LOW);
  analogWrite(IN3, 255); digitalWrite(IN4, LOW);
}

void retroceder() {
  digitalWrite(IN1, LOW); analogWrite(IN2, 200);
  digitalWrite(IN3, LOW); analogWrite(IN4, 255);
}

void detenerse() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void girarDerecha() {
  analogWrite(IN1, 200); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); analogWrite(IN4, 255);
}

void girarIzquierda() {
  digitalWrite(IN1, LOW); analogWrite(IN2, 200);
  analogWrite(IN3, 255); digitalWrite(IN4, LOW);
}