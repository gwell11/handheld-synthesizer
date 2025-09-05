//------------------------------------------------------------------------
// Copyright(c) 2024 Dad Design.
//      Bibliothèque graphique
//
// Inspiré largement de :
//    Adafruit-GFX-Library : https://github.com/adafruit/Adafruit-GFX-Library
//    eSPI : https://github.com/Bodmer/TFT_eSPI
//------------------------------------------------------------------------
#include "Debug.h"
#include "GFX.h"

//***********************************************************************************
// CFont
// Gestion des polices de caratères
// Utilisation de la structuration des fonts de Adafruit-GFX-Library
// Ce qui permet de profiter des outils de conversion (ex: https://rop.nl/truetype2gfx/)
cFont::cFont(const GFXfont *pFont)
{
    m_pFont = pFont;         // Pointe sur le descripteur de font
    m_pTable = pFont->glyph; // Pointe sur la table des descriteurs de carratères

    // On parcours tous les caratères pour déterminer la hauteur max des caratères
    // En sortie nous avons deux variables configurées :
    //    m_NegHeight = hauteur sous la ligne du curseur;
    //    m_PosHeight = hauteur au dessus de la ligne du curseur;
    GFXglyph *pTable = m_pTable;
    uint16_t SizeTable = 1 + pFont->last - pFont->first;
    m_NegHeight = 0;
    m_PosHeight = 0;
    for (uint16_t index = 0; index < SizeTable; index++)
    {
        int8_t Offset = pTable->yOffset;
        int8_t NegHeight = pTable->height + Offset;
        if (NegHeight > m_NegHeight)
        {
            m_NegHeight = NegHeight;
        }
        if (Offset < m_PosHeight)
        {
            m_PosHeight = Offset;
        }
        pTable++;
    }
}

//***********************************************************************************
// cGFX
//   Bibliothèque Graphique

//-----------------------------------------------------------------------------------
// Tracer un rectange vide 
void cGFX::drawRect(uint16_t x, uint16_t y, int16_t dx, int16_t dy, cColor Color){
    if(dx < 0){ x += dx; dx = -dx;}
    if(dy < 0){ y += dy; dy = -dy;}

    drawLine(x, y, x+dx, y, Color);
    drawLine(x+dx, y, x+dx, y+dy, Color);
    drawLine(x+dx, y+dy, x, y+dy, Color);
    drawLine(x, y+dy, x, y, Color);
}

//-----------------------------------------------------------------------------------
// Tracer un rectangle plein
void cGFX::drawFillRect(uint16_t x, uint16_t y, int16_t dx, int16_t dy, cColor Color){
    if(dx < 0){ x += dx; dx = -dx;}
    if(dy < 0){ y += dy; dy = -dy;}

    RGB *pFrame;
    RGB *pEndLigne;
    
    for (uint16_t PosY = y; PosY < (y+dy); PosY++){
        pFrame = getPtr(x, PosY);
        pEndLigne = getPtr(x+dx, PosY);
        while (pFrame < pEndLigne){
            pFrame->set(Color);
            pFrame++;
        }
    }
}
//-----------------------------------------------------------------------------------
// Dessin d'une ligne
// Merci a Frédéric Goset http://fredericgoset.ovh/
// L'algorithme de ligne de Bresenham 
void cGFX::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, cColor Color)
{
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    
    int incX = dx < 0 ? -1 : (dx > 0 ? 1 : 0);
    int incY = dy < 0 ? -1 : (dy > 0 ? 1 : 0);
    if(dx < 0) dx = -dx;
    if(dy < 0) dy = -dy;

    RGB *pFrame;
    RGB *pEndLigne;
 
    if (dy == 0){
        // horizontal line
        uint16_t Temp;
        if(x1 < x0) {Temp = x1; x1 = x0 ; x0= Temp;}
        pFrame = getPtr(x0,y0);
        pEndLigne = getPtr(x1,y1);
        while (pFrame < pEndLigne){
            pFrame->set(Color);
            pFrame++;
        }


    }else if (dx == 0){
        uint16_t Temp;
        if(y1 < y0) {Temp = y1; y1 = y0 ; y0= Temp;}
        pFrame = getPtr(x0,y0);
        pEndLigne = getPtr(x1,y1);
        while (pFrame < pEndLigne){
            pFrame->set(Color);
            pFrame += getWidth();
        }
    
    }else if (dx >= dy) {
        // more horizontal than vertical
        int slope = 2 * dy;
        int error = -dx;
        int errorInc = -2 * dx;
        int y = y0;

        for (int x = x0; x != x1 + incX; x += incX)
        {
            setPixel(x,y, Color);
            error += slope;

            if (error >= 0)
            {
                y += incY;
                error += errorInc;
            }
        }
    
    }else{
        // more vertical than horizontal
        int slope = 2 * dx;
        int error = -dy;
        int errorInc = -2 * dy;
        int x = x0;

        for (int y = y0; y != y1 + incY; y += incY)
        {
            setPixel(x,y, Color);
            error += slope;

            if (error >= 0)
            {
                x += incX;
                error += errorInc;
            }
        }
    }
}

