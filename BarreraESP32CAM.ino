#include "esp_camera.h"
#include <ESP32Servo.h>

// Pines para AI Thinker ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Pines de tu Hardware
const int pinServo = 14;
const int ledRojo = 12;
const int ledVerde = 13;

Servo miBarrera;

void setup() {
  Serial.begin(115200);
  
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  
  // INICIO MODO PARQUEADERO: Barrera cerrada, luz roja encendida
  digitalWrite(ledRojo, HIGH);
  digitalWrite(ledVerde, LOW);
  
  miBarrera.setPeriodHertz(50); 
  miBarrera.attach(pinServo, 500, 2400);
  miBarrera.write(90); // 90 grados = Barrera horizontal (cerrada)

  // Configuración de la cámara
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  
  config.pixel_format = PIXFORMAT_RGB565; 
  config.frame_size = FRAMESIZE_QQVGA; // 160x120 pixeles
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error al iniciar cámara: 0x%x", err);
    return;
  }
}

void loop() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) return;

  long totalR = 0, totalG = 0, totalB = 0;
  int pixelesContados = 0;

  for (int y = 55; y < 65; y++) {
    for (int x = 75; x < 85; x++) {
      int index = (y * 160 + x) * 2;
      uint8_t alto = fb->buf[index];
      uint8_t bajo = fb->buf[index + 1];
      uint16_t pixel = (alto << 8) | bajo;

      uint8_t r = ((pixel >> 11) & 0x1F) * 8;
      uint8_t g = ((pixel >> 5) & 0x3F) * 4;
      uint8_t b = (pixel & 0x1F) * 8;

      totalR += r;
      totalG += g;
      totalB += b;
      pixelesContados++;
    }
  }

  esp_camera_fb_return(fb);

  int promR = totalR / pixelesContados;
  int promG = totalG / pixelesContados;
  int promB = totalB / pixelesContados;

  // LÓGICA DE PARQUEADERO (Con detección mejorada)
  if (promR > 90 && promR > (promG + 20) && promR > (promB + 20)) { 
    Serial.println("¡TARJETA ROJA DETECTADA! Abriendo barrera...");
    
    digitalWrite(ledRojo, LOW);   // Apaga el rojo
    digitalWrite(ledVerde, HIGH); // Enciende el verde
    miBarrera.write(0);           // Levanta la barrera (0 grados)
    
    delay(2000); // Mantiene la barrera abierta por 2 segundos (ajustado)
    
  } else {
    // Si no hay tarjeta roja, mantiene el parqueadero cerrado
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, HIGH);
    miBarrera.write(90); // Barrera abajo
  }
  
  delay(100); 
}