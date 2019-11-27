//
//  Font.h
//  Projectios1
//
//  Created by Kamil Konecko on 9/17/13.
//
//

#ifndef Projectios1_Font_h
#define Projectios1_Font_h
#include "Core/Memory/DynamicObject.h"
#include "Core/StringManager/StringManager.h"

class Font : public DynamicObject
{
public:
   
    friend class FontManager;
    
    static const StringManager::StringID CLASS_ID_FONT;
    
    struct Glymph
    {
        Glymph(): m_offsetX(0.0f), m_offsetY(0.0f), m_u(0.0f), m_v(0.0f), m_width(67.0f), m_height(0.0f){};
        float m_offsetX;
        float m_offsetY;
        float m_u;
        float m_v;
        float m_width;
        float m_height;
    };

    Font():DynamicObject(CLASS_ID_FONT){};
    
    const Glymph& getGlymph(const char & character)const {return m_glymphData[character];};
    float getWidthForString(const char* text)const;
    
protected:
    Glymph* getGlymphDataWriteable(){return &m_glymphData[0];};
    
private:
    Glymph m_glymphData[256];
   
    
};

#endif
