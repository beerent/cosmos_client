//
//  TutTexturedRenderable.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 11/1/12.
//
//

#include <iostream>
#include <cstdio>
#include "Core/Rendering/Renderables/TutTexturedRenderable.h"
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

CONST_STRING_DEF(TutTexturedRenderable, CLASS_ID_TUTTEXTURE_RENDERABLE)

struct VertexData {
    GLfloat m_vertice[3];
    GLfloat m_color[3];
};

const void TutTexturedRenderable::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix)
{
    RenderableObj::onRender(renderer, modelViewProjectionMatrix);
    
    if(m_tweenPercentage > 8)
    {
        m_tweenPercentage = 0;
    }
    else
    {
        m_tweenPercentage += 0.01f;
    }
    
    float angle = m_tweenPercentage/ 1.0 * 45;  // 45° per second
    glm::vec3 axis(1, 1, 0);
    glm::mat4 m_transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.4, 0.4, 0.4)) *
        glm::rotate(glm::mat4(1.0f), angle, axis) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    
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
    glBindTexture(GL_TEXTURE_2D, m_textureID);
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
                          sizeof(TutTexturedRenderable::VertexData),             // no extra data between each position
                          (GLvoid*) offsetof(struct TutTexturedRenderable::VertexData, m_vertice)                // pointer to the C array
                          );
    Renderer::validate();
    
    
    glEnableVertexAttribArray(attributeTextureCoordinate);
    glVertexAttribPointer(
                          attributeTextureCoordinate, // attribute
                          2,                 // number of elements per vertex, here (x,y)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          sizeof(TutTexturedRenderable::VertexData),                 // no extra data between each position
                          (GLvoid*) offsetof(struct TutTexturedRenderable::VertexData, m_textCoord)                  // pointer to the C array
                          );
    
    glUniformMatrix4fv(attributeTransform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    
    /* Push each element in buffer_vertices to the vertex shader */
//    glDrawArrays(GL_TRIANGLES, 0, 24);
    GLint size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT,0);
    
    glDisableVertexAttribArray(attributeCoord2d);
    glDisableVertexAttribArray(attributeTextureCoordinate);
    
    
    Renderer::validate();
}

void TutTexturedRenderable::_release()
{
    RenderableObj::_release();
    glDeleteBuffers(1, &m_bufferHandle);
    glDeleteTextures(1, &m_textureID);
}

void TutTexturedRenderable::setResourceLoader(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
}

const void TutTexturedRenderable::init(PropertyCollection const &propertyCollection)
{
    RenderableObj::init(propertyCollection);
    
    m_tweenFwrd = false;
    m_tweenPercentage = 0;

    
    Texture *texture = TextureManager::getInstance()->getTexture(std::string("testTexture.png"));
  
    //m_resourceLoader->loadPngImage("testTexture.png");
    //m_resourceLoader->loadPngImage("redBlack.png");
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 texture->getWidth(),
                 texture->getHeight(),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texture->getRawData());
    
    struct TutTexturedRenderable::VertexData vertexData3d[] = {
     //Front
        {{ -1.0, -1.0, 1.0}, {0.0, 0.0}},
        {{  1.0, -1.0, 1.0}, {1.0, 0.0}},
        {{  1.0,  1.0, 1.0}, {1.0, 1.0}},
        {{ -1.0,  1.0, 1.0}, {0.0, 1.0}},
     //Top
        {{ -1.0,  1.0, 1.0}, {0.0, 0.0}},
        {{  1.0,  1.0, 1.0}, {1.0, 0.0}},
        {{  1.0,  1.0,-1.0}, {1.0, 1.0}},
        {{ -1.0,  1.0,-1.0}, {0.0, 1.0}},
     //Back
        {{  1.0, -1.0,-1.0}, {0.0, 0.0}},
        {{ -1.0, -1.0,-1.0}, {1.0, 0.0}},
        {{ -1.0,  1.0,-1.0}, {1.0, 1.0}},
        {{  1.0,  1.0,-1.0}, {0.0, 1.0}},
     //Bottom
        {{ -1.0, -1.0,-1.0}, {0.0, 0.0}},
        {{  1.0, -1.0,-1.0}, {1.0, 0.0}},
        {{  1.0, -1.0, 1.0}, {1.0, 1.0}},
        {{ -1.0, -1.0, 1.0}, {0.0, 1.0}},
     //Left
        {{ -1.0, -1.0,-1.0}, {0.0, 0.0}},
        {{ -1.0, -1.0, 1.0}, {1.0, 0.0}},
        {{ -1.0,  1.0, 1.0}, {1.0, 1.0}},
        {{ -1.0,  1.0,-1.0}, {0.0, 1.0}},
     //Right
        {{  1.0, -1.0, 1.0}, {0.0, 0.0}},
        {{  1.0, -1.0,-1.0}, {1.0, 0.0}},
        {{  1.0,  1.0,-1.0}, {1.0, 1.0}},
        {{  1.0,  1.0, 1.0}, {0.0, 1.0}}
    };
    
    GLushort cubeIndexes[] = {
        // front
        0,  1,  2,
        2,  3,  0,
        // top
        4,  5,  6,
        6,  7,  4,
        // back
        8,  9, 10,
        10, 11,  8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20,
    };
    
    glGenBuffers(1, &m_bufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData3d), vertexData3d, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &m_iboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexes), cubeIndexes, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}