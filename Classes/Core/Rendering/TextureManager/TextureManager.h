//
//  TextureManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 11/26/12.
//
//

#ifndef Projectios1_TextureManager_h
#define Projectios1_TextureManager_h

#include <iostream>
#include <string>
#include <map>
#include "Core/Rendering/TextureManager/Texture.h"
#include "Interface.hpp"
#include "Core/StringManager/StringManager.h"

class TextureManager
{ 
public:
private:
    IResourceLoader *m_resourceLoader;
    std::map<std::string, Texture*> m_textures;
    static TextureManager * m_instance;

public:
    void init(IResourceLoader *resourceLoader);
    Texture* getTexture(std::string &&name)
    {
        return getTexture(StringManager::getIDForString(name));
    }
    
    Texture* getTexture(StringManager::StringID name);
    void release();
    static TextureManager* getInstance();
};

#endif