//-----------------------------------------------------------------------------------
// Dessin d'un cercle
// Merci a Frédéric Goset http://fredericgoset.ovh/
// L'algorithme de cercle de Bresenham 
void cGFX::drawCircle(uint16_t centerX, uint16_t centerY, uint16_t radius, cColor Color)
{
    int16_t x = 0;
    int16_t y = radius;
    int16_t m = 5 - 4 * radius;

    while (x <= y)
    {
        setPixel(centerX + x, centerY + y, Color);
        setPixel(centerX + x, centerY - y, Color);
        setPixel(centerX - x, centerY + y, Color);
        setPixel(centerX - x, centerY - y, Color);
        setPixel(centerX + y, centerY + x, Color);
        setPixel(centerX + y, centerY - x, Color);
        setPixel(centerX - y, centerY + x, Color);
        setPixel(centerX - y, centerY - x, Color);

        if (m > 0)
        {
            y--;
            m -= 8 * y;
        }
        x++;
        m += 8 * x + 4;
    }
}

//-----------------------------------------------------------------------------------
// Dessin d'un arc de cercle
// L'algorithme de cercle de Bresenham 
void cGFX::drawArc(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t AlphaIn, uint16_t AlphaOut, cColor Color)
{
    bool Inv = false;
    if(AlphaIn > AlphaOut){
        uint16_t Temp = AlphaIn;
        AlphaIn = AlphaOut;
        AlphaOut = Temp;
        Inv = true;
    }
    int16_t x = 0;
    int16_t y = radius;
    int16_t m = 5 - 4 * radius;

    while (x <= y) {
        
        uint16_t angle = atan2((float) y, (float) x) * 180 / __PI; // Conversion de radians en degrés
        if(Inv){
            uint16_t angle1 = 90 - angle;        
            if(!((angle1 >= AlphaIn) && (angle1 <= AlphaOut)))  setPixel(centerX + x, centerY - y, Color);
            uint16_t angle2 = angle;
            if(!((angle2 >= AlphaIn) && (angle2 <= AlphaOut)))  setPixel(centerX + y, centerY - x, Color);
            angle1 += 90;       
            if(!((angle1 >= AlphaIn) && (angle1 <= AlphaOut)))  setPixel(centerX + y, centerY + x, Color);
            angle2 += 90;
            if(!((angle2 >= AlphaIn) && (angle2 <= AlphaOut)))  setPixel(centerX + x, centerY + y, Color);
            angle1 += 90;
            if(!((angle1 >= AlphaIn) && (angle1 <= AlphaOut)))  setPixel(centerX - x, centerY + y, Color);
            angle2 += 90;
            if(!((angle2 >= AlphaIn) && (angle2 <= AlphaOut)))  setPixel(centerX - y, centerY + x, Color);
            angle1 += 90;
            if(!((angle1 >= AlphaIn) && (angle1 <= AlphaOut)))  setPixel(centerX - y, centerY - x, Color);
            angle2 += 90;
            if(!((angle2 >= AlphaIn) && (angle2 <= AlphaOut)))  setPixel(centerX - x, centerY - y, Color);
        }else{
            uint16_t angle1 = 90 - angle;        
            if(((angle1 >= AlphaIn) && (angle1 <= AlphaOut)))  setPixel(centerX + x, centerY - y, Color);
            uint16_t angle2 = angle;
            if(((angle2 >= AlphaIn) && (angle2 <= AlphaOut)))  setPixel(centerX + y, centerY - x, Color);
            angle1 += 90;       
            if(((angle1 >= AlphaIn) && (angle1 <= AlphaOut)))  setPixel(centerX + y, centerY + x, Color);
            angle2 += 90;
            if(((angle2 >= AlphaIn) && (angle2 <= AlphaOut)))  setPixel(centerX + x, centerY + y, Color);
            angle1 += 90;
            if(((angle1 >= AlphaIn) && (angle1 <= AlphaOut)))  setPixel(centerX - x, centerY + y, Color);
            angle2 += 90;
            if(((angle2 >= AlphaIn) && (angle2 <= AlphaOut)))  setPixel(centerX - y, centerY + x, Color);
            angle1 += 90;
            if(((angle1 >= AlphaIn) && (angle1 <= AlphaOut)))  setPixel(centerX - y, centerY - x, Color);
            angle2 += 90;
            if(((angle2 >= AlphaIn) && (angle2 <= AlphaOut)))  setPixel(centerX - x, centerY - y, Color);
        }

        if (m > 0)
        {
            y--;
            m -= 8 * y;
        }
        x++;
        m += 8 * x + 4;
    }
}

