#ifndef __MIDI_H__
#define __MIDI_H__

#include <Arduino.h>
#include "wiring_private.h"
#include <USB-MIDI.h>
#include <Adafruit_NeoPixel.h>

#define PIN 2
#define NUMPIXELS    60
#define BRIGHTNESS  128
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Instancia Midi en USB
USBMIDI_CREATE_INSTANCE(0, UMIDI);

// Instancia Midi en Serial 6,7
MIDI_CREATE_DEFAULT_INSTANCE();

// Instancia Midi en SERCOM0 9,10
Uart mySerial2 (&sercom0, A9, A10, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void mySerial2Init () {
  mySerial2.begin(9600);
  pinPeripheral(A9, PIO_SERCOM);  //Assign RX function to pin 9
  pinPeripheral(A10, PIO_SERCOM); //Assign TX function to pin 10
}

// Attach the interrupt handler to the SERCOM
void SERCOM0_Handler() {
  mySerial2.IrqHandler();
}

MIDI_CREATE_INSTANCE(HardwareSerial, mySerial2, MIDI1);

uint32_t colores[10] = {
     pixels.Color(10,10,10),    // 0 - Apagado
     pixels.Color(80,30,20),    // 1 - Marrón
     pixels.Color(255,0,0),     // 2 - Rojo
     pixels.Color(175,50,0),    // 3 - Naranja
     pixels.Color(255,215,0),   // 4 - Amarillo
     pixels.Color(0,255,0),     // 5 - Verde
     pixels.Color(0,0,255),     // 6 - Azul
     pixels.Color(63,0,128),    // 7 - Violeta
     pixels.Color(40,165,65),   // 8 - Cian
     pixels.Color(255,255,255)  // 9 - Blanco
};



namespace Chordifyer {
    
    namespace Midi
    {

        #define NOTA_INI 45

        void pintaTick(uint8_t beat, uint8_t compas, const uint8_t* progresion)
        {
          
          // Progresion
          uint8_t idAcorde = 0;
          uint8_t idPixel = 0;
          uint8_t tipoAcorde;
          pixels.clear();
          while (  tipoAcorde = progresion[idAcorde]) 
          {
              
              for (uint8_t i = 0; i < 4; i++)
              {
                  pixels.setPixelColor(idPixel++, colores[tipoAcorde & 0x0F]);
              }
              
              idAcorde++;
          }
          pixels.setPixelColor(compas*4 + beat, pixels.Color(255,255,255));
          pixels.show();   
        }
        
        void pintaNota(uint8_t nota, bool OnOff, uint32_t color)
        {
          if ( nota < 48 ) 
          {
            return;
          }
          if ( OnOff ) {
              pixels.setPixelColor(nota-48, color);
          } else {
              pixels.setPixelColor(nota-48, pixels.Color(0, 0, 0));
          }
          pixels.show();   
        }
        
        void notaOn(uint8_t nota, uint8_t volumen)
        {
            if ( nota > 0 ) 
            {
               //Serial.printf("NotaOn : %d\n",nota);
               UMIDI.sendNoteOn(nota, volumen, 1);
               //pintaNota(nota, true, colores[tipoAcorde]);
            }
        } 

        void notaOff(uint8_t nota)
        {
            if ( nota > 0 ) 
            {
               //Serial.printf("NotaOff: %d\n",nota);
               UMIDI.sendNoteOff(nota, 127, 1);
               //pintaNota(nota, false, 0);
            }
        } 
    }
}


#endif // __MIDI_H__
