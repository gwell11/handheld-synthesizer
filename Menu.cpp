//Menu.cpp with keyboard chord selection:

#include "Menu.h"
#include "Buttons.h"
#include "Pap.h"
#include "daisy.h" // Include the Daisy library for Encoder
#include <stack>
#include "Spirits.h"
#include "Sound.h"

#define MAIN_MENU_OPTIONS 4
#define CHORD_SELECTION_OPTIONS 8  // For the 7 chords in C major
#define SOUND_DESIGN_OPTIONS 5     // Oscillator, ADSR, LFO, Effects, Filter
#define PERFORMANCE_SETTINGS_OPTIONS 3  // Example: Arpeggiator, Chord Strumming, Velocity
#define GLOBAL_SETTINGS_OPTIONS 4  // Example: MIDI, Output, Screen, Save/Load
#define KEYBOARD_KEYS 7


extern Spirits spirits;
extern Spirits sound2;
extern Spirits option3Sound;
extern Switch buttonKeyboard;

extern cGFX __Display;
extern cFont FreeSans15;
extern cFont FreeSans30B;

extern daisy::Encoder encoder;
extern daisy::DaisySeed hw;

int currentOption = 0;

Key currentKey = Key::C;
MenuState currentState = MAIN_MENU;
EditMode currentEditMode = EditMode::Navigation;
int selectedParameter = 0;
std::stack<MenuState> stateStack; // Stack to keep track of previous states


const ChordInfo CHORD_INFO[7] = {
    {"I",   {{"C", "Cmaj7", "C6"}, {"G", "Gmaj7", "G6"}, {"D", "Dmaj7", "D6"}, {"A", "Amaj7", "A6"}, {"E", "Emaj7", "E6"}, {"B", "Bmaj7", "B6"}, {"F", "Fmaj7", "F6"}}},
    {"ii",  {{"Dm", "Dm7", "Dm6"}, {"Am", "Am7", "Am6"}, {"Em", "Em7", "Em6"}, {"Bm", "Bm7", "Bm6"}, {"F#m", "F#m7", "F#m6"}, {"C#m", "C#m7", "C#m6"}, {"Gm", "Gm7", "Gm6"}}},
    {"iii", {{"Em", "Em7", "Em6"}, {"Bm", "Bm7", "Bm6"}, {"F#m", "F#m7", "F#m6"}, {"C#m", "C#m7", "C#m6"}, {"G#m", "G#m7", "G#m6"}, {"D#m", "D#m7", "D#m6"}, {"Am", "Am7", "Am6"}}},
    {"IV",  {{"F", "Fmaj7", "F6"}, {"C", "Cmaj7", "C6"}, {"G", "Gmaj7", "G6"}, {"D", "Dmaj7", "D6"}, {"A", "Amaj7", "A6"}, {"E", "Emaj7", "E6"}, {"Bb", "Bbmaj7", "Bb6"}}},
    {"V",   {{"G", "G7", "G9"}, {"D", "D7", "D9"}, {"A", "A7", "A9"}, {"E", "E7", "E9"}, {"B", "B7", "B9"}, {"F#", "F#7", "F#9"}, {"C", "C7", "C9"}}},
    {"vi",  {{"Am", "Am7", "Am6"}, {"Em", "Em7", "Em6"}, {"Bm", "Bm7", "Bm6"}, {"F#m", "F#m7", "F#m6"}, {"C#m", "C#m7", "C#m6"}, {"G#m", "G#m7", "G#m6"}, {"Dm", "Dm7", "Dm6"}}},
    {"vii°",{{"Bdim", "Bm7b5", "Bo7"}, {"F#dim", "F#m7b5", "F#o7"}, {"C#dim", "C#m7b5", "C#o7"}, {"G#dim", "G#m7b5", "G#o7"}, {"D#dim", "D#m7b5", "D#o7"}, {"A#dim", "A#m7b5", "A#o7"}, {"Edim", "Em7b5", "Eo7"}}}
};

int selectedVariations[7] = {0, 0, 0, 0, 0, 0, 0};


