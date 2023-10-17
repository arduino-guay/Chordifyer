#ifndef __PANTALLA_H__
#define __PANTALLA_H__

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace Chordifyer
{
    namespace Pantalla
    {

        #define SCREEN_WIDTH 128 // OLED display width, in pixels
        #define SCREEN_HEIGHT 64 // OLED display height, in pixels
    
        #define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
        #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

        const char*  romanos[7] = {
            "I", "II", "III", "IV", "V", "VI",  "VII"
        };   

        Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
        const uint8_t* progresion;
        const char* modo;
        uint8_t bpm;
        uint8_t notaBase;

        void init()
        {
            // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
            if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
            {
                return;
            }

            // Clear the buffer
            display.clearDisplay();
            display.setTextSize(1);              // Normal 1:1 pixel scale
            display.setTextColor(SSD1306_WHITE); // Draw white text
            display.setCursor(0, 0);             // Start at top-left corner
            display.display();
        }


        void setProgresion(const uint8_t* _progresion) 
        {
            progresion = _progresion;
        }

        void setModo(const char* _modo) 
        {
            modo = _modo;
        }

        void update()
        {
            display.clearDisplay();

            // Progresion
            uint8_t i = 0;
            uint8_t tipoAcorde;
            display.setTextSize(1);              
            display.setCursor(0, 0); 
            while (  tipoAcorde = progresion[i]) 
            {
                if ( i > 0 )
                {
                    display.print("-");
                }
                display.print(romanos[(tipoAcorde & 0x0F)-1]);
                i++;
            }
            
            // BPM
            display.setTextSize(2);              
            display.setCursor(30, 20); 
            display.print(bpm);
            display.print(" BPM");

            // Modo            
            display.setTextSize(1);              
            display.setCursor(0, 40); 
            display.print(modo);

            // Nota Base
            display.setTextSize(1);              
            display.setCursor(40, 40); 
            display.print(Chordifyer::Escala::getNotaStr(notaBase));
            display.print(notaBase/12 - 2);

            display.display();
        }

        void setBpm (uint8_t _bpm)
        {
            bpm = _bpm;
        }
        
        
    }
}

#endif // __PANTALLA_H__
