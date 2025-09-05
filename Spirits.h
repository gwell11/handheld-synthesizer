#pragma once
#include "Sound.h"
#include <array>

class Spirits : public Sound {
public:
    static const int NUM_OSCILLATORS = 3;  // Ensure this matches your chords array definition
    static const int NUM_CHORDS = 7;       // Change this from 2 to 7

    Spirits();
    void Init(float sample_rate) override;
    float Process() override;
    void SetChord(int chordIndex, const std::array<float, NUM_OSCILLATORS>& freqs);
    void Trigger(int chordIndex);

    // Add a test function to verify the huge array
    void TestHugeArray();

private:
    std::array<std::array<daisysp::Oscillator, NUM_OSCILLATORS>, NUM_CHORDS> oscs;
    daisysp::AdEnv env;
    bool isPlaying;
    int currentChord;

    // Declare the chords array as static to place it in SDRAM
    static const std::array<float, NUM_OSCILLATORS> chords[NUM_CHORDS] __attribute__((section(".sdram_data")));
};

// Declare a large array for testing SDRAM
extern float hugeArray[] DSY_SDRAM_BSS;