//-----------------------------------------------------------------------------------
// Dessin d'un cercle plein
void cGFX::drawFillCircle(uint16_t centerX, uint16_t centerY, uint16_t radius, cColor Color)
{
    int x = 0;
    int y = radius;
    int m = 5 - 4 * radius;

    while (x <= y)
    {
        RGB *pFrame = getPtr(centerX - y, centerY - x);
        for (int xx = centerX - y; xx <= centerX + y; xx++)
            pFrame++->set(Color);

        pFrame = getPtr(centerX - y, centerY + x);
        for (int xx = centerX - y; xx <= centerX + y; xx++)
            pFrame++->set(Color);

        if (m > 0)
        {
            pFrame = getPtr(centerX - x, centerY - y);
            for (int xx = centerX - x; xx <= centerX + x; xx++)
                pFrame++->set(Color);

            pFrame = getPtr(centerX - x, centerY + y);
            for (int xx = centerX - x; xx <= centerX + x; xx++)
                pFrame++->set(Color);

            y--;
            m -= 8 * y;
        }

        x++;
        m += 8 * x + 4;
    }
}

//-----------------------------------------------------------------------------------
// Tracer une image 8bits par couleurs
void cGFX::drawR8G8B8Image(uint16_t x, uint16_t y, uint16_t dx, uint16_t dy, const uint8_t *pImg){
    RGB *pFrame;
    RGB *pEndLigne;
    
    for (uint16_t PosY = y; PosY < (y+dy); PosY++){
        pFrame = getPtr(x, PosY);
        pEndLigne = getPtr(x+dx, PosY);
        while (pFrame < pEndLigne){
            pFrame->set(cColor((*(pImg+2)),(*(pImg+1)),(*(pImg))));
            pImg = pImg+3;
            pFrame++;
        }
    }
}
// ==========================================================================
// Dessiner du texte

//-----------------------------------------------------------------------------------
// Dessiner le caractère c
void cGFX::drawChar(const char c, bool Erase){
    const GFXglyph *pTable = m_pFont->getGFXglyph(c);
    const uint8_t *pBitmap = m_pFont->getBitmap(c);
    uint8_t BitMap = *pBitmap++;
    uint8_t numBit = 0;
    uint8_t indexX;
    RGB * pFrame;
    
    for (uint8_t indexY=0; indexY < pTable->height; indexY ++){
        indexX = 0;
        pFrame = getPtr(m_xCursor+pTable->xOffset, indexY+m_yCursor+pTable->yOffset);
        while (indexX < pTable->width){
            if((BitMap&0x80) != 0){
                if(Erase){
                    pFrame->set(m_TextBackColor);
                }else{
                    pFrame->set(m_TextFrontColor);
                }
            }
            pFrame++;
            numBit++;
            if(numBit > 7){
                numBit = 0;
                BitMap = *pBitmap++;
            }else{
                BitMap <<= 1 ;
            }
            indexX++;
        }
    }  
    m_xCursor += pTable->xAdvance;
}

//-----------------------------------------------------------------------------------
// Dessiner le texte
void cGFX::drawText(const char *Text, bool Erase){
    drawFillRect(m_xCursor, m_yCursor-m_pFont->getPosHeight(),m_pFont->getTextWidth(Text), m_pFont->getHeight(), m_TextBackColor);
    const char *pText = Text;
    while(*pText != '\0'){
        drawChar(*pText++, Erase);
    }
}