bool UpdateMenuOption()
{
    encoder.Debounce();
    int optionCount;
    
    switch (currentState)
    {
        case MAIN_MENU:
            optionCount = MAIN_MENU_OPTIONS;
            break;
        case CHORD_SELECTION:
            optionCount = 8;  
            break;
        case SOUND_DESIGN:
            optionCount = SOUND_DESIGN_OPTIONS;
            break;
        case PERFORMANCE_SETTINGS:
            optionCount = PERFORMANCE_SETTINGS_OPTIONS;
            break;
        case GLOBAL_SETTINGS:
            optionCount = GLOBAL_SETTINGS_OPTIONS;
            break;
        case VISUALS:
            optionCount = 1; // Visuals doesn't have selectable options
            break;
        default:
            optionCount = MAIN_MENU_OPTIONS;
            break;
    }

    bool optionChanged = false;
    int increment = -encoder.Increment();

    if (increment != 0)
    {
        hw.PrintLine("Encoder increment: %d", increment);
        currentOption = (currentOption + increment + optionCount) % optionCount;
        if (currentOption < 0) {
            currentOption += optionCount;
        }
        hw.PrintLine("Scrolling to option: %d", currentOption);
        optionChanged = true;
    }

    return optionChanged;
}


void DisplayMenu()
{
    hw.PrintLine("DisplayMenu: currentState = %d, currentOption = %d", currentState, currentOption);
    
    // Clear the screen before displaying the menu
    __Display.drawFillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, cColor(0, 0, 0));

    switch (currentState)
    {
    case MAIN_MENU:
        DisplayMainMenu();
        break;
    case CHORD_SELECTION:
        DisplayChordSelection();
        break;
    case SOUND_DESIGN:
        DisplaySoundDesign();
        break;
    case PERFORMANCE_SETTINGS:
        DisplayPerformanceSettings();
        break;
    case GLOBAL_SETTINGS:
        DisplayGlobalSettings();
        break;
    case VISUALS:
        DisplayVisuals();
        break;
    }

    // Flush the frame buffer to update the display
    __Display.FlushFrame();
}


void DisplayMainMenu()
{
    uint16_t x = 10;
    uint16_t y = 60;
    uint16_t spacing = 30;

    for (int i = 0; i < MAIN_MENU_OPTIONS; ++i)
    {
        __Display.setCursor(x, y + i * spacing);
        __Display.setFont(&FreeSans15);
        __Display.setTextFrontColor(i == currentOption ? cColor(0, 200, 255) : cColor(255, 255, 255));

        switch (i)
        {
        case 0:
            __Display.drawTransText("Chord Selection");
            break;
        case 1:
            __Display.drawTransText("Sound Design");
            break;
        case 2:
            __Display.drawTransText("Performance Settings");
            break;
        case 3:
            __Display.drawTransText("Global Settings");
            break;
        }
    }
    __Display.FlushFrame();
}



void DrawKeyboard()
{
    // Define the width of bottom keys
    uint16_t keyWidthBottom = TFT_WIDTH / 7;

    // Define the width of top keys (slightly skinnier)
    uint16_t keyWidthTop = keyWidthBottom * 1.2; // 90% of bottom key width

    // Define the height of each key
    uint16_t keyHeight = TFT_HEIGHT / 6;

    // Define the x-coordinate for the first column of keys (left side)
    uint16_t x1 = 95;

    // Define the x-coordinate for the second column of keys (right side)
    uint16_t x2 = 103;

    // Define the y-coordinate for the top row of keys
    uint16_t yTop = 84;

    // Define the y-coordinate for the bottom row of keys
    uint16_t yBottom = 122;


    // Define the visual positions of the chords
    const int chordPositions[] = {0, 4, 5, 1, 2, 6, 3}; // I, ii, iii, IV, V, vi, vii°

    // Loop through all 7 keys to draw them
    for (int i = 0; i < 7; ++i)
    {
        // Determine the x-coordinate for the current key
        uint16_t x = (i < 4) ? x1 + (i * (keyWidthBottom + 3)) : x2 + ((i - 4) * (keyWidthBottom + 10));

        // Determine the y-coordinate for the current key
        uint16_t y = (i < 4) ? yBottom : yTop;

        // Determine the width for the current key
        uint16_t width = (i < 4) ? keyWidthBottom : keyWidthTop;

       // Draw the key
        if (i < 4) {
            // Bottom row: filled rectangles
            __Display.drawFillRect(x, y, width, keyHeight, cColor(255, 255, 255));
        } else {
            // Top row: hollow rectangles
            __Display.drawRect(x, y, width, 35, cColor(255, 255, 255));
        }


        // If the key should be highlighted or it's the current option, highlight it
if (currentOption < 7 && chordPositions[currentOption] == i)
        {
            if (i < 4) {
                __Display.drawFillRect(x + 2, y + 2, width - 4, keyHeight - 4, cColor(0, 200, 255));
            } else {
                __Display.drawFillRect(x + 2, y + 2, width - 4, 35 - 4, cColor(0, 200, 255));
            }
        }
    }
    // Display the current chord at the bottom
    const char* currentChord = CHORD_INFO[currentOption < 7 ? currentOption : 0].chords[static_cast<int>(currentKey)][selectedVariations[currentOption < 7 ? currentOption : 0]];
    __Display.setCursor(100, 200);
    __Display.setFont(&FreeSans15);
    __Display.setTextFrontColor(cColor(255, 255, 255));
    char chordDisplay[30];
    snprintf(chordDisplay, sizeof(chordDisplay), "Chord: %s", currentChord);
    __Display.drawTransText(chordDisplay);
}




