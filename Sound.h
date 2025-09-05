#pragma once
#include "daisy_seed.h"
#include "daisysp.h"

class Sound {
protected:
    float attack;
    float decay;
    float sustain;
    float release;
    float cutoff;
    float resonance;
    float lfoIntensity;
    bool lfoOn;
    float sampleRate;

public:
    Sound();
    virtual ~Sound();

    virtual void Init(float sample_rate);
    virtual float Process() = 0;

    // Getter and setter methods
    float GetAttack() const { return attack; }
    void SetAttack(float value);
    float GetDecay() const { return decay; }
    void SetDecay(float value);
    float GetSustain() const { return sustain; }
    void SetSustain(float value);
    float GetRelease() const { return release; }
    void SetRelease(float value);
    float GetCutoff() const { return cutoff; }
    void SetCutoff(float value);
    
    float GetResonance() const { return resonance; }
    void SetResonance(float value);

    float GetLfoIntensity() const { return lfoIntensity; }
    void SetLfoIntensity(float value);
    bool IsLfoOn() const { return lfoOn; }
    void SetLfoOn(bool value);
};
