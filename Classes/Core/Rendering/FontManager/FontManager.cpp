//
//  FontManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 9/17/13.
//
//

#include "Core/Rendering/FontManager/FontManager.h"
#include "Core/Debug/Logger/Logger.h"
#include <string>
#include <algorithm>

FontManager* FontManager::m_instance = NULL;
const int FontManager::GLYMPH_COUNT_MAX = 256;

void FontManager::init(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
    FontManager::m_instance = this;
}

Font* FontManager::getFont(StringManager::StringID name)
{
    
    Font *font = NULL;
    std::string &nameStr = *StringManager::getStringFromID(name);
    
    if(m_fonts.find(nameStr) != m_fonts.end())
    {
        return m_fonts[nameStr];
    }
    else
    {
        font = new Font();
        m_fonts[nameStr] = font;
        readFontGlymphData(name, *font);
    }
        
    return font;
}

void FontManager::readFontGlymphData(STRING_ID name, Font& font)
{
    std::string nameLowerStr = *ID_TO_STRING(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
    std::string path = m_resourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() +".txt";
    std::ifstream file;
    file.open(path.c_str());
    
    if(!file.good())
    {
        Logger::Error(std::string("Failed to load font: " + *StringManager::getStringFromID(name)));
        return;
    }
    
    char buffer[256];
    char *bufferPtr = &buffer[0];
    float width = -1.0f;
    float height = -1.0f;
    
    Font::Glymph * glymphData = font.getGlymphDataWriteable();
    
    while(file.good())
    {
        file.getline(bufferPtr, 256);
        //std::cout << m_charBuffer << std::endl;
        //Find
        char * pEnd = NULL;
        
        if(file.gcount() > 2)
        {
            switch (bufferPtr[0])
            {
                case 'W':
                    width = std::strtof(&bufferPtr[1], &pEnd);
                    break;
                case 'H':
                    height = std::strtof(&bufferPtr[1], &pEnd);
                    break;
                case 'F':
                    readGlymph(glymphData, bufferPtr, file.gcount());
                    break;
                default:
                    break;
            }
        }
    }
    
    font.init();    
    file.close();
    
    //Normalize glymph data
    for(int glymphIndex = 0; glymphIndex < GLYMPH_COUNT_MAX; glymphIndex++)
    {
        Font::Glymph &glymph = glymphData[glymphIndex];
        glymph.m_height /= height;
        glymph.m_width /= width;
        glymph.m_offsetX /= width;
        glymph.m_offsetY /= height;
        glymph.m_offsetY /= 1.0f - glymph.m_offsetY;
        glymph.m_u /= width;
        glymph.m_v /= height;
        glymph.m_v = 1.0f - glymph.m_v;
    }
}

void FontManager::readGlymph(Font::Glymph *glymphs, char* buffer, size_t bufferLength)
{
    //F ! ou 0 ov 0 u 0 v 0 w 67 h 137
    char *pStart = &buffer[2];
    char *pBufferEnd = buffer + bufferLength;
    char *pEnd = NULL;
    
    Font::Glymph *glymph = &glymphs[*pStart];
    pStart++;
    
    while(pStart < pBufferEnd)
    {
        switch (*pStart)
        {
            case ' ':
                pStart ++;
                break;
            case 'o':
                pStart ++;
                if(*pStart == 'u')
                {
                    pStart++;
                    glymph->m_offsetX = std::strtof(pStart, &pEnd);
                    pStart = pEnd;
                    
                }
                else if(*pStart == 'v')
                {
                    pStart++;
                    glymph->m_offsetY = std::strtof(pStart, &pEnd);
                    pStart = pEnd;                    
                }
                break;
            case 'u':
                pStart++;
                glymph->m_u = std::strtof(pStart, &pEnd);
                pStart = pEnd;
                break;
            case 'v':
                pStart++;
                glymph->m_v = std::strtof(pStart, &pEnd);
                pStart = pEnd;
                break;
            case 'w':
                pStart++;
                glymph->m_width = std::strtof(pStart, &pEnd);
                pStart = pEnd;
                break;
            case 'h':
                pStart++;
                glymph->m_height = std::strtof(pStart, &pEnd);
                pStart = pEnd;
                return;
        }
    }
}

void FontManager::release()
{
    std::map<std::string, Font*>::iterator it = m_fonts.begin();
    while(it != m_fonts.end())
    {
        it->second->release();
        ++it;
    }
}

FontManager* FontManager::getInstance()
{
    return m_instance;
}
