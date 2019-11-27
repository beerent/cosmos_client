//
//  Texture.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 11/26/12.
//
//

#include "Core/Rendering/TextureManager/Texture.h"
const StringManager::StringID Texture::CLASS_ID_TEXTURE = StringManager::getIDForString("CLASS_ID_TEXTURE");

void Texture::init(TextureFormat textureFormat, unsigned char *data, unsigned int size, unsigned int width, unsigned int height)
{
    m_data = new unsigned char [size];
    unsigned char * ptrData = m_data;
    unsigned char * ptrIncoming = data;
    m_width = width;
    m_height = height;
    
    for(unsigned int i = 0; i < size; ++i)
    {
        *ptrData = *ptrIncoming;
        ++ptrData;
        ++ptrIncoming;
    }
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 getWidth(),
                 getHeight(),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 getRawData());
    
    DynamicObject::init();
}

unsigned int Texture::getHeight()
{
    return m_height;
}

unsigned int Texture::getWidth()
{
    return m_width;
}

unsigned char* Texture::getRawData()
{
    return m_data;
}

void Texture::release()
{
    glDeleteTextures(1, &m_textureID);
    delete[] m_data;    
    DynamicObject::release();
}

