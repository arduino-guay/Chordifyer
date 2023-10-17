#ifndef __MIDICONTROL_H__
#define __MIDICONTROL_H__
#include "Metronomo.h"
#include "PianoRoll.h"
#include <MIDI.h>

namespace Chordifyer
{

    namespace MidiControl
    {
        bool cambioNotaBase = false;
        bool activo = true;
        uint8_t antGrado = 0;
        uint8_t notas[100] ;

        void init() 
        {
            for (uint8_t i = 0; i < 100; i++)
            {
                notas[i] = 0;
            }
                
        }

        void msgControlChange(midi::MidiType cmd, midi::DataByte d1, midi::DataByte d2, midi::Channel ch)
        {
            switch (d1)
            {
            case 0x0C: // BPM
                Chordifyer::Metronomo::setBpm(30 + 5 * map(d2, 0, 127, 0, 20));
                break;
            case 0x18: // Volumen
                Chordifyer::PianoRoll::setVolumen(d2);
                break;
            case 0x66: // Anterior Modo escala
                if (d2 == 127)
                {
                    Chordifyer::PianoRoll::anteriorModo();
                }
                break;
            case 0x67: // Siguiente Modo escala
                if (d2 == 127)
                {
                    Chordifyer::PianoRoll::siguienteModo();
                }
                break;
            case 0x73: // Start
                Chordifyer::PianoRoll::stop();
                Chordifyer::Metronomo::automatico();
                activo = true;
                break;
            case 0x74: // Stop
                Chordifyer::PianoRoll::stop();
                Chordifyer::PianoRoll::clearPiano();
                Chordifyer::Metronomo::manual();
                activo = false;
                break;
            case 0x76: // Cambio de nota base
                cambioNotaBase = true;
                break;
            default:
                UMIDI.send(cmd, d1, d2, ch);
                break;
            }
        }

        void msgProgramChange(midi::MidiType cmd, midi::DataByte d1, midi::DataByte d2, midi::Channel ch)
        {
            switch (d1)
            {
            case 0x00:
                Chordifyer::PianoRoll::setRitmo(Chordifyer::Acorde::ritmo_Legato);
                break;
            case 0x02:
                Chordifyer::PianoRoll::setRitmo(Chordifyer::Acorde::ritmo_Spicy);
                break;
            case 0x04:
                Chordifyer::PianoRoll::setRitmo(Chordifyer::Acorde::ritmo_LegatoNat);
                break;
            case 0x09:
                Chordifyer::PianoRoll::setRitmo(Chordifyer::Acorde::ritmo_Fine);
                break;
            case 0x0C:
                Chordifyer::PianoRoll::setRitmo(Chordifyer::Acorde::ritmo_Energico);
                break;
            case 0x0E:
                Chordifyer::PianoRoll::setRitmo(Chordifyer::Acorde::ritmo_Petite);
                break;
            case 0x11:
                Chordifyer::PianoRoll::setRitmo(Chordifyer::Acorde::ritmo_Rainy);
                break;
            case 0x13:
                Chordifyer::PianoRoll::setRitmo(Chordifyer::Acorde::ritmo_Snobbish);
                break;

            case 0x19:
                Chordifyer::PianoRoll::setProgresion(0);
                break;
            case 0x26:
                Chordifyer::PianoRoll::setProgresion(1);
                break;
            case 0x28:
                Chordifyer::PianoRoll::setProgresion(2);
                break;
            case 0x2E:
                Chordifyer::PianoRoll::setProgresion(3);
                break;
            case 0x3D:
                Chordifyer::PianoRoll::setProgresion(4);
                break;
            case 0x47:
                Chordifyer::PianoRoll::setProgresion(5);
                break;
            case 0x62:
                Chordifyer::PianoRoll::setProgresion(6);
                break;
            default:
                UMIDI.send(cmd, d1, d2, ch);
                break;
            }
        }

        void msgNoteOn(midi::MidiType cmd, midi::DataByte d1, midi::DataByte d2, midi::Channel ch)
        {
            if (cambioNotaBase)
            {
                cambioNotaBase = false;
                Chordifyer::PianoRoll::setNotaBase(d1);
            }
            else
            {
              /*
                switch (d1)
                {
                case 0x31: // Bajamos octava
                    Chordifyer::PianoRoll::notaBase -= 12;
                    break;
                case 0x33: // Subimos Octava
                    Chordifyer::PianoRoll::notaBase += 12;
                    break;
                default:
                    break;
                }
                */
            }
        }

        void msgMidi(midi::MidiType cmd, midi::DataByte d1, midi::DataByte d2, midi::Channel ch)
        {
            if (cmd == midi::MidiType::ControlChange)
            {
                if ( d1 == 0x01 ) 
                {
                  msgControlChange(cmd, 73, d2, ch);
                }
                else 
                {
                  msgControlChange(cmd, d1, d2, ch);
                }
            }
            else if (cmd == midi::MidiType::ProgramChange)
            {
                msgProgramChange(cmd, d1, d2, ch);
            }
            else if (cmd == midi::MidiType::NoteOn)
            {
                notas[d1] = d2;
                msgNoteOn(cmd, d1, d2, ch);
            }
            else if (cmd == midi::MidiType::NoteOff)
            {
                notas[d1] = 0;
            }
            if (!activo && (d1 < 0x3C)) // Modo acorde
            {
                uint8_t grado = 0;
                short desp = 0;
                
                switch (d1)
                {
                case 0x30:
                    grado = 1;
                    break;
                case 0x32:
                    grado = 2;
                    break;
                case 0x34:
                    grado = 3;
                    break;
                case 0x35:
                    grado = 4;
                    break;
                case 0x37:
                    grado = 5;
                    break;
                case 0x39:
                    grado = 6;
                    break;
                case 0x3B:
                    grado = 7;
                    break;
                default:
                    return;    
                }

                if ( notas[0x36] ) 
                {
                    grado += ACOR_INV1;
                } 
                else if ( notas[0x38] ) 
                {
                    grado += ACOR_INV2;
                } 
                else if ( notas[0x3A] ) 
                {
                    grado += ACOR_7;
                }
                
                if ( notas[0x31] ) 
                {
                  desp = -12;
                } 
                else 
                if ( notas[0x33] ) 
                {
                  desp = 12;
                } 
                
                if (cmd == midi::MidiType::NoteOn)
                {
                    Chordifyer::PianoRoll::stop();
                    Chordifyer::PianoRoll::clearPiano();
                    Chordifyer::PianoRoll::montaCompas(grado,desp);
                    Chordifyer::PianoRoll::toqueManual();
                    Chordifyer::Metronomo::start();
                    antGrado = grado;
                }
                else if (cmd == midi::MidiType::NoteOff && (grado == antGrado))
                {
                    Chordifyer::PianoRoll::stop();
                    Chordifyer::PianoRoll::clearPiano();
                }
            }
            else if ((cmd != 0xFE) && (cmd != 0xF0))
            {
                if (cmd == midi::MidiType::NoteOn || cmd == midi::MidiType::NoteOff){
                    d1 = Chordifyer::Escala::getNotaDeEscala(d1, Chordifyer::PianoRoll::notaBase, Chordifyer::PianoRoll::modo);
                }
                UMIDI.send(cmd, d1, d2, ch);
            }
        }
    }
}

#endif // __MIDICONTROL_H__
