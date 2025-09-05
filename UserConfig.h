//------------------------------------------------------------------------
// Copyright(c) 2023 Dad Design.
//      Definir ici les paramètre de votre écran
//------------------------------------------------------------------------
#pragma once

// Hauteur et largeur de l'écran en pixel
#define TFT_WIDTH       240
#define TFT_HEIGHT      320

// Type de contôleur
#define TFT_CONTROLEUR_TFT  ST7789
//#define TFT_CONTROLEUR_TFT  ST7735

// Codage des couleurs
// 18 Codage 18 bits RGB666
// 16 Codage 16 bits RGB565

//#define TFT_COLOR 18
#define TFT_COLOR 16

// Configuration du SPI
#define TFT_SPI_PORT SPI_1
#define TFT_SPI_MODE Mode3
#define TFT_SPI_BaudPrescaler PS_2


// Configuration des GPIO utilisés par le SPI
#define TFT_MOSI D10
#define TFT_SCLK D8
#define TFT_DC   D21
#define TFT_RST  D20

// Définition de la taille des blocs utilisés pour une mise à jour partielle de l'écran
//   -> On me transmet à l'écran que les bloc quii on été modifiés
// Ex : ecran 240x320 / FRAME_GRILLE 10 -> Bloc = 24x32
// Attention la hauteur et la largeur  doivent être divisible par FRAME_GRILLE
#define FRAME_GRILLE 10

// Taille du FIFO pour la transmission SPI des blocs par le DMA
// Attention nombre limité car le FIFO est placé dans la SRAM D1 qui semble être utilisée aussi par un autre process
#define SIZE_FIFO 10