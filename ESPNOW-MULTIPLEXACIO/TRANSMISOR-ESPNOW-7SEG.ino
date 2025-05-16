#include <esp_now.h>     // ✔ Librería para comunicación inalámbrica ESP-NOW
#include <WiFi.h>        // ✔ Necesaria para configurar el WiFi del ESP32

#define BUTTON_SUM 34    // Botón para incrementar el contador
#define BUTTON_RES 32    // Botón para decrementar el contador
#define BUTTON_MID 35    // Botón para reiniciar el contador a 50

// ✔ Dirección MAC del receptor (debe coincidir con el ESP32 receptor)
uint8_t broadcastAddress[] = { 0x08, 0x3A, 0xF2, 0xB4, 0x1C, 0x0C };

// ✔ Estructura que contiene el valor a enviar por ESP-NOW
typedef struct struct_message {
  int valor;
} struct_message;

struct_message myData;               // Instancia del mensaje
esp_now_peer_info_t peerInfo;        // Información del peer (receptor)
int contador = 0;                    // Valor a enviar

// ✔ Estados anteriores de los botones para detección de flancos
bool lastSUM = HIGH;
bool lastRES = HIGH;
bool lastMID = HIGH;

// ✔ Callback que se llama al enviar un mensaje
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Enviado" : "Fallo");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // ✔ Modo estación necesario para ESP-NOW

  // ✔ Configura los botones como entradas con resistencia pull-up
  pinMode(BUTTON_SUM, INPUT_PULLUP);
  pinMode(BUTTON_RES, INPUT_PULLUP);
  pinMode(BUTTON_MID, INPUT_PULLUP);

  // ✔ Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);  // ✔ Callback para monitorear envío

  // ✔ Configuración del peer receptor
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error al añadir peer");
    return;
  }

  // ✔ Envía el valor inicial al receptor
  myData.valor = contador;
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
}

void loop() {
  // ✔ Lee el estado actual de los botones
  bool currentSUM = digitalRead(BUTTON_SUM);
  bool currentRES = digitalRead(BUTTON_RES);
  bool currentMID = digitalRead(BUTTON_MID);
  bool botonPresionado = false;

  // ✔ Detecta flanco de bajada (botón presionado) para sumar
  if (lastSUM == HIGH && currentSUM == LOW) {
    contador = min(99, contador + 1);
    botonPresionado = true;
    Serial.println("Botón SUMA");
  }

  // ✔ Para restar
  if (lastRES == HIGH && currentRES == LOW) {
    contador = max(0, contador - 1);
    botonPresionado = true;
    Serial.println("Botón RESTA");
  }

  // ✔ Para poner el valor en 50
  if (lastMID == HIGH && currentMID == LOW) {
    contador = 50;
    botonPresionado = true;
    Serial.println("Botón MITAD");
  }

  // ✔ Si se presionó algún botón, se envía el nuevo valor
  if (botonPresionado) {
    myData.valor = contador;
    esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  }

  // ✔ Guarda el estado actual para la próxima iteración
  lastSUM = currentSUM;
  lastRES = currentRES;
  lastMID = currentMID;

  delay(50);  // ✔ Delay antirrebote para evitar múltiples lecturas por pulsación
}
