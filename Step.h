#ifndef STEP_H_
#define STEP_H_

#include "note.h"

#define BASE_FREQ _C4

class Step {
   public:
    byte note = BASE_FREQ;
    byte velocity = 127;
    bool slide = false;
    int freqDiff = 0;

    Step() {}

    Step(byte _note, bool _slide) {
        note = _note;
        slide = _slide;
    }

    void set(Step* step) {
        set(step->note, step->velocity, step->slide);
    }

    void set(byte _note) {
        note = _note;
        freqDiff = NOTE_FREQ[BASE_FREQ] - NOTE_FREQ[note];
    }

    void set(byte _note, byte _velocity) {
        velocity = _velocity;
        set(_note);
    }

    void set(byte _note, bool _slide) {
        slide = _slide;
        set(_note);
    }

    void set(byte _note, byte _velocity, bool _slide) {
        slide = _slide;
        set(_note, _velocity);
    }
};

#endif
