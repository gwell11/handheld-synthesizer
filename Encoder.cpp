// Encoder.cpp

#include "Encoder.h"

Encoder encoder;

void Encoder::Init(dsy_gpio_pin pin_a, dsy_gpio_pin pin_b, dsy_gpio_pin pin_sw)
{
    encoder_a_.pin = pin_a;
    encoder_b_.pin = pin_b;
    encoder_sw_.pin = pin_sw;

    encoder_a_.mode = DSY_GPIO_MODE_INPUT;
    encoder_b_.mode = DSY_GPIO_MODE_INPUT;
    encoder_sw_.mode = DSY_GPIO_MODE_INPUT;

    dsy_gpio_init(&encoder_a_);
    dsy_gpio_init(&encoder_b_);
    dsy_gpio_init(&encoder_sw_);

    last_state_ = dsy_gpio_read(&encoder_a_) | (dsy_gpio_read(&encoder_b_) << 1);
    last_sw_state_ = dsy_gpio_read(&encoder_sw_);
    increment_ = 0;
    rising_edge_ = false;
}

void Encoder::Debounce()
{
    int current_state = dsy_gpio_read(&encoder_a_) | (dsy_gpio_read(&encoder_b_) << 1);
    if (current_state != last_state_)
    {
        if ((last_state_ == 0 && current_state == 1) ||
            (last_state_ == 1 && current_state == 3) ||
            (last_state_ == 3 && current_state == 2) ||
            (last_state_ == 2 && current_state == 0))
        {
            increment_ = -1; // Changed from 1 to -1
            Debug::PrintLine("Encoder turned right"); // Add this debug print
        }
        else if ((last_state_ == 0 && current_state == 2) ||
                 (last_state_ == 2 && current_state == 3) ||
                 (last_state_ == 3 && current_state == 1) ||
                 (last_state_ == 1 && current_state == 0))
        {
            increment_ = 1; // Changed from -1 to 1
            Debug::PrintLine("Encoder turned left"); // Add this debug print
        }
        last_state_ = current_state;
        
    }
    else
    {
        increment_ = 0;
    }

    bool current_sw_state = dsy_gpio_read(&encoder_sw_);
    rising_edge_ = !last_sw_state_ && current_sw_state;
    last_sw_state_ = current_sw_state;
}

int Encoder::Increment()
{
    return increment_;
}

bool Encoder::RisingEdge()
{
    return rising_edge_;
}

void InitEncoder()
{
    encoder.Init(seed::D15, seed::D16, seed::D17); // Use pins D15, D16, and D17
}