void DisplayChordSelection()
{
    uint16_t width = TFT_HEIGHT;
    uint16_t height = TFT_WIDTH;

    // Clear the entire screen
    __Display.drawFillRect(0, 0, width, height, cColor(0, 0, 0));

    // Draw the keyboard
    DrawKeyboard();

    // Display current key
    const char* keyNames[] = {"C", "G", "D", "A", "E", "B", "F"};
    char keyDisplay[20];
    snprintf(keyDisplay, sizeof(keyDisplay), "Key: %s", keyNames[static_cast<int>(currentKey)]);
    __Display.setCursor(115, 60);
    __Display.setFont(&FreeSans15);
    __Display.setTextFrontColor(currentOption == 7 ? cColor(0, 200, 255) : cColor(255, 255, 255));
    __Display.drawTransText(keyDisplay);

    __Display.FlushFrame();
}


void InitMenu()
{
    currentOption = 0;
    currentState = MAIN_MENU;
    currentEditMode = EditMode::Navigation;
    selectedParameter = 0;
    while (!stateStack.empty()) stateStack.pop();
    currentKey = Key::C;
    for (int i = 0; i < 7; ++i) {
        selectedVariations[i] = 0;
    }
}


void ChangeKey(Key newKey)
{
    currentKey = newKey;
    DisplayMenu();
}


void DisplaySoundDesign()
{
    uint16_t x = 10;
    uint16_t y = 60;
    uint16_t spacing = 30;
    uint16_t width = TFT_HEIGHT;  // Swapped due to rotation
    uint16_t height = TFT_WIDTH;  // Swapped due to rotation

    // Clear the entire screen
    __Display.drawFillRect(0, 0, width, height, cColor(0, 0, 0));

    const char* menuItems[] = {"Oscillator", "ADSR", "LFO", "Effects", "Visuals"};
    for (int i = 0; i < SOUND_DESIGN_OPTIONS; ++i)
    {
        __Display.setCursor(x, y + i * spacing);
        __Display.setFont(&FreeSans15);
        __Display.setTextFrontColor(i == currentOption ? cColor(0, 200, 255) : cColor(255, 255, 255));
        __Display.drawTransText(menuItems[i]);
    }

    // Flush the frame buffer to update the display
    __Display.FlushFrame();
}


void DisplayPerformanceSettings()
{
    uint16_t x = 30;
    uint16_t y = 60;
    uint16_t spacing = 30;
    uint16_t width = TFT_HEIGHT;  // Swapped due to rotation
    uint16_t height = TFT_WIDTH;  // Swapped due to rotation

    // Clear the entire screen
    __Display.drawFillRect(0, 0, width, height, cColor(0, 0, 0));

    for (int i = 0; i < PERFORMANCE_SETTINGS_OPTIONS; ++i)
    {
        __Display.setCursor(x, y + i * spacing);
        __Display.setFont(&FreeSans15);
        __Display.setTextFrontColor(i == currentOption ? cColor(0, 200, 255) : cColor(255, 255, 255));

        switch (i)
        {
        case 0:
            __Display.drawTransText("Arpeggiator");
            break;
        case 1:
            __Display.drawTransText("Chord Strumming");
            break;
        case 2:
            __Display.drawTransText("Velocity");
            break;
        }
    }
    __Display.FlushFrame();
}


void DisplayGlobalSettings()
{
    uint16_t x = 30;
    uint16_t y = 60;
    uint16_t spacing = 30;
    uint16_t width = TFT_HEIGHT;  // Swapped due to rotation
    uint16_t height = TFT_WIDTH;  // Swapped due to rotation

    // Clear the entire screen
    __Display.drawFillRect(0, 0, width, height, cColor(0, 0, 0));

    for (int i = 0; i < GLOBAL_SETTINGS_OPTIONS; ++i)
    {
        __Display.setCursor(x, y + i * spacing);
        __Display.setFont(&FreeSans15);
        __Display.setTextFrontColor(i == currentOption ? cColor(0, 200, 255) : cColor(255, 255, 255));

        switch (i)
        {
        case 0:
            __Display.drawTransText("MIDI Configuration");
            break;
        case 1:
            __Display.drawTransText("Output Routing");
            break;
        case 2:
            __Display.drawTransText("Screen Brightness");
            break;
        case 3:
            __Display.drawTransText("Save/Load Presets");
            break;
        }
    }
    
}

