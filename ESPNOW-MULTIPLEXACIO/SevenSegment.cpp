#include "SevenSegment.h"  // Incluye la cabecera del archivo con la definición de la clase SevenSegment

// Constructor de la clase SevenSegment
SevenSegment::SevenSegment(int *segmentPins, uint8_t LSB, uint8_t MSB) {
  segPins = segmentPins;   // Guarda el arreglo de pines de segmentos (a, b, c, d, e, f, g, punto decimal)
  LSB_pin = LSB;           // Guarda el pin que activa el display de las unidades (Less Significant Bit)
  MSB_pin = MSB;           // Guarda el pin que activa el display de las decenas (Most Significant Bit)

  // Configura los 8 pines de segmentos como salidas y los apaga (HIGH en común ánodo)
  for (int i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);         // Configura cada pin de segmento como salida
    digitalWrite(segPins[i], HIGH);      // Apaga el segmento (HIGH en display de ánodo común)
  }

  // Configura los pines de control de los displays (unidad y decena) como salidas
  pinMode(LSB_pin, OUTPUT);
  pinMode(MSB_pin, OUTPUT);
}

// Método que muestra un solo dígito (0–9) o símbolo en el display activando los segmentos correspondientes
void SevenSegment::displayDigit(uint8_t digit) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segPins[i], segCode[digit][i]);  // Activa o desactiva cada segmento según el código del dígito
  }
}

// Método que muestra un número de dos dígitos (decenas y unidades) usando multiplexación
void SevenSegment::displayNumber(uint8_t units, uint8_t tens, uint8_t delayTime) {
  displayDigit(16);                      // Apaga todos los segmentos antes de actualizar
  digitalWrite(LSB_pin, HIGH);          // Enciende el display de unidades
  digitalWrite(MSB_pin, LOW);           // Apaga el display de decenas
  displayDigit(units);                  // Muestra el dígito de las unidades
  delay(delayTime);                     // Espera para que el ojo humano lo perciba

  displayDigit(16);                     // Apaga todos los segmentos
  digitalWrite(LSB_pin, LOW);           // Apaga el display de unidades
  digitalWrite(MSB_pin, HIGH);          // Enciende el display de decenas
  displayDigit(tens);                   // Muestra el dígito de las decenas
  delay(delayTime);                     // Espera para que el ojo humano lo perciba
}
