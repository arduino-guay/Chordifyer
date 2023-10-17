#ifndef __PIANOROLL_H__
#define __PIANOROLL_H__

#include <Arduino.h>
#include "Chmidi.h"
#include "Escala.h"
#include "Pantalla.h"

namespace Chordifyer
{

    namespace PianoRoll
    {
        #define MAXTICKS 16
        #define MAXNOTAS 6
        #define ST_OCTAVA 12

        #define RITM_TICK 0
        #define RITM_NOTA 1
        #define RITM_DURACION 2

        uint8_t notaOn[MAXTICKS][MAXNOTAS];  // Posiciones en el Piano-Roll  de un compas ticks x notas  (16x6)
        uint8_t notaOff[MAXTICKS][MAXNOTAS]; // Posiciones en el Piano-Roll  de un compas ticks x notas  (16x6)
                                             // las notas off del último beat se ponen en el beat 0 de notaOFF
        uint8_t notasTickOn[MAXTICKS];  // contador de notasOn en ese tick
        uint8_t notasTickOff[MAXTICKS]; // contador de notasOff en ese tick

        Chordifyer::Escala::Modo modo =  Chordifyer::Escala::menor;
        const uint8_t*  progresion = Chordifyer::Acorde::progresiones[0];
        const uint8_t   (*ritmo)[3]  = Chordifyer::Acorde::ritmo_Spicy;
        uint8_t notaBase = 48;  // C4
        uint8_t notas[MAXNOTAS];
        uint8_t volumen = 64;

        uint8_t compas = 0;
        uint8_t beat   = 0; // dentro del compás
        uint8_t tick   = 0; // dentro del beat
        uint8_t beatXcompas = 4;
        uint8_t tickXbeat = 4;

        void init(uint8_t _beatXcompas = 4, uint8_t _tickXbeat = 4)
        {
            beatXcompas = _beatXcompas;
            tickXbeat = _tickXbeat;
            Chordifyer::Pantalla::setProgresion(progresion);
            Chordifyer::Pantalla::setModo(Chordifyer::Escala::modoStr[modo]);
            Chordifyer::Pantalla::notaBase = notaBase;
        }

        uint16_t getMsTick(uint8_t bpm) 
        {
            return 60000 / bpm / tickXbeat ;

        }

        void setVolumen(uint8_t _volumen)
        {
            volumen = _volumen;
        }

        // Apagamos todas las notas
        void stop()
        {
            for (uint8_t j = 0; j < MAXTICKS; j++)
            {
                for (uint8_t i = 0; i < MAXNOTAS; i++)
                {
                    Chordifyer::Midi::notaOff(notaOff[j][i]);
                }
            }
        }

        void clearPiano()
        {
            for (uint8_t i = 0; i < MAXTICKS; i++)
            {
                for (uint8_t j = 0; j < MAXNOTAS; j++)
                {
                    notaOn[i][j] = 0;
                    notaOff[i][j] = 0;
                }
            }
        }

        void setProgresion(uint8_t prog) 
        {
            progresion = Chordifyer::Acorde::progresiones[prog];
            Chordifyer::Pantalla::setProgresion(progresion);
            stop();
            clearPiano();
        }   

        void setRitmo(const uint8_t (*_ritmo)[3])
        {
            ritmo = _ritmo;
            stop();
            clearPiano();
        }

        void anteriorModo()
        {
            modo =  Chordifyer::Escala::getAnteriorModo(modo); 
            Chordifyer::Pantalla::setModo(Chordifyer::Escala::modoStr[modo]);
            Chordifyer::Pantalla::update();
        }
        
        void siguienteModo()
        {
            modo =  Chordifyer::Escala::getSiguienteModo(modo); 
            Chordifyer::Pantalla::setModo(Chordifyer::Escala::modoStr[modo]);
            Chordifyer::Pantalla::update();
        }

        void setEscala(Chordifyer::Escala::Modo _modo) 
        {
            modo = _modo;
        }

        void setNotaBase(uint8_t _nota) 
        {
            notaBase = _nota; 
            Chordifyer::Pantalla::notaBase = notaBase;
            Chordifyer::Pantalla::update();
        }

