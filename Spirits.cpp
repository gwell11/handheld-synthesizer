#include "Spirits.h"
#include <cmath>

using namespace daisysp;
extern daisy::DaisySeed hw;

// Assuming you have a macro for DSY_SDRAM_BSS
#define DSY_SDRAM_BSS __attribute__((section(".sdram_bss")))

// Define the chords array and place it in a different SDRAM section
const std::array<float, Spirits::NUM_OSCILLATORS> Spirits::chords[NUM_CHORDS] __attribute__((section(".sdram_data"))) = {
    {261.63f, 329.63f, 392.00f},  // C major: C4, E4, G4
    {293.66f, 349.23f, 440.00f},  // D minor: D4, F4, A4
    {329.63f, 392.00f, 493.88f},  // E minor: E4, G4, B4
    {349.23f, 440.00f, 523.25f},  // F major: F4, A4, C5
    {392.00f, 493.88f, 587.33f},  // G major: G4, B4, D5
    {440.00f, 523.25f, 659.25f},  // A minor: A4, C5, E5
    {493.88f, 587.33f, 698.46f}   // B diminished: B4, D5, F5
};

// Define the huge array in SDRAM
float hugeArray[1024 * 1024] DSY_SDRAM_BSS;  // 1 million floats

Spirits::Spirits() : isPlaying(false), currentChord(0) {}

void Spirits::Init(float sample_rate) {
    Sound::Init(sample_rate);  // Call base class Init
    sampleRate = sample_rate;
    for (auto& chord : oscs) {
        for (auto& osc : chord) {
            osc.Init(sampleRate);
            osc.SetWaveform(Oscillator::WAVE_RAMP);
            osc.SetAmp(0.6f / NUM_OSCILLATORS);
        }
    }

    // Set up chords for C major
    for (int i = 0; i < NUM_CHORDS; ++i) {
        SetChord(i, chords[i]);
    }

    env.Init(sampleRate);
    env.SetTime(ADENV_SEG_ATTACK, attack);
    env.SetTime(ADENV_SEG_DECAY, decay);
    env.SetMin(0.0);
    env.SetMax(sustain);
    env.SetCurve(0.0f);  // Linear curve

    // Initialize the huge array with some values
    for (int i = 0; i < 1024 * 1024; ++i) {
        hugeArray[i] = static_cast<float>(i) / (1024 * 1024);
    }
}

float Spirits::Process() {
    if (!isPlaying) return 0.0f;

    float env_out = env.Process();
    if (env_out < 0.001f) {
        isPlaying = false;
        return 0.0f;
    }

    float output = 0.0f;
    for (auto& osc : oscs[currentChord]) {
        output += osc.Process() * env_out;
    }

    // Apply low-pass filter (simple first-order)
    static float lastOutput = 0.0f;
    float alpha = cutoff / (cutoff + sampleRate);
    output = alpha * output + (1.0f - alpha) * lastOutput;
    lastOutput = output;

    return output;
}

void Spirits::SetChord(int chordIndex, const std::array<float, NUM_OSCILLATORS>& freqs) {
    for (size_t i = 0; i < NUM_OSCILLATORS; ++i) {
        oscs[chordIndex][i].SetFreq(freqs[i]);
    }
}

void Spirits::Trigger(int chordIndex) {
    if (chordIndex >= 0 && chordIndex < NUM_CHORDS) {
        env.Trigger();
        isPlaying = true;
        currentChord = chordIndex;
        hw.PrintLine("Spirits chord %d triggered", chordIndex);
    } else {
        hw.PrintLine("Invalid chord index: %d", chordIndex);
    }
}

void Spirits::TestHugeArray() {
    hw.PrintLine("Testing huge array in SDRAM...");
    for (int i = 0; i < 10; ++i) {  // Print first 10 elements as a test
        hw.PrintLine("hugeArray[%d] = %f", i, hugeArray[i]);
    }
}
