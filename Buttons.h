//Buttons.h:

#pragma once
#include "daisy_seed.h"
using namespace daisy;

extern DaisySeed hw;

// Keep existing buttons
extern Switch button1;
extern Switch button2;
extern Switch buttonBack;

// Add new chord buttons
extern Switch buttonC;
extern Switch buttonDm;
extern Switch buttonEm;
extern Switch buttonF;
extern Switch buttonG;
extern Switch buttonAm;
extern Switch buttonBdim;

void InitButtons();
void UpdateButtons();