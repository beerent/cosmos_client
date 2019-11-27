//
//  TutTexturedRenderable.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 11/1/12.
//
//

#include <iostream>
#include <cstdio>
#include "Core/Rendering/Renderables/UITexturedQuadRenderable.h"
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

CONST_STRING_DEF(UITexturedQuadRenderable, CLASS_ID_TEXTURED_QUAD_RENDERABLE)

const StringManager::StringID UITexturedQuadRenderable::TEXTURE_NAME = StringManager::getIDForString("TEXTURE_NAME");
const StringManager::StringID UITexturedQuadRenderable::UV_LEFT = StringManager::getIDForString("UV_LEFT");
const StringManager::StringID UITexturedQuadRenderable::UV_RIGHT = StringManager::getIDForString("UV_RIGHT");
const StringManager::StringID UITexturedQuadRenderable::UV_TOP = StringManager::getIDForString("UV_TOP");
const StringManager::StringID UITexturedQuadRenderable::UV_BOTTOM = StringManager::getIDForString("UV_BOTTOM");
const StringManager::StringID UITexturedQuadRenderable::UV_LEFT_PIXEL = StringManager::getIDForString("UV_LEFT_PIXEL");
const StringManager::StringID UITexturedQuadRenderable::UV_RIGHT_PIXEL = StringManager::getIDForString("UV_RIGHT_PIXEL");
const StringManager::StringID UITexturedQuadRenderable::UV_TOP_PIXEL = StringManager::getIDForString("UV_TOP_PIXEL");
const StringManager::StringID UITexturedQuadRenderable::UV_BOTTOM_PIXEL = StringManager::getIDForString("UV_BOTTOM_PIXEL");
const StringManager::StringID UITexturedQuadRenderable::TEXTURE_WIDTH_PIXEL = StringManager::getIDForString("TEXTURE_WIDTH_PIXEL");
const StringManager::StringID UITexturedQuadRenderable::TEXTURE_HEIGHT_PIXEL = StringManager::getIDForString("TEXTURE_HEIGHT_PIXEL");

struct VertexData {
    GLfloat m_vertice[3];
    GLfloat m_color[3];
};

GLuint UITexturedQuadRenderable::m_iboHandle = 0;

const void UITexturedQuadRenderable::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix)
{
    onRender(renderer, modelViewProjectionMatrix, glm::vec3(1.0, 1.0, 1.0));
}

const void UITexturedQuadRenderable::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, glm::vec3 const &scale)
{
    RenderableObj::onRender(renderer, modelViewProjectionMatrix);
        
    glm::mat4 m_transform = glm::scale(modelViewProjectionMatrix,scale);
    
	GLuint program = ShaderManager::getProgram(ShaderManager::TEXTURED_SHADER);
    
    const char* attribute_name = "coord3d";
    const char* attributeTransformName = "mMVP";
    const char* attributeTextureCoordinateName = "mTexCord";
    const char* attributeTextureName = "mytexture";
    
    GLuint attributeCoord2d = glGetAttribLocation(program, attribute_name);
    GLuint attributeTransform = glGetUniformLocation(program, attributeTransformName);
    GLuint attributeTextureCoordinate = glGetAttribLocation(program, attributeTextureCoordinateName);
    GLuint attributeTexture = glGetUniformLocation(program, attributeTextureName);
    
    if (attributeTransform == -1) {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeTransformName << std::endl;
        return;
    }
    
    
    if (attributeTextureCoordinate == -1) {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeTextureCoordinateName << std::endl;
        return;
    }
    
    if (attributeCoord2d == -1) {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attribute_name << std::endl;
        return;
    }
    
    if (attributeTexture == -1) {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeTextureName << std::endl;
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
                          sizeof(UITexturedQuadRenderable::VertexData),             // no extra data between each position
                          (GLvoid*) offsetof(struct UITexturedQuadRenderable::VertexData, m_vertice)                // pointer to the C array
                          );
    Renderer::validate();
    
    
    glEnableVertexAttribArray(attributeTextureCoordinate);
    glVertexAttribPointer(
                          attributeTextureCoordinate, // attribute
                          2,                 // number of elements per vertex, here (x,y)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          sizeof(UITexturedQuadRenderable::VertexData),                 // no extra data between each position
                          (GLvoid*) offsetof(struct UITexturedQuadRenderable::VertexData, m_textCoord)                  // pointer to the C array
                          );
    
    glUniformMatrix4fv(attributeTransform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    
    /* Push each element in buffer_vertices to the vertex shader */
     //glDrawArrays(GL_TRIANGLES, 0, 24);
    GLint size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT,0);
    
    glDisableVertexAttribArray(attributeCoord2d);
    glDisableVertexAttribArray(attributeTextureCoordinate);
    
    
    Renderer::validate();
}

