//------------------------------------------------------------------------
// Copyright(c) 2024 Dad Design.
//      Gestion d'une liaison SP vers un ecran 
//------------------------------------------------------------------------
#pragma once
#include "daisy_seed.h"
#include "per/spi.h"
#include "per/gpio.h"
#include "sys/system.h"
#include "UserConfig.h"
#include "ST7789_Defines.h"

using namespace daisy;
extern DaisySeed hw;

enum class SPIMode {
    Mode0,
    Mode1,
    Mode2,
    Mode3
};

enum class Rotation{
    Degre_0,
    Degre_90,
    Degre_180,
    Degre_270
};

// Configuration des GPIO utilisés
#define _TFT_SPI_PORT SpiHandle::Config::Peripheral::TFT_SPI_PORT
#define _TFT_SPI_MODE SPIMode::TFT_SPI_MODE

// Configuration des GPIO utilisés
#define _TFT_MOSI seed::TFT_MOSI
#define _TFT_SCLK seed::TFT_SCLK
#define _TFT_DC   seed::TFT_DC
#define _TFT_RST  seed::TFT_RST 

//***********************************************************************************
// TFT_SPI
//  Gestion de la liaison SPI
//*********************************************************************************** 
class TFT_SPI {
    public :

	// --------------------------------------------------------------------------
	// Initialisation du SPI
    void Init_TFT_SPI();
    
	// --------------------------------------------------------------------------
	// Initialisation spécifique au SPI
    void ST7789_Initialise();
	
    // --------------------------------------------------------------------------
	// Modification de l'orientation de l'écran
    void setTFTRotation(Rotation r);
    
    // --------------------------------------------------------------------------
    // Emission d'une commande
    inline void SendCommand(uint8_t cmd){
        m_dc.Write(false);
        m_spi.BlockingTransmit(&cmd, 1);
    }

    // --------------------------------------------------------------------------
    // Emission d'une donnée
    inline void SendData(uint8_t Data){
        m_dc.Write(true);
        m_spi.BlockingTransmit(&Data, 1);
    }

    // --------------------------------------------------------------------------
    // Emission d'un bloc de données
    inline void SendData(uint8_t* buff, size_t size) {
        m_dc.Write(true);
        m_spi.BlockingTransmit(buff, size);
    }

    // --------------------------------------------------------------------------
    // Emission d'une commande en mode DMA
    inline void SendDMACommand(uint8_t *cmd, SpiHandle::EndCallbackFunctionPtr end_callback=NULL, void* callback_context=NULL)
    {
        m_dc.Write(false);
        m_spi.DmaTransmit(cmd, 1, NULL, end_callback, callback_context);
    }
    
    // --------------------------------------------------------------------------
    // Emission d'un bloc de données en mode DMA
    inline void SendDMAData(uint8_t* buff, size_t size, SpiHandle::EndCallbackFunctionPtr end_callback=NULL, void* callback_context=NULL)
    {   
        m_dc.Write(true);
        m_spi.DmaTransmit(buff, size, NULL, end_callback, callback_context);
    }

    // --------------------------------------------------------------------------
    // Delay en mimisecondes    
    inline void Delay(uint32_t msTime){
        System::Delay(msTime);
    }
    
    // --------------------------------------------------------------------------
    // Positionnement de la la broche Data/Command    
    inline void setDC(){
        m_dc.Write(true);       
    }
    inline void resetDC(){
        m_dc.Write(false);     
    }

    // --------------------------------------------------------------------------
    // Positionnement de la la broche Reset
    inline void setRST(){
        m_reset.Write(true);       
    }
    
    inline void resetRST(){
        m_reset.Write(true);       
    }
    
    // --------------------------------------------------------------------------
    // Données de la classe    
    protected :
    SpiHandle           m_spi;
    SpiHandle::Config   m_spi_config;
    
    GPIO                m_reset;
    GPIO                m_dc;
};