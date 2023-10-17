#ifndef __ESCALA_H__
#define __ESCALA_H__

#include <Arduino.h>
#include "Acorde.h"

namespace Chordifyer {
    
    namespace Escala
    {

            #define NUMESCALAS 9

            typedef enum Modo {
                mayor,
                dorico,
                frigio,
                lidio,
                mixolidio,
                menor,
                locrio,
                andaluza,
                arabe
            };

            const uint8_t gradosNota[12]  =  {
                1,1,2,2,3,4,4,5,5,6,6,7
            };

            const char* notasStr[12]  =  {
                "A","A#","B","C","C#","D","D#","E","F","F#","G","G#"
            };

            const char* modoStr[NUMESCALAS]  =  {
                "Mayor",
                "Dorico",
                "Frigio",
                "Lidio",
                "Mixolidio",
                "Menor",
                "Locrio",
                "Andaluza",
                "Arabe"
            } ;  

            const uint8_t stEscala[NUMESCALAS][8] = {
                // El primer dígito es la longitud de la escala
                {7,2,2,1,2,2,2,1},
                {7,2,1,2,2,2,1,2},
                {7,1,2,2,2,1,2,2},
                {7,2,2,2,1,2,2,1},
                {7,2,2,1,2,2,1,2},
                {7,2,1,2,2,1,2,2},
                {7,1,2,2,1,2,2,2},
                {7,1,2,2,2,1,2,2},
                {7,1,3,1,2,1,3,1}
            };

            Modo getSiguienteModo( Modo actual ) 
            {
                    switch (actual)
                    {
                        case  mayor:  return dorico;
                        case  dorico: return frigio; 
                        case  frigio: return lidio;
                        case  lidio:  return mixolidio;
                        case  mixolidio: return menor;
                        case  menor: return locrio;
                        case  locrio: return andaluza;
                        case  andaluza: return arabe;
                        case  arabe: return mayor;
                    }
            }


            Modo getAnteriorModo( Modo actual ) 
            {
                    switch (actual)
                    {
                        case  mayor:  return arabe;
                        case  arabe:  return andaluza;
                        case  andaluza:  return locrio;
                        case  dorico: return mayor; 
                        case  frigio: return dorico;
                        case  lidio:  return frigio;
                        case  mixolidio: return lidio;
                        case  menor: return mixolidio;
                        case  locrio: return menor;
                    }
            }

            uint8_t getIncNota(uint8_t gradoAcorde, Modo modo, uint8_t gradoNota) {
                 uint8_t idxNota  = gradoAcorde - 1 + gradoNota - 1;
                 uint8_t val = 0;
                 uint8_t longEscala = stEscala[modo][0];

                 for (uint8_t i = 0; i < idxNota; i++)
                 {
                    val += stEscala[modo][1 + (i % longEscala)] ;
                 }
                 return val;
            }

            uint8_t getNotaDeEscala(uint8_t notaMidi, uint8_t notaBase, Modo modo) {                
                 uint8_t idxOctava = (notaMidi - 0x30) / 12 ;
                 uint8_t gradoNota  = gradosNota[notaMidi % 12];  // 1-7
                 uint8_t val = notaBase + 12 * (idxOctava-1);

                 for (uint8_t i = 1; i < gradoNota; i++)
                 {
                    val += stEscala[modo][i] ;
                 }
                 return val;
            }

            const char* getNotaStr(uint8_t notaMidi) 
            {
                return notasStr[(notaMidi+3) % 12];
            }

    };

}

#endif // __ESCALA_H__
