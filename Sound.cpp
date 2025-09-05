#include "Sound.h"

Sound::Sound() 
    : attack(0.1f), decay(0.1f), sustain(0.5f), release(0.5f), cutoff(2000.0f), resonance(1.0f), lfoIntensity(0.5f), lfoOn(true), sampleRate(0.0f) 
{}

Sound::~Sound() {}

void Sound::Init(float sample_rate) {
    sampleRate = sample_rate;
}

void Sound::SetAttack(float value) {
    attack = value;
}

void Sound::SetDecay(float value) {
    decay = value;
}

void Sound::SetSustain(float value) {
    sustain = value;
}

void Sound::SetRelease(float value) {
    release = value;
}

void Sound::SetCutoff(float value) {
    cutoff = value;
}

void Sound::SetResonance(float value) {
    resonance = value;
}

void Sound::SetLfoIntensity(float value) {
    lfoIntensity = value;
}

void Sound::SetLfoOn(bool value) {
    lfoOn = value;
}
