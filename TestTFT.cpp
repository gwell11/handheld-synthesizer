//Main.cpp


#include "daisy_seed.h"
#include "GFX.h"
#include "Vanilla20pt.h"
#include "FreeSansBold24pt.h"
#include "Pap.h"
#include "Buttons.h"
#include "Menu.h"
#include "daisy.h"
#include <stdio.h>
#include "Spirits.h"


#define DSY_SDRAM_BSS_FRAMEBUFF __attribute__((section(".sdram_bss_framebuff")))
#define DSY_SDRAM_BSS_PAPMAP __attribute__((section(".sdram_bss_papmap")))
#define MAIN_MENU_OPTIONS 4
#define CHORD_SELECTION_OPTIONS 8  // For the 7 chords in C major
#define SOUND_DESIGN_OPTIONS 5     // Oscillator, ADSR, LFO, Effects, Filter
#define PERFORMANCE_SETTINGS_OPTIONS 3  // Example: Arpeggiator, Chord Strumming, Velocity
#define GLOBAL_SETTINGS_OPTIONS 4  // Example: MIDI, Output, Screen, Save/Load
using namespace daisy;


Spirits spirits;
Spirits sound2;
Spirits option3Sound;

DaisySeed hw;
int projectBPM = 120;

FIFO_Data DMA_BUFFER_MEM_SECTION __Fifo;
RGB DSY_SDRAM_BSS_FRAMEBUFF __FrameBuff[TFT_WIDTH * TFT_HEIGHT];
cGFX __Display;
cFont FreeSans15(&FreeSans15pt7b);
//cFont FreeSans30B(&FreeSansBold30pt7b);
uint32_t lastTriggerTime = 0;

Encoder encoder;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        float spirit_out = spirits.Process();
        float sound2_out = sound2.Process();
        float option3_out = option3Sound.Process();
        
        // Mix the sounds or choose which one to output based on your requirements
        float mixed_out = spirit_out + sound2_out + option3_out;  // Simple mixing, adjust as needed
        
        out[0][i] = mixed_out;
        out[1][i] = mixed_out;
    }
}



void InitEncoder()
{
    encoder.Init(seed::D15, seed::D16, seed::D17);
}

void InitHardware()
{
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(32);
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    hw.StartAudio(AudioCallback);
    hw.usb_handle.Init(UsbHandle::FS_INTERNAL);
    float sample_rate = hw.AudioSampleRate();
    spirits.Init(sample_rate);
    sound2.Init(sample_rate);
    option3Sound.Init(sample_rate);    
    __Display.Init(__FrameBuff, &__Fifo, TFT_WIDTH, TFT_HEIGHT);
    __Display.setRotation(Rotation::Degre_270);
    

    InitButtons();
    InitEncoder();

    System::Delay(500);
    hw.PrintLine("Hardware initialized");
}



void UpdateMenu()
{
    int optionCount;
    switch (currentState)
    {
        case MAIN_MENU:
            optionCount = MAIN_MENU_OPTIONS;
            break;
        case CHORD_SELECTION:
            optionCount = CHORD_SELECTION_OPTIONS;
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

    bool menuChanged = false;
    int increment = -encoder.Increment();

    if (increment != 0)
    {
        int oldOption = currentOption;
        currentOption = (currentOption + increment + optionCount) % optionCount;
        menuChanged = true;
        hw.PrintLine("Encoder increment: %d, Scrolling from %d to %d (optionCount: %d)", 
                     increment, oldOption, currentOption, optionCount);
    }

    // Handle chord button presses for navigation only
    if (currentState == CHORD_SELECTION)
    {
        if (buttonC.RisingEdge()) { currentOption = 0; menuChanged = true; }
        else if (buttonDm.RisingEdge()) { currentOption = 1; menuChanged = true; }
        else if (buttonEm.RisingEdge()) { currentOption = 2; menuChanged = true; }
        else if (buttonF.RisingEdge()) { currentOption = 3; menuChanged = true; }
        else if (buttonG.RisingEdge()) { currentOption = 4; menuChanged = true; }
        else if (buttonAm.RisingEdge()) { currentOption = 5; menuChanged = true; }
        else if (buttonBdim.RisingEdge()) { currentOption = 6; menuChanged = true; }
    }

    if (menuChanged) {
        hw.PrintLine("Current Option changed to: %d", currentOption);
        DisplayMenu();
    }
}




int main(void)
{
    InitHardware();
    InitMenu();
    DisplayMenu();

    hw.PrintLine("Main loop starting...");

   while (1)
    {
        UpdateButtons();
        encoder.Debounce();

        UpdateMenu();


        // Handle sound triggering for all sounds
        if (currentState == CHORD_SELECTION)
        {
            if (buttonC.Pressed()) {
                spirits.Trigger(0);
                hw.PrintLine("C Major triggered");
            } else if (buttonDm.Pressed()) {
                spirits.Trigger(1);
                hw.PrintLine("D Minor triggered");
            } else if (buttonEm.Pressed()) {
                spirits.Trigger(2);
                hw.PrintLine("E Minor triggered");
            } else if (buttonF.Pressed()) {
                spirits.Trigger(3);
                hw.PrintLine("F Major triggered");
            } else if (buttonG.Pressed()) {
                spirits.Trigger(4);
                hw.PrintLine("G Major triggered");
            } else if (buttonAm.Pressed()) {
                spirits.Trigger(5);
                hw.PrintLine("A Minor triggered");
            } else if (buttonBdim.Pressed()) {
                spirits.Trigger(6);
                hw.PrintLine("B Diminished triggered");
            }
        }





        if (button2.RisingEdge() || encoder.RisingEdge())
        {
            HandleMenuSelection(encoder.RisingEdge());
        }

        // Handle button1 for Visuals
        if (button1.RisingEdge())
        {
            stateStack.push(currentState);
            currentState = VISUALS;
            DisplayMenu();
        }

        HandleBackButton();
        // Special handling for BPM adjustment (now part of Global Settings)
        if (currentState == GLOBAL_SETTINGS && currentOption == 0) // Assuming BPM is the first option in Global Settings
        {
            int increment = -encoder.Increment();
            if (increment != 0)
            {
                projectBPM = std::max(20, std::min(300, projectBPM + increment));
                hw.PrintLine("BPM adjusted: %d", projectBPM); //bpm set to 120 upon initialization
                DisplayMenu();
            }
        }


        System::DelayUs(50);
    }
}

