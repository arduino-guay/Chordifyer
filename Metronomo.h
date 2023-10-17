#ifndef __METRONOMO_H__
#define __METRONOMO_H__

#include <Arduino.h>
#include <timerBruno.h>
#include "PianoRoll.h"
#include "Pantalla.h"

namespace Chordifyer {
    
    namespace Metronomo
    {
        uint8_t bpm;
        uint32_t milisBPM;
        Timer timer;

        void automatico() {
            timer.setCallback(Chordifyer::PianoRoll::toque);
        }
        
        void manual() {
            timer.setCallback(Chordifyer::PianoRoll::toqueManual);
        }

        void init() 
        { 
            automatico();
        };

        void setBpm(uint8_t _bpm)
        {
            bpm = _bpm;
            milisBPM = 60000 / bpm;
            timer.setInterval(Chordifyer::PianoRoll::getMsTick(bpm));
            Chordifyer::Pantalla::setBpm(bpm);
        }

        void start() 
        {
            timer.reset();
            timer.start();
        }

        void stop() 
        {
            timer.stop();
        }

        void update() 
        {
            timer.update();
        }
    };
}

#endif // __METRONOMO_H__
