//
//  UIFontRenderable.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 9/29/13.
//
//

#include "Core/Rendering/Renderables/UIFontRenderable.h"
#include <iostream>
#include <cstdio>
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Shaders/ShaderManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "Core/OpenSource/Vector.hpp"
#include "Core/Rendering/TextureManager/TextureManager.h"
#include "Core/Rendering/TextureManager/Texture.h"
#include "IEngine.h"

CONST_STRING_DEF(UIFontRenderable, CLASS_ID_FONT_RENDERABLE)
CONST_STRING_DEF(UIFontRenderable, TEXTURE_NAME)
CONST_STRING_DEF(UIFontRenderable, FONT_NAME)
CONST_STRING_DEF(UIFontRenderable, FONT_3D);

GLuint UIFontRenderable::m_iboHandle = 0;
const int UIFontRenderable::CLEAN_UP_INTERVAL_FRAMES = 5;
const int UIFontRenderable::MAX_LIFE_TIME_FRAMES_STRING_BUFFER = 10;

struct VertexData
{
    GLfloat m_vertice[3];
    GLfloat m_color[3];
};

const void UIFontRenderable::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix)
{
    onRender(renderer, modelViewProjectionMatrix, glm::vec3(1.0, 1.0, 1.0), "Error", glm::vec4(1.0, 0.0, 0.0, 1.0));
}

const void UIFontRenderable::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, glm::vec3 const &scale, const char* text, glm::vec4 const &color, float xOffset, float yOffset)
{
	if (*text == NULL)
	{
		return;
	}

    RenderableObj::onRender(renderer, modelViewProjectionMatrix);
    
    const UIFontRenderable::StringVertexBuffer *stringVertexBuffer = getBufferForString(text);
    glDeleteBuffers(1, &m_bufferHandle);
    glDeleteBuffers(1, &m_iboHandle);
    
    glGenBuffers(1, &m_bufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UIFontRenderable::VertexData) * 4 * stringVertexBuffer->mCount, stringVertexBuffer->mVertexBuffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &m_iboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * stringVertexBuffer->mCount, stringVertexBuffer->mIndexBuffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glm::mat4 labelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(xOffset, yOffset, 0.0f));
    labelTransform = modelViewProjectionMatrix * labelTransform;
    labelTransform = glm::scale(labelTransform, scale);
    
    GLuint program = ShaderManager::getProgram(ShaderManager::FONT_SHADER);
    
    const char* attribute_name = "coord3d";
    const char* attributeTransformName = "mMVP";
    const char* attributeTextureCoordinateName = "mTexCord";
    const char* attributeTextureName = "mytexture";
    const char* attributeUniformColorName = "vColor";
    
    GLint attributeCoord2d = glGetAttribLocation(program, attribute_name);
    GLint attributeTransform = glGetUniformLocation(program, attributeTransformName);
    GLint attributeTextureCoordinate = glGetAttribLocation(program, attributeTextureCoordinateName);
    GLint attributeTexture = glGetUniformLocation(program, attributeTextureName);
    GLint attributeUniformColor = glGetUniformLocation(program, attributeUniformColorName);
    
    if (attributeTransform == -1)
    {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeTransformName << std::endl;
        return;
    }
    
    if (attributeTextureCoordinate == -1)
    {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeTextureCoordinateName << std::endl;
        return;
    }
    
    if (attributeCoord2d == -1)
    {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attribute_name << std::endl;
        return;
    }
    
    if (attributeTexture == -1)
    {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeTextureName << std::endl;
        return;
    }

    if (attributeUniformColor == -1)
    {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeUniformColor << std::endl;
        return;
    }
    
    glUseProgram(program);
    
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->getTextureGLID());
    glUniform1i(attributeTexture,0);
    
    glEnableVertexAttribArray(attributeCoord2d);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboHandle);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    
    Renderer::validate();
    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
                          attributeCoord2d, // attribute
                          3,// number of elements per vertex, here (x,y)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          sizeof(UIFontRenderable::VertexData),             // no extra data between each position
                          (GLvoid*) offsetof(struct UIFontRenderable::VertexData, m_vertice)                // pointer to the C array
                          );
    Renderer::validate();
    
    
    glEnableVertexAttribArray(attributeTextureCoordinate);
    glVertexAttribPointer(
                          attributeTextureCoordinate, // attribute
                          2,                 // number of elements per vertex, here (x,y)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          sizeof(UIFontRenderable::VertexData),                 // no extra data between each position
                          (GLvoid*) offsetof(struct UIFontRenderable::VertexData, m_textCoord)                  // pointer to the C array
                          );
    
    glUniformMatrix4fv(attributeTransform, 1, GL_FALSE, glm::value_ptr(labelTransform));
    glUniform4f(attributeUniformColor, color.x, color.y, color.z, color.w);
    
    GLint size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT,0);
    
    glDisableVertexAttribArray(attributeCoord2d);
    glDisableVertexAttribArray(attributeTextureCoordinate);
    
    
    Renderer::validate();
}