//-----------------------------------------------------------------------------------
// Dessiner le texte sans couleur d'arriere plan
void cGFX::drawTransText(const char *Text, bool Erase){
    const char *pText = Text;
    while(*pText != '\0'){
        drawChar(*pText++, Erase);
    }
}

void cGFX::drawGhost(uint16_t x, uint16_t y, uint16_t width, uint16_t height, cColor bodyColor, cColor eyeColor)
{
    // Draw the ghost's body (main circle)
    uint16_t radius = width / 2;
    drawFillCircle(x + radius, y + radius, radius, bodyColor);

    // Draw the ghost's bottom part (rectangular)
    drawFillRect(x, y + radius, width, height - radius, bodyColor);

    // Draw the ghost's eyes (small circles)
    uint16_t eyeRadius = width / 10;
    drawFillCircle(x + radius / 2, y + radius - eyeRadius / 2, eyeRadius, eyeColor);
    drawFillCircle(x + 3 * radius / 2, y + radius - eyeRadius / 2, eyeRadius, eyeColor);


    // Draw the ghost's arms (thick lines)
    uint16_t armLength = width / 2; // Length of the arms
    uint16_t armThickness = 5; // Thickness of the arms
    
    // Left arm
    for (uint16_t i = 0; i < armThickness; ++i)
    {
        drawLine(x, y + radius + i, x - armLength, y + radius - armLength / 4 + i, bodyColor);
    }
    
    // Fingers on the left arm
    uint16_t leftArmX = x - armLength;
    uint16_t leftArmY = y + radius - armLength / 4;
    uint16_t fingerLength = armLength / 4;
    drawLine(leftArmX, leftArmY, leftArmX - fingerLength, leftArmY - fingerLength / 2, bodyColor); // Upper finger
    drawLine(leftArmX, leftArmY, leftArmX - fingerLength, leftArmY, bodyColor); // Middle finger
    drawLine(leftArmX, leftArmY, leftArmX - fingerLength, leftArmY + fingerLength / 2, bodyColor); // Lower finger

    // Right arm
    for (uint16_t i = 0; i < armThickness; ++i)
    {
        drawLine(x + width, y + radius + i, x + width + armLength, y + radius - armLength / 4 + i, bodyColor);
    }

    // Fingers on the right arm
    uint16_t rightArmX = x + width + armLength;
    uint16_t rightArmY = y + radius - armLength / 4;
    drawLine(rightArmX, rightArmY, rightArmX + fingerLength, rightArmY - fingerLength / 2, bodyColor); // Upper finger
    drawLine(rightArmX, rightArmY, rightArmX + fingerLength, rightArmY, bodyColor); // Middle finger
    drawLine(rightArmX, rightArmY, rightArmX + fingerLength, rightArmY + fingerLength / 2, bodyColor); // Lower finger
}


void cGFX::drawExplosion(uint16_t x, uint16_t y, uint16_t size, cColor mainColor, cColor secondaryColor)
{
    // Draw the center of the explosion (main circle with a smiley face)
    uint16_t centerRadius = size / 4;
    drawFillCircle(x, y, centerRadius, mainColor);
    
    // Draw smiley face in the center
    uint16_t eyeRadius = centerRadius / 8;
    drawFillCircle(x - centerRadius / 3, y - centerRadius / 4, eyeRadius, secondaryColor);
    drawFillCircle(x + centerRadius / 3, y - centerRadius / 4, eyeRadius, secondaryColor);
    drawArc(x, y + centerRadius / 8, centerRadius / 2, 30, 150, secondaryColor);
    
    // Draw outer circles for the explosion effect
    uint16_t numCircles = 10; // Number of circles
    for (uint16_t i = 0; i < numCircles; ++i)
    {
        float angle = i * (360.0 / numCircles);
        float rad = angle * (3.14159 / 180.0); // Convert to radians
        
        uint16_t outerX = x + size * cos(rad);
        uint16_t outerY = y + size * sin(rad);
        
        drawFillCircle(outerX, outerY, centerRadius / 2, mainColor);
        
        // Draw smiley faces on outer circles
        drawFillCircle(outerX - centerRadius / 6, outerY - centerRadius / 8, eyeRadius, secondaryColor);
        drawFillCircle(outerX + centerRadius / 6, outerY - centerRadius / 8, eyeRadius, secondaryColor);
        drawArc(outerX, outerY + centerRadius / 16, centerRadius / 4, 30, 150, secondaryColor);
    }
}
