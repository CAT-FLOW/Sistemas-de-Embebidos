#ifndef SEVENSEGMENT_H      // Verifica si SEVENSEGMENT_H no ha sido definido antes
#define SEVENSEGMENT_H      // Lo define para evitar inclusiones múltiples del mismo archivo

#include <Arduino.h>        // Incluye las definiciones de Arduino (como pinMode, digitalWrite, etc.)

// Definición de la clase SevenSegment
class SevenSegment {
  private:
    int *segPins;             // Puntero a un arreglo con los pines conectados a los segmentos (a-g y punto)
    uint8_t LSB_pin, MSB_pin; // Pines que controlan qué display se activa: unidades (LSB) y decenas (MSB)

    // Tabla de codificación para los segmentos: 17 dígitos/caracteres, cada uno con 8 bits (a, b, c, d, e, f, g, punto)
    uint8_t segCode[17][8] = {
      //  a b c d e f g .   ← segmentos
      {0,0,0,0,0,0,1,1}, // 0
      {1,0,0,1,1,1,1,1}, // 1
      {0,0,1,0,0,1,0,1}, // 2
      {0,0,0,0,1,1,0,1}, // 3
      {1,0,0,1,1,0,0,1}, // 4
      {0,1,0,0,1,0,0,1}, // 5
      {0,1,0,0,0,0,0,1}, // 6
      {0,0,0,1,1,1,1,1}, // 7
      {0,0,0,0,0,0,0,1}, // 8
      {0,0,0,0,1,0,0,1}, // 9
      {0,0,0,1,0,0,0,1}, // A
      {1,1,0,0,0,0,0,1}, // b
      {0,1,1,0,0,0,1,1}, // C
      {1,0,0,0,0,1,0,1}, // d
      {0,1,1,0,0,0,0,1}, // E
      {0,1,1,1,0,0,0,1}, // F
      {1,1,1,1,1,1,1,1}  // Apagar todos (blanco)
    };

  public:
    // Constructor: recibe el arreglo de pines de segmentos, y los pines para LSB y MSB
    SevenSegment(int *segmentPins, uint8_t LSB, uint8_t MSB);

    // Muestra un solo dígito en el display activando los segmentos necesarios
    void displayDigit(uint8_t digit);

    // Muestra un número de 2 dígitos alternando entre unidades y decenas
    void displayNumber(uint8_t units, uint8_t tens, uint8_t delayTime);
};

#endif  // Fin de la protección contra inclusión múltiple
