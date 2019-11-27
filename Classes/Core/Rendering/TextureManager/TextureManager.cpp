//
//  TextureManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 11/26/12.
//
//

#include "Core/Rendering/TextureManager/TextureManager.h"
#include "Core/Debug/Logger/Logger.h"
#include <string>
#include <algorithm>

TextureManager* TextureManager::m_instance = NULL;

void TextureManager::init(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
    TextureManager::m_instance = this;
}

Texture* TextureManager::getTexture(StringManager::StringID name)
{ 

    Texture *texture = NULL;
    
    std::string nameLowerStr = *ID_TO_STRING(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
    std::string &nameStr = nameLowerStr;
    
    if(m_textures.find(nameStr) != m_textures.end())
    {
        return m_textures[nameStr];
    }
    else
    {
        texture = new Texture();
        m_textures[nameStr] = texture;
    }
    
    int width;
    int height;
    int dataSize = -1;
    void *data = NULL;

    m_resourceLoader->loadPngImage(nameStr, width, height, &data, dataSize);

    if(dataSize == -1)
    {
        Logger::Warning(Logger::LOADER, std::string("Failed to load Image " + nameStr));
        return getTexture("errorModel");
    }

    if(!(
		width == 2 ||
		width == 4 ||
   	   width == 8 ||
       width == 16 ||
       width == 32 ||
       width == 64 ||
       width == 128 ||
       width == 256 ||
       width == 512 ||
       width == 1024 ||
       width == 2048 ||
	   height == 2 ||
	   height == 4 ||
       height == 8 ||
       height == 16 ||
       height == 32 ||
       height == 64 ||
       height == 128 ||
       height == 256 ||
       height == 512 ||
       height == 1024 ||
       height == 2048 ))
    {
        Logger::Warning(Logger::LOADER, std::string("Failed to load Image " + nameStr + " Dimentions need to be a power of two!"));
        return getTexture("errorModel");
    }
    
    texture->init(Texture::RGBA, (unsigned char *)data, dataSize, width, height);
    return texture;
}

void TextureManager::release()
{
    std::map<std::string, Texture*>::iterator it = m_textures.begin();
    while(it != m_textures.end())
    {
        it->second->release();
        ++it;
    }
}

TextureManager* TextureManager::getInstance()
{
    return m_instance;
}
