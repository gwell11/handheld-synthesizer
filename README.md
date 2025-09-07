# Handheld Chord Synthesizer

A portable, open-source chord synthesizer built on the Electrosmith Daisy Seed platform. It is designed to be a powerful performance instrument focused on the expressive creation of harmonic textures.

![Photo of the synthesizer breadboard](placeholder.jpg)
*(Suggestion: Replace `placeholder.jpg` with a real photo of your breadboarded synth! You can upload images directly to GitHub.)*

## Features

This project aims to implement a professional-grade feature set for deep synthesis and performance.

### Sound Engine (Per Voice)
*   **4-Note Polyphony:** Allows for full chords to ring out.
*   **Triple Oscillator Architecture:** Each voice is comprised of three independent oscillators with selectable waveforms (Sine, Triangle, Saw, Square) and controls for mix levels and detuning.
*   **Noise Generator:** A mixable white noise source for adding texture and percussive elements.

### Sound Shaping (Per Voice)
*   **Multi-Mode Filter:** A resonant filter with Low-Pass, High-Pass, and Band-Pass modes.
*   **Dual ADSR Envelopes:** A dedicated envelope for the amplifier (volume) and a second for modulating the filter cutoff.

### Performance & Sequencing
*   **Expressive Chord Control:** 7 dedicated buttons for triggering diatonic chords, with the ability to cycle through inversions.
*   **Performance Encoder:** A special mode where the encoder can be used to "strum" chords or control the rate of a built-in arpeggiator (Up, Down, Up/Down, Random patterns).
*   **16-Step Chord Sequencer:** An onboard sequencer for building and playing entire chord progressions.
*   **Humanize Function:** An optional feature to add subtle, random variations to note timing and velocity for a more organic, less robotic feel.

### Modulation
*   **Flexible LFO:** One Low-Frequency Oscillator with multiple waveforms that can be routed to modulate Pitch, Filter Cutoff, or Volume.

## Hardware Specification

*   **Processor:** Electrosmith Daisy Seed
*   **Display:** Wide-aspect landscape color TFT screen
*   **Controls:** 1x Rotary Encoder, 9x Momentary Push-Buttons (7 for chords, 2 for navigation)
*   **Connectivity:**
    *   3.5mm Mono Audio Output
    *   Class-Compliant USB MIDI via Micro-USB port
    *   3.5mm "Trio Sync" I/O for tempo-syncing with other gear

## How to Build and Run

1.  **Setup Toolchain:** Ensure you have the [Daisy development toolchain](https://electro-smith.github.io/DaisyWiki/docgod/getting-started-mac.html) set up.
2.  **Clone Repository:** `git clone https://github.com/gwell11/handheld-synthesizer.git`
3.  **Update Makefile:** Open the `Makefile` and verify that the `LIBDAISY_DIR` and `DAISYSP_DIR` variables point to the correct locations on your machine.
4.  **Build:** Run `make` in the project directory.
5.  **Flash:** Put the Daisy Seed in DFU mode and run `make program-dfu`.

## Project Status

This project has been revived with a new, ambitious feature set. The immediate goals are to fix outstanding bugs in the core code, refactor the sound engine to support the new architecture, and then begin implementing the advanced performance and sequencing features.

