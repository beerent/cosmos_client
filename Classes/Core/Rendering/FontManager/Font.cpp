//
//  Font.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 9/17/13.
//
//

#include "Font.h"

const STRING_ID Font::CLASS_ID_FONT = StringManager::getIDForString("CLASS_ID_FONT");

float Font::getWidthForString(const char* text)const
{
    float width = 0;
    const char* readPtr = text;
    while(*readPtr)
    {
        const Glymph &glymph = m_glymphData[*readPtr];
        if(glymph.m_height > 0.0f)
        {
            width += glymph.m_width / glymph.m_height;
        }
        readPtr++;
    }

    return width;
}