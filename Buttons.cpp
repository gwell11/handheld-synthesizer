//Buttons.cpp:

#include "Buttons.h"

// Keep existing buttons
Switch button1;
Switch button2;
Switch buttonBack;

// Add new chord buttons
Switch buttonC;
Switch buttonDm;
Switch buttonEm;
Switch buttonF;
Switch buttonG;
Switch buttonAm;
Switch buttonBdim;

void InitButtons()
{
    // Initialize existing buttons
    button1.Init(hw.GetPin(28), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
    button2.Init(hw.GetPin(27), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
    buttonBack.Init(hw.GetPin(26), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);

    // Initialize new chord buttons
    buttonC.Init(hw.GetPin(0), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
    buttonDm.Init(hw.GetPin(1), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
    buttonEm.Init(hw.GetPin(2), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
    buttonF.Init(hw.GetPin(3), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
    buttonG.Init(hw.GetPin(4), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
    buttonAm.Init(hw.GetPin(5), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
    buttonBdim.Init(hw.GetPin(6), 1000, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::Pull::PULL_UP);
}

void UpdateButtons() {
    // Debounce existing buttons
    button1.Debounce();
    button2.Debounce();
    buttonBack.Debounce();

    // Debounce new chord buttons
    buttonC.Debounce();
    buttonDm.Debounce();
    buttonEm.Debounce();
    buttonF.Debounce();
    buttonG.Debounce();
    buttonAm.Debounce();
    buttonBdim.Debounce();
}