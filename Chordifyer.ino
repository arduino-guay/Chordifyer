#include "Metronomo.h"
#include "PianoRoll.h"
#include "Pantalla.h"
#include "MidiControl.h"
#include <MIDI.h>

const uint8_t* gradosAcorde = Chordifyer::Acorde::grados3[0];

void setup () 
{
    //Serial.begin(9600);
    mySerial2Init(); 
    UMIDI.begin(MIDI_CHANNEL_OMNI);
    UMIDI.turnThruOff();
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
    MIDI1.begin(MIDI_CHANNEL_OMNI);
    MIDI1.turnThruOff();

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.clear(); // Set all pixel colors to 'off'
    
    Chordifyer::PianoRoll::init();
    Chordifyer::Metronomo::setBpm(60);
    Chordifyer::Metronomo::init();
    Chordifyer::Metronomo::start();
    Chordifyer::Pantalla::init();
}

void loop()
{
    Chordifyer::Metronomo::update();
    if (MIDI.read())
    {
      Chordifyer::MidiControl::msgMidi(MIDI.getType(), MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
    }  
    if (MIDI1.read())
    {
      Chordifyer::MidiControl::msgMidi(MIDI1.getType(), MIDI1.getData1(), MIDI1.getData2(), MIDI1.getChannel());
    }  
}