void UIFontRenderable::_release()
{
    RenderableObj::_release();
    glDeleteBuffers(1, &m_bufferHandle);
    
    CharToBufferMap::iterator it =  mCharToBufferMap.begin();
    CharToBufferMap::iterator end = mCharToBufferMap.end();
    
    while(it != end)
    {
        delete it->second;
        it++;
    }
    
    IEngine::getEngine()->unregisterForDeltaTimeEvents(mOnDeltaTimeCallBack);
}

void UIFontRenderable::setResourceLoader(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
}

const void UIFontRenderable::init(PropertyCollection const &propertyCollection)
{
    RenderableObj::init(propertyCollection);
    m_font3d = false;
    
    STRING_ID textureName;
    STRING_ID fontName;
    
    int numberOfProperties = propertyCollection.getPropertyCount();
    for(int i=0; i < numberOfProperties; ++i)
    {
        const PropertyCollection::Property *property = propertyCollection.getPropertyAtIndex(i);
        if(property->m_name == TEXTURE_NAME)
        {
            textureName = property->m_data.StringIDValue;
        }
        else if(property->m_name == FONT_NAME)
        {
            fontName = property->m_data.StringIDValue;
        }
        else if(property->m_name == FONT_3D)
        {
            m_font3d = property->m_data.intValue == 1 ? true : false;
        }
    }
    
    m_texture = TextureManager::getInstance()->getTexture(textureName);
    m_font = FontManager::getInstance()->getFont(fontName);
    
    mOnDeltaTimeCallBack.bind(this, &UIFontRenderable::OnDeltaTimeUpdate);
    IEngine::getEngine()->registerForDeltaTimeEvents(mOnDeltaTimeCallBack);
}