void DisplayVisuals()
{
    uint16_t width = TFT_HEIGHT;  // Swapped due to rotation
    uint16_t height = TFT_WIDTH;  // Swapped due to rotation

    // Clear the entire screen
    __Display.drawFillRect(0, 0, width, height, cColor(0, 0, 0));
    // This function will be implemented later to show animations
    __Display.setCursor(30, 60);
    __Display.setFont(&FreeSans15);
    __Display.setTextFrontColor(cColor(0, 200, 255));
    __Display.drawTransText("Visuals Mode");
}


void HandleMenuSelection(bool isEncoderPress)
{
    bool selectionMade = isEncoderPress && encoder.RisingEdge();
    bool menuUpdated = false;

    if (selectionMade)
    {
        switch (currentState)
        {
            case MAIN_MENU:
                stateStack.push(currentState);
                switch (currentOption)
                {
                    case 0:
                        currentState = CHORD_SELECTION;
                        break;
                    case 1:
                        currentState = SOUND_DESIGN;
                        break;
                    case 2:
                        currentState = PERFORMANCE_SETTINGS;
                        break;
                    case 3:
                        currentState = GLOBAL_SETTINGS;
                        break;
                }
                currentOption = 0;
                menuUpdated = true;
                break;

            case CHORD_SELECTION:
                if (currentOption < 7) {
                    // Cycle through chord variations
                    selectedVariations[currentOption] = (selectedVariations[currentOption] + 1) % 3;
                } else {
                    // Change key (assuming the key change option is after the 7 chords)
                    currentKey = static_cast<Key>((static_cast<int>(currentKey) + 1) % 7);
                }
                menuUpdated = true;
                break;

            case SOUND_DESIGN:
                // Implement sound design selection logic here
                break;

            case PERFORMANCE_SETTINGS:
                // Implement performance settings selection logic here
                break;

            case GLOBAL_SETTINGS:
                // Implement global settings selection logic here
                break;

            case VISUALS:
                // Implement visuals selection logic here
                break;

            default:
                hw.PrintLine("Unexpected menu state: %d", static_cast<int>(currentState));
                break;
        }
    }

    if (menuUpdated)
    {
        DisplayMenu();
    }
}



void HandleBackButton()
{
    if (buttonBack.RisingEdge()) // Button pressed (inverted polarity)
    {
        if (!stateStack.empty())
        {
            currentState = stateStack.top();
            stateStack.pop();
            currentOption = 0; // Reset to the first option in the previous menu
            __Display.drawFillRect(0, 0, __Display.getWidth(), __Display.getHeight(), cColor(0, 0, 0)); // Fill the entire screen with black color
            DisplayMenu(); // Update the display to show the previous menu
        }
        else
        {
            hw.PrintLine("Cannot go back, at top-level menu");
        }
    }
}


void DisplaySoundParameterMenu(Sound* sound, const char* soundName)
{
    uint16_t x = 10;
    uint16_t y = 20;
    uint16_t spacing = 30;
    uint16_t barWidth = 100;
    uint16_t barHeight = 15;
    uint16_t barOffset = 130;

    // Clear the screen
    __Display.drawFillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, cColor(0, 0, 0));

    // Display sound name
    __Display.setCursor(x, y);
    __Display.setFont(&FreeSans15);
    __Display.setTextFrontColor(cColor(0, 200, 255)); // Blue for all text
    __Display.drawTransText(soundName);

    const char* paramNames[] = {"Attack", "Decay", "Sustain", "Release", "Cutoff"};
    float paramValues[] = {
        sound->GetAttack(),
        sound->GetDecay(),
        sound->GetSustain(),
        sound->GetRelease(),
        sound->GetCutoff() / 20000.0f  // Normalize to 0-1 range
    };

    for (int i = 0; i < 5; ++i)
    {
        uint16_t textY = y + (i + 1) * spacing;
        __Display.setCursor(x, textY);
        __Display.setFont(&FreeSans15);
        __Display.setTextFrontColor(i == selectedParameter ? cColor(0, 200, 255) : cColor(255, 255, 255)); // Highlight selected parameter in blue

        __Display.drawTransText(paramNames[i]);

        // Draw selection box if this parameter is selected
        if (i == selectedParameter)
        {
            uint16_t boxY = textY - 15;
            uint16_t boxHeight = 20;
            uint16_t boxWidth = 60;
            cColor boxColor = (currentEditMode == EditMode::Editing) ? cColor(255, 255, 0) : cColor(0, 255, 0);
            __Display.drawRect(x - 2, boxY, boxWidth, boxHeight, boxColor);
        }

        // Draw parameter value bar
        uint16_t barX = x + barOffset;
        uint16_t barY = textY - 12;
        uint16_t fillWidth = static_cast<uint16_t>(paramValues[i] * barWidth);
        __Display.drawRect(barX, barY, barWidth, barHeight, cColor(255, 255, 255));
        __Display.drawFillRect(barX, barY, fillWidth, barHeight, cColor(0, 200, 255));
    }

    // Flush the frame buffer to update the display
    __Display.FlushFrame();
}