        bool montaCompas(uint8_t tipoAcorde, short desplazamiento = 0)
        {
            uint8_t idxRitmo = 0;
            uint8_t duracion;
            uint8_t posTick;

            tick = 0;
            beat = 0;
              
            Chordifyer::Pantalla::update();
            
            //Serial.printf("Acorde %d\n", tipoAcorde);

            const uint8_t* gradosNotas = Chordifyer::Acorde::getGradosNotas(tipoAcorde);
            for (uint8_t i = 0; i < MAXNOTAS; i++)
            {
                notas[i] = 0;
            }

            // Borramos el piano
            clearPiano();
            
            for (uint8_t i = 0; i < MAXTICKS; i++)
            {
                notasTickOn[i] = 0;
                notasTickOff[i] = 0;
            }

            // Montamos las notas segun grado de acorde y escala
            // El primer elemeto es la longitud del array
            for (uint8_t i = 1; i <= gradosNotas[0]; i++)
            {
                // Filtramos el tipoacorde con 0x0F para eliminar la parte de las inversiones y 7ªS
                notas[i-1] = notaBase + desplazamiento + Chordifyer::Escala::getIncNota(tipoAcorde & 0x0F, modo, gradosNotas[i]);
            }
            
            // Montamos el pianoRoll
            // duración 1-16 ticks , 0: fin de la secuencia
            while (duracion = ritmo[idxRitmo][RITM_DURACION])
            {
                posTick = ritmo[idxRitmo][RITM_TICK];
                uint8_t idxRitmoNota = ritmo[idxRitmo][RITM_NOTA];
                uint8_t nota;

                // idxRitmoNota  0:fund-1 oct 1:fund 2: 3: 4:7ª 7:fund+1 oct 
                switch (idxRitmoNota)
                {
                case 0 :
                    nota = notas[0] - ST_OCTAVA;
                    break;
                case 7 :
                    nota = notas[0] + ST_OCTAVA;
                    break;
                default:
                    nota = notas[idxRitmoNota-1];
                    break;
                }

                // Comprobamos por si el ritmo incluye la 7ª pero es un acorde de 3ª
                if ( nota > 0 ) {
                    notaOn[posTick][notasTickOn[posTick]] = nota;
                    // Como solo hay un compas, si el final es en el tick MAXTICKS , se pasa al 0 del siguiente
                    uint8_t posOff = (posTick + duracion) % MAXTICKS;
                    notaOff[posOff][notasTickOff[posOff]] = nota;
                    notasTickOn[posTick]++;
                    notasTickOff[posOff]++;
                }
                idxRitmo++;
            }
        }

        void toque()
        {
            uint8_t tipoAcorde = progresion[compas];
            
            //Serial.printf("compas:%d - beat:%d - tick:%d \n", compas, beat, tick);
            for (uint8_t i = 0; i < MAXNOTAS; i++)
            {
                Chordifyer::Midi::notaOff(notaOff[tick][i]);
            }

            if ( tick == 0 ) 
            {
                // Fin de la progresión
                if ( tipoAcorde ==  0 ) {
                    beat = 0;
                    compas = 0;
                    tipoAcorde = progresion[compas];
                }
                montaCompas(tipoAcorde);
            }

            Chordifyer::Midi::pintaTick(beat, compas, progresion);
            
            for (uint8_t i = 0; i < MAXNOTAS; i++)
            {
                Chordifyer::Midi::notaOn(notaOn[tick][i], (tick == 0)? volumen : uint8_t(volumen * 0.8));
            }

            if (++tick == MAXTICKS)
            {
                tick = 0;
            }
            if ((tick % tickXbeat) == 0)
            {
                if (++beat == beatXcompas)
                {
                    beat = 0;
                    compas++;
                }
            }
        }
        
        void toqueManual()
        {
            for (uint8_t i = 0; i < MAXNOTAS; i++)
            {
                Chordifyer::Midi::notaOff(notaOff[tick][i]);
            }
            for (uint8_t i = 0; i < MAXNOTAS; i++)
            {
                Chordifyer::Midi::notaOn(notaOn[tick][i], (tick == 0)? volumen : uint8_t(volumen * 0.8));
            }
    
            if (++tick == MAXTICKS)
            {
                tick = 0;
            }
            if ((tick % tickXbeat) == 0)
            {
                if (++beat == beatXcompas)
                {
                    beat = 0;
                    compas++;
                }
            }
        }
    
    }
}

#endif // __PIANOROLL_H__