const UIFontRenderable::StringVertexBuffer * UIFontRenderable::getBufferForString(const char* text)
{
    CharToBufferMap::iterator it = mCharToBufferMap.find(text);

    GLfloat yPolarity = m_font3d == true ? -1 : 1;
    
    if(it == mCharToBufferMap.end())
    {
        std::string textString(text);
        
        int vertexIndex = 0;
        int charCount = textString.length();
        float charSpacing = 1.0f / charCount;
        
        UIFontRenderable::VertexData *vertexData3d = new UIFontRenderable::VertexData[charCount * 4];
        UIFontRenderable::StringVertexBuffer *stringVertexBuffer = new StringVertexBuffer();
        
        stringVertexBuffer->mVertexBuffer = vertexData3d;
        stringVertexBuffer->mCount = charCount;
        
        
        mCharToBufferMap[text] = stringVertexBuffer;
        
        for(int charIndex = 0; charIndex < charCount; charIndex ++)
        {
            const Font::Glymph *glymph = &m_font->getGlymph(text[charIndex]);
       
            float padding = glymph->m_width / 20.0f;
            
            //TOP RIGHT
            vertexData3d[vertexIndex].m_vertice[0] = charSpacing + charSpacing * charIndex;
            vertexData3d[vertexIndex].m_vertice[1] = 0.0f;
            vertexData3d[vertexIndex].m_vertice[2] = 0.0f;
            
            vertexData3d[vertexIndex].m_textCoord[0] = glymph->m_u + glymph->m_width - padding/2.0f;
            vertexData3d[vertexIndex].m_textCoord[1] = glymph->m_v + padding/2.0f;
            vertexIndex++;
            
            //TOP LEFT
            vertexData3d[vertexIndex].m_vertice[0] = charSpacing * charIndex;
            vertexData3d[vertexIndex].m_vertice[1] = 0.0f;
            vertexData3d[vertexIndex].m_vertice[2] = 0.0f;
            
            vertexData3d[vertexIndex].m_textCoord[0] = glymph->m_u + padding/2.0f;
            vertexData3d[vertexIndex].m_textCoord[1] = glymph->m_v + padding/2.0f;
            vertexIndex++;
            
            //BOTTOM LEFT
            vertexData3d[vertexIndex].m_vertice[0] = charSpacing * charIndex;
            vertexData3d[vertexIndex].m_vertice[1] = 1.0f * yPolarity;
            vertexData3d[vertexIndex].m_vertice[2] = 0.0f;
            
            vertexData3d[vertexIndex].m_textCoord[0] = glymph->m_u + padding/2.0f;
            vertexData3d[vertexIndex].m_textCoord[1] = glymph->m_v - glymph->m_height + padding/2.0f;
            vertexIndex++;
            
            //BOTTOM RIGHT
            vertexData3d[vertexIndex].m_vertice[0] = charSpacing + charSpacing * charIndex;
            vertexData3d[vertexIndex].m_vertice[1] = 1.0f * yPolarity;
            vertexData3d[vertexIndex].m_vertice[2] = 0.0f;
            
            vertexData3d[vertexIndex].m_textCoord[0] = glymph->m_u + glymph->m_width - padding/2.0f;
            vertexData3d[vertexIndex].m_textCoord[1] = glymph->m_v - glymph->m_height + padding/2.0f;
            vertexIndex++;
            
        }
        
        GLushort *cubeIndexes = new GLushort[charCount * 6];
        
        for(int charIndex = 0; charIndex < charCount; charIndex++)
        {
            int currentOffset = charIndex * 6;
            int currentIdexOffset = charIndex * 4;
            cubeIndexes[currentOffset + 0] = currentIdexOffset + 0;
            cubeIndexes[currentOffset + 1] = currentIdexOffset + 1;
            cubeIndexes[currentOffset + 2] = currentIdexOffset + 2;
            cubeIndexes[currentOffset + 3] = currentIdexOffset + 2;
            cubeIndexes[currentOffset + 4] = currentIdexOffset + 3;
            cubeIndexes[currentOffset + 5] = currentIdexOffset + 0;
        }
        
        stringVertexBuffer->mIndexBuffer = cubeIndexes;
        stringVertexBuffer->mLastFrameCount = IEngine::getEngine()->getCurrentFrameCount();
        
        return stringVertexBuffer;
        
    }
    
    it->second->mLastFrameCount = IEngine::getEngine()->getCurrentFrameCount();

    return it->second;
}

void UIFontRenderable::OnDeltaTimeUpdate(float deltatime)
{
    const int currentFrameCount = IEngine::getEngine()->getCurrentFrameCount();
    if(currentFrameCount - mLastCleanUpFrame > CLEAN_UP_INTERVAL_FRAMES)
    {
        CharToBufferMap::iterator it  = mCharToBufferMap.begin();
        CharToBufferMap::iterator end = mCharToBufferMap.end();
        while(it != end)
        {
            StringVertexBuffer* strVertexBuffer = it->second;
            if(currentFrameCount -  strVertexBuffer->mLastFrameCount > MAX_LIFE_TIME_FRAMES_STRING_BUFFER)
            {
                delete(strVertexBuffer);
                mCharToBufferMap.erase(it++);
            }
            else
            {
                ++it;
            }
        }
        
        mLastCleanUpFrame = currentFrameCount;
    }
    
}