void DisplayLfoParameterMenu(Sound* sound)
{
    uint16_t x = 10;
    uint16_t y = 20;
    uint16_t spacing = 30;
    uint16_t barWidth = 100;
    uint16_t barHeight = 15;
    uint16_t barOffset = 130;

    // Clear the screen
    __Display.drawFillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, cColor(0, 0, 0));

    // Display LFO Intensity
    __Display.setCursor(x, y);
    __Display.setFont(&FreeSans15);
    __Display.setTextFrontColor(cColor(0, 200, 255));
    __Display.drawTransText("LFO Intensity");

    float lfoIntensity = sound->GetLfoIntensity();
    uint16_t fillWidth = static_cast<uint16_t>(lfoIntensity * barWidth);
    __Display.drawRect(x + barOffset, y, barWidth, barHeight, cColor(255, 255, 255));
    __Display.drawFillRect(x + barOffset, y, fillWidth, barHeight, cColor(0, 200, 255));

    // Display LFO ON/OFF
    __Display.setCursor(x, y + spacing);
    __Display.setFont(&FreeSans15);
    __Display.setTextFrontColor(cColor(0, 200, 255));
    __Display.drawTransText("LFO ON/OFF");

    bool lfoOn = sound->IsLfoOn();
    __Display.setCursor(x + barOffset, y + spacing);
    __Display.drawTransText(lfoOn ? "ON" : "OFF");

    // Flush the frame buffer to update the display
    __Display.FlushFrame();
}

void HandleLfoOnOffToggle(Sound* sound)
{
    if (button2.RisingEdge())
    {
        bool lfoOn = sound->IsLfoOn();
        sound->SetLfoOn(!lfoOn);
        DisplayLfoParameterMenu(sound);
    }
}

void DisplayFilterParameterMenu(Sound* sound)
{
    uint16_t x = 10;
    uint16_t y = 20;
    uint16_t spacing = 30;
    uint16_t graphWidth = 200;
    uint16_t graphHeight = 100;
    uint16_t graphX = x + 30;
    uint16_t graphY = y + 40;

    // Clear the screen
    __Display.drawFillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, cColor(0, 0, 0));

    // Display filter cutoff
    __Display.setCursor(x, y);
    __Display.setFont(&FreeSans15);
    __Display.setTextFrontColor(cColor(0, 200, 255));
    __Display.drawTransText("Filter Cutoff");

    float cutoff = sound->GetCutoff();
    __Display.setCursor(x + 130, y);
    __Display.drawTransText(std::to_string(static_cast<int>(cutoff)).c_str());

    // Display filter resonance
    __Display.setCursor(x, y + spacing);
    __Display.setFont(&FreeSans15);
    __Display.setTextFrontColor(cColor(0, 200, 255));
    __Display.drawTransText("Resonance");

    float resonance = sound->GetResonance();
    __Display.setCursor(x + 130, y + spacing);
    __Display.drawTransText(std::to_string(static_cast<int>(resonance)).c_str());

    // Draw a simple frequency response curve
    // This is just a placeholder visualization
    for (int i = 0; i < graphWidth; ++i)
    {
        int responseY = static_cast<int>(graphY + graphHeight / 2 * (1 - std::exp(-static_cast<float>(i) / graphWidth * cutoff / 10000.0f)));
        __Display.drawLine(graphX + i, responseY, graphX + i, responseY + 1, cColor(0, 200, 255));
    }

    // Flush the frame buffer to update the display
    __Display.FlushFrame();
}


