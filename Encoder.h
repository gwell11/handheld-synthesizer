// Encoder.h

#ifndef ENCODER_H
#define ENCODER_H

#include "daisy_seed.h"
using namespace daisy;

class Encoder
{
public:
    void Init(dsy_gpio_pin pin_a, dsy_gpio_pin pin_b, dsy_gpio_pin pin_sw);
    void Debounce();
    int Increment();
    bool RisingEdge();
};

extern Encoder encoder; // Declare the encoder object as an external variable

void InitEncoder(); // Declaration of the InitEncoder function

#endif // ENCODER_H
