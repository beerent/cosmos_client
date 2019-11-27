//
//  FontManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 9/17/13.
//
//

#ifndef Projectios1_FontManager_h
#define Projectios1_FontManager_h

#include <iostream>
#include <string>
#include <map>
#include "Font.h"
#include "Interface.hpp"
#include "Core/StringManager/StringManager.h"

class FontManager
{
        
public:
    static const int GLYMPH_COUNT_MAX;

private:
    
    IResourceLoader *m_resourceLoader;
    std::map<std::string, Font*> m_fonts;
    static FontManager * m_instance;
    
public:
    void init(IResourceLoader *resourceLoader);
    Font* getFont(std::string &&name)
    {
        return getFont(StringManager::getIDForString(name));
    }
    
    Font* getFont(STRING_ID name);
    void release();
    static FontManager* getInstance();
    
private:
    void readFontGlymphData(STRING_ID name, Font& font);
    void readGlymph(Font::Glymph *glymphs, char* buffer, size_t bufferLength);
};


#endif