void UITexturedQuadRenderable::_release()
{
    RenderableObj::_release();
    glDeleteBuffers(1, &m_bufferHandle);
}

void UITexturedQuadRenderable::setResourceLoader(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
}

const void UITexturedQuadRenderable::init(PropertyCollection const &propertyCollection)
{
    RenderableObj::init(propertyCollection);
    
    
    StringManager::StringID textureName;
    double uvTop = 0.0f;
    double uvBottom = 1.0f;
    double uvLeft = 0.0f;
    double uvRight = 1.0f;
    int pixelWidth = -1;
    int pixelHeight = -1;
    int uvTopPixels = -1;
    int uvBottomPixels = -1;
    int uvLeftPixels = -1;
    int uvRightPixels = -1;
    
    int numberOfProperties = propertyCollection.getPropertyCount();
    for(int i=0; i < numberOfProperties; ++i)
    {
        const PropertyCollection::Property *property = propertyCollection.getPropertyAtIndex(i);
        if(property->m_name == TEXTURE_NAME)
        {
            textureName = property->m_data.StringIDValue;
        }
        else if(property->m_name == UV_TOP)
        {
            uvTop = 1.0f - property->m_data.doubleValue;
        }
        else if(property->m_name == UV_BOTTOM)
        {
            uvBottom = 1.0f - property->m_data.doubleValue;
        }
        else if(property->m_name == UV_LEFT)
        {
            uvLeft = property->m_data.doubleValue;
        }
        else if(property->m_name == UV_RIGHT)
        {
            uvRight = property->m_data.doubleValue;
        }
        else if(property->m_name == UV_TOP_PIXEL)
        {
            uvTopPixels = property->m_data.intValue;
        }
        else if(property->m_name == UV_BOTTOM_PIXEL)
        {
            uvBottomPixels = property->m_data.intValue;
        }
        else if(property->m_name == UV_LEFT_PIXEL)
        {
            uvLeftPixels = property->m_data.intValue;
        }
        else if(property->m_name == UV_RIGHT_PIXEL)
        {
            uvRightPixels = property->m_data.intValue;
        }
        else if(property->m_name == TEXTURE_WIDTH_PIXEL)
        {
            pixelWidth = property->m_data.intValue;
        }
        else if(property->m_name == TEXTURE_HEIGHT_PIXEL)
        {
            pixelHeight = property->m_data.intValue;
        }
    }
    
    m_texture = TextureManager::getInstance()->getTexture(textureName);
    
    if(pixelHeight > 0 && pixelWidth > 0)
    {
        if (uvRightPixels)
        {
            uvRight = (double) uvRightPixels / (double) pixelWidth;
        }
        
        if (uvLeftPixels)
        {
            uvLeft = (double) uvLeftPixels / (double) pixelWidth;
        }
        
        if (uvTopPixels)
        {
            uvTop = 1.0f - ((double) uvTopPixels / (double) pixelHeight);
        }

        if (uvBottomPixels)
        {
            uvBottom = 1.0f - ((double) uvBottomPixels / (double) pixelHeight);
        }
    }
    
    struct UITexturedQuadRenderable::VertexData vertexData3d[] = {
        {{  1.0, 0.0,0.0}, {static_cast<GLfloat>(uvRight), static_cast<GLfloat>(uvTop)}},
        {{  0.0, 0.0,0.0}, {static_cast<GLfloat>(uvLeft), static_cast<GLfloat>(uvTop)}},
        {{  0.0, 1.0,0.0}, {static_cast<GLfloat>(uvLeft), static_cast<GLfloat>(uvBottom)}},
        {{  1.0, 1.0,0.0}, {static_cast<GLfloat>(uvRight), static_cast<GLfloat>(uvBottom)}}
    };
    
    glGenBuffers(1, &m_bufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData3d), vertexData3d, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
 
    //KKonecko: Only generate the index bufferonce since we do not need it to be unique.
    if(m_iboHandle == 0)
    {
        GLushort cubeIndexes[] = {
            // front
            0,  1,  2,
            2,  3,  0
        };

        glGenBuffers(1, &m_iboHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexes), cubeIndexes, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
