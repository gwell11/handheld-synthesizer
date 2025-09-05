//------------------------------------------------------------------------
// Copyright(c) 2024 Dad Design.
//      Gestion d'une liaison SP vers un écran 
//------------------------------------------------------------------------
#include "Debug.h"
#include "TFT_SPI.h"
#include "ST7789_Defines.h"

//***********************************************************************************
// TFT_SPI
//  Gestion de la liaison SPI
//*********************************************************************************** 

// --------------------------------------------------------------------------
// Initialisation du SPI
void TFT_SPI::Init_TFT_SPI(){
    // Configuration du SPI
    m_spi_config.periph     = _TFT_SPI_PORT;
    m_spi_config.mode       = SpiHandle::Config::Mode::MASTER;
    m_spi_config.direction  = SpiHandle::Config::Direction::TWO_LINES_TX_ONLY;
    m_spi_config.datasize   = 8;
    
    switch(_TFT_SPI_MODE) {
        case SPIMode::Mode3 :
            m_spi_config.clock_polarity = SpiHandle::Config::ClockPolarity::HIGH;
            m_spi_config.clock_phase    = SpiHandle::Config::ClockPhase::TWO_EDGE;
        break;
        case SPIMode::Mode2 :
            m_spi_config.clock_polarity = SpiHandle::Config::ClockPolarity::LOW;
            m_spi_config.clock_phase    = SpiHandle::Config::ClockPhase::TWO_EDGE;
        break;
        case SPIMode::Mode1 :
            m_spi_config.clock_polarity = SpiHandle::Config::ClockPolarity::HIGH;
            m_spi_config.clock_phase    = SpiHandle::Config::ClockPhase::ONE_EDGE;
        break;
        case SPIMode::Mode0 :
        default :
            m_spi_config.clock_polarity = SpiHandle::Config::ClockPolarity::LOW;
            m_spi_config.clock_phase    = SpiHandle::Config::ClockPhase::ONE_EDGE;
        break;
    }
    
    m_spi_config.nss            = SpiHandle::Config::NSS::SOFT;
    m_spi_config.baud_prescaler = SpiHandle::Config::BaudPrescaler::TFT_SPI_BaudPrescaler;

    m_spi_config.pin_config.sclk = _TFT_SCLK;
    m_spi_config.pin_config.miso = dsy_gpio_pin();
    m_spi_config.pin_config.mosi = _TFT_MOSI;
    m_spi_config.pin_config.nss  = dsy_gpio_pin();
        
    // TFT control pin config
    m_dc.Init(_TFT_DC, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::VERY_HIGH);
    m_reset.Init(_TFT_RST, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::VERY_HIGH);
    m_dc.Write(true);
    m_reset.Write(true);

    // Initialize SPI
    m_spi.Init(m_spi_config);
    
    // Reset LCD
    m_reset.Write(false);
    System::Delay(50);
    m_reset.Write(true);
    System::Delay(50);
    ST7789_Initialise();
}

// --------------------------------------------------------------------------
// Initialisation de l'écran de type ST7789 (thank eSPI)
 void TFT_SPI::ST7789_Initialise(){
    
    SendCommand(ST7789_SLPOUT);   // Sleep out
    System::Delay(120);
    SendCommand(ST7789_NORON);    // Normal display mode on

    //------------------------------display and color format setting--------------------------------//
    SendCommand(ST7789_MADCTL);
    SendData(TFT_MAD_COLOR_ORDER);

    // JLX240 display datasheet
    SendCommand(0xB6);
    SendData(0x0A);
    SendData(0x82);

    SendCommand(ST7789_RAMCTRL);
    SendData(0x00);
    SendData(0xC0); // 5 to 6-bit conversion: r0 = r5, b0 = b5

    SendCommand(ST7789_COLMOD);
#if TFT_COLOR == 16
        SendData(0x55);
#else
        SendData(0x66);
#endif
    System::Delay(10);

    //--------------------------------ST7789V Frame rate setting----------------------------------//
    SendCommand(ST7789_PORCTRL);
    SendData(0x0c);
    SendData(0x0c);
    SendData(0x00);
    SendData(0x33);
    SendData(0x33);

    SendCommand(ST7789_GCTRL);      // Voltages: VGH / VGL
    SendData(0x35);

    //---------------------------------ST7789V Power setting--------------------------------------//
    SendCommand(ST7789_VCOMS);
    SendData(0x28);		// JLX240 display datasheet

    SendCommand(ST7789_LCMCTRL);
    SendData(0x0C);

    SendCommand(ST7789_VDVVRHEN);
    SendData(0x01);
    SendData(0xFF);

    SendCommand(ST7789_VRHS);       // voltage VRHS
    SendData(0x10);

    SendCommand(ST7789_VDVSET);
    SendData(0x20);

    SendCommand(ST7789_FRCTR2);
    SendData(0x0f);

    SendCommand(ST7789_PWCTRL1);
    SendData(0xa4);
    SendData(0xa1);

    //--------------------------------ST7789V gamma setting---------------------------------------//
    SendCommand(ST7789_PVGAMCTRL);
    SendData(0xd0);
    SendData(0x00);
    SendData(0x02);
    SendData(0x07);
    SendData(0x0a);
    SendData(0x28);
    SendData(0x32);
    SendData(0x44);
    SendData(0x42);
    SendData(0x06);
    SendData(0x0e);
    SendData(0x12);
    SendData(0x14);
    SendData(0x17);

    SendCommand(ST7789_NVGAMCTRL);
    SendData(0xd0);
    SendData(0x00);
    SendData(0x02);
    SendData(0x07);
    SendData(0x0a);
    SendData(0x28);
    SendData(0x31);
    SendData(0x54);
    SendData(0x47);
    SendData(0x0e);
    SendData(0x1c);
    SendData(0x17);
    SendData(0x1b);
    SendData(0x1e);

    SendCommand(ST7789_CASET);    // Column address set
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0xEF);    // 239

    SendCommand(ST7789_RASET);    // Row address set
    SendData(0x00);
    SendData(0x00);
    SendData(0x01);
    SendData(0x3F);    // 319

    SendCommand(ST7789_INVON);
    SendCommand(ST7789_INVON);

    System::Delay(120);
    SendCommand(ST7789_DISPON);    //Display on
    System::Delay(120);
}

// Set the display image orientation to 0, 1, 2 or 3
void  TFT_SPI::setTFTRotation(Rotation r){
    SendCommand(TFT_MADCTL);
    switch (r) {
    case Rotation::Degre_0 : // Portrait
        SendData(TFT_MAD_COLOR_ORDER);
        break;
    case Rotation::Degre_90 : // Landscape (Portrait + 90)
        SendData(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
        break;
    case Rotation::Degre_180 : // Inverter portrait
        SendData(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
        break;
    case Rotation::Degre_270 : // Inverted landscape
        SendData(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
        break;
    }
}