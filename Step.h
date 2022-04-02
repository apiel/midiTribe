#ifndef STEP_H_
#define STEP_H_

class Step
{
public:
    byte note = 0;
    byte velocity = 127;
    bool slide = false;

    Step() {}

    Step(byte _note, bool _slide)
    {
        note = _note;
        slide = _slide;
    }

    void reset()
    {
        note = 0;
        velocity = 127;
        slide = false;
    }

    void set(Step *step)
    {
        set(step->note, step->velocity, step->slide);
    }

    void set(byte _note)
    {
        note = _note;
    }

    void set(byte _note, byte _velocity)
    {
        velocity = _velocity;
        set(_note);
    }

    void set(byte _note, bool _slide)
    {
        slide = _slide;
        set(_note);
    }

    void set(byte _note, byte _velocity, bool _slide)
    {
        slide = _slide;
        set(_note, _velocity);
    }
};

#endif
