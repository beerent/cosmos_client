//
//  Texture.h
//  Projectios1
//
//  Created by Kamil Konecko on 11/26/12.
//
//

#ifndef Projectios1_Texture_h
#define Projectios1_Texture_h
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Core/Memory/DynamicObject.h"

class Texture : public DynamicObject
{
public:
    enum TextureFormat{RGBA};
    static const StringManager::StringID CLASS_ID_TEXTURE;
    Texture():DynamicObject(CLASS_ID_TEXTURE){};
    
private:
    unsigned char * m_data;
    TextureFormat m_type;
    unsigned int m_size;
    unsigned int m_width;
    unsigned int m_height;
    GLuint m_textureID;
    
public:
    void init(TextureFormat textureFormat, unsigned char *data, unsigned int size, unsigned int width, unsigned int height);
    unsigned char* getRawData();
    unsigned int getHeight();
    unsigned int getWidth();
    GLuint getTextureGLID(){return m_textureID;};
    
    void release();
private:
    
};
#endif
