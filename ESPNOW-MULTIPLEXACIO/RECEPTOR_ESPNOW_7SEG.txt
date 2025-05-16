#include <esp_now.h>           // ✔ Librería de comunicación inalámbrica ESP-NOW
#include <WiFi.h>              // ✔ Necesaria para configurar el modo WiFi del ESP32
#include <SevenSegment.h>      // ✔ Nuestra clase personalizada para manejar el display de 7 segmentos

// ✔ Pines conectados a los segmentos del display (a, b, c, d, e, f, g, punto)
#define SA 5
#define SB 4
#define SC 19
#define SD 25
#define SE 26
#define SF 33
#define SG 32
#define SP 14

// ✔ Pines que controlan cada dígito del display (unidades y decenas)
#define LSB 13  // LSB → dígito de menor peso (unidades)
#define MSB 12  // MSB → dígito de mayor peso (decenas)
#define WAIT 2  // ✔ Tiempo de espera para multiplexado (en ms)

// ✔ Arreglo con los pines de los segmentos
int segPins[] = {SA, SB, SC, SD, SE, SF, SG, SP};

// ✔ Se crea el objeto 'display' usando los pines y el constructor de la clase
SevenSegment display(segPins, LSB, MSB);


// ✔ Estructura para definir el mensaje que se recibirá por ESP-NOW
typedef struct struct_message {
  int valor;                  // El número que se enviará (ej: 0 a 99)
} struct_message;

// ✔ Variable global que almacena el último valor recibido
volatile int valorRecibido = 0;

// ✔ Función que se ejecuta automáticamente al recibir datos por ESP-NOW
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  struct_message incomingData;
  memcpy(&incomingData, data, sizeof(incomingData)); // Copia los datos recibidos a la estructura
  valorRecibido = incomingData.valor;                // Guarda el valor recibido en la variable global
  Serial.print("Dato recibido: ");                   // Imprime por monitor serial para verificar
  Serial.println(valorRecibido);
}

void setup() {
  Serial.begin(115200);        // ✔ Inicializa el puerto serial para depuración
  WiFi.mode(WIFI_STA);         // ✔ Configura el ESP32 en modo estación (necesario para ESP-NOW)

  // ✔ Inicia el protocolo ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW"); // Si falla, lo indica por serial
    return;
  }

  esp_now_register_recv_cb(onDataRecv); // ✔ Registra la función que se ejecuta al recibir datos
}

void loop() {
  // ✔ Se separa el valor recibido en unidades y decenas
  int unidad = valorRecibido % 10;           // Ej: si valor = 57 → unidad = 7
  int decena = (valorRecibido / 10) % 10;    // Ej: si valor = 57 → decena = 5

  // ✔ Multiplexado: refresca la pantalla 50 veces alternando los dígitos
  for (int i = 0; i < 50; i++) {
    display.displayNumber(unidad, decena, WAIT); // Muestra ambos dígitos con un pequeño delay
  }
}
