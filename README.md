# Handheld Chord Synthesizer

A portable, open-source chord synthesizer built on the Electrosmith Daisy Seed platform. It features an interactive TFT display for navigating menus, selecting chords, and shaping sounds in real-time.

![Photo of the synthesizer breadboard](placeholder.jpg)
*(Suggestion: Replace `placeholder.jpg` with a real photo of your breadboarded synth! You can upload images directly to GitHub.)*

## Features

*   **Polyphonic Chord Engine:** Plays multi-voice chords based on the major scale.
*   **Dedicated Chord Buttons:** 7 physical buttons for instantly triggering the chords of the C Major scale (C, Dm, Em, F, G, Am, Bdim).
*   **Interactive UI:** A color TFT screen with a menu system navigated by a rotary encoder and buttons.
*   **Sound Design:** On-device editing of sound parameters like ADSR envelope and filter cutoff.
*   **State Management:** A menu state machine that allows navigation between different settings screens (Main Menu, Chord Selection, Sound Design, etc.).

## Hardware

*   Electrosmith Daisy Seed
*   Color TFT Display (e.g., ST7735 or ILI9341)
*   Rotary Encoder with Push-button
*   10x Momentary Push Buttons
*   Breadboard and jumper wires

## How to Build and Run

1.  **Setup Toolchain:** Ensure you have the [Daisy development toolchain](https://electro-smith.github.io/DaisyWiki/docgod/getting-started-mac.html) set up.
2.  **Clone Repository:** `git clone https://github.com/gwell11/handheld-synthesizer.git`
3.  **Update Makefile:** Open the `Makefile` and verify that the `LIBDAISY_DIR` and `DAISYSP_DIR` variables point to the correct locations on your machine.
4.  **Build:** Run `make` in the project directory.
5.  **Flash:** Put the Daisy Seed in DFU mode and run `make program-dfu`.

## Project Status

This project was recently revived after being abandoned for about a year. The immediate goals are to refactor the code, fix bugs, and implement the remaining features.