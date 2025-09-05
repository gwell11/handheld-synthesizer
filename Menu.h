//Menu.h

#pragma once

#include "daisy_seed.h"
#include "GFX.h"
#include "Vanilla20pt.h"
#include "Pap.h"
#include "Sound.h"
#include "Spirits.h"
#include <stack>
#include <array>

// Enum for menu options
enum MenuOption {
    OPTION_1,
    OPTION_2,
    OPTION_3,
    OPTION_4,
    OPTION_5,
    OPTION_COUNT
};

enum class EditMode {
    Navigation,
    Editing
};


enum class Key {
    C, G, D, A, E, B, F
};

struct ChordInfo {
    const char* label;
    const char* chords[7][3];  // [key][variation]
};
// Enum for menu states
enum MenuState
{
    MAIN_MENU,
    CHORD_SELECTION,
    SOUND_DESIGN,
    PERFORMANCE_SETTINGS,
    GLOBAL_SETTINGS,
    VISUALS
};

#define MAIN_MENU_OPTIONS 4
#define CHORD_SELECTION_OPTIONS 8  // For the 7 chords in C major
#define SOUND_DESIGN_OPTIONS 5     // Oscillator, ADSR, LFO, Effects, Filter
#define PERFORMANCE_SETTINGS_OPTIONS 3  // Example: Arpeggiator, Chord Strumming, Velocity
#define GLOBAL_SETTINGS_OPTIONS 4  // Example: MIDI, Output, Screen, Save/Load

extern int currentOption;
extern MenuState currentState;
extern EditMode currentEditMode;
extern int selectedParameter;
extern std::stack<MenuState> stateStack; // Stack to keep track of previous states
extern int projectBPM; // Declare projectBPM as extern
extern Spirits spirits;
extern Spirits sound2;
extern Spirits option3Sound;
extern int currentlyPlayingChord;

extern Key currentKey;
extern const ChordInfo CHORD_INFO[7];
extern int selectedVariations[7];

bool UpdateMenuOption();
void DisplayMenu();
void HandleBackButton();
void HandleMenuSelection(bool isEncoderPress = false);
void DisplaySoundParameterMenu(Sound* sound, const char* soundName);
void HandleSoundParameterAdjustment(Sound* sound);
void HandleLfoOnOffToggle(Sound* sound);

// New display functions for each menu state
void DisplayMainMenu();
void DisplayChordSelection();
void DisplaySoundDesign();
void DisplayPerformanceSettings();
void DisplayGlobalSettings();
void DisplayVisuals();

// New functions for handling specific menus
void HandleChordSelection();
void HandleSoundDesign();
void HandlePerformanceSettings();
void HandleGlobalSettings();
bool HandleChordButtonPress();

// New function for changing the key
void ChangeKey(Key newKey);

// Initialize menu
void InitMenu();