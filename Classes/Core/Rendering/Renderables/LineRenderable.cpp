//
//  LineRenderable.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 11/25/13.
//
//

#include "Core/Rendering/Renderables/LineRenderable.h"
#include <iostream>
#include <cstdio>
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Shaders/ShaderManager.h"
#include "Core/OpenSource/Vector.hpp"

CONST_STRING_DEF(LineRenderable, CLASS_ID_LINE_RENDERABLE)

void LineRenderable::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, const VertexData *lineData,  size_t lineDataSize)
{
    RenderableObj::onRender(renderer, modelViewProjectionMatrix);
    
    GLuint program = ShaderManager::getProgram(ShaderManager::LINE_SHADER);
    
    const char* attribute_name = "coord3d";
    const char* attributeTransformName = "mMVP";
    const char* attributeColorName = "vColor";
    
    GLuint attributeCoord3d = glGetAttribLocation(program, attribute_name);
    GLuint attributeTransform = glGetUniformLocation(program, attributeTransformName);
    GLuint attributeColor = glGetAttribLocation(program, attributeColorName);
    
    if (attributeTransform == -1) {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeTransformName << std::endl;
        return;
    }
    
    if (attributeCoord3d == -1) {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attribute_name << std::endl;
        return;
    }
    
    if (attributeColor == -1) {
        std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeColorName << std::endl;
    }
    
    GLuint bufferHandle;
    
    
    glGenBuffers(1, &bufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
    glBufferData(GL_ARRAY_BUFFER, lineDataSize, lineData, GL_DYNAMIC_DRAW);
    
    glUseProgram(program);
    glEnableVertexAttribArray(attributeCoord3d);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    
    Renderer::validate();
    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glEnableVertexAttribArray(attributeCoord3d);
    glVertexAttribPointer(
                          attributeCoord3d, // attribute
                          3,// number of elements per vertex, here (x,y)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          sizeof(LineRenderable::VertexData),             // no extra data between each position
                          (GLvoid*) offsetof(struct LineRenderable::VertexData, m_vertice)                // pointer to the C array
                          );
    Renderer::validate();
    
    glEnableVertexAttribArray(attributeColor);
    glVertexAttribPointer(
                          attributeColor, // attribute
                          3,                 // number of elements per vertex, here (x,y)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          sizeof(LineRenderable::VertexData),                 // no extra data between each position
                          (GLvoid*) offsetof(struct LineRenderable::VertexData, m_color)                  // pointer to the C array
                          );
    
    Renderer::validate();

    glUniformMatrix4fv(attributeTransform, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

    GLint size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    GLint itemSize = sizeof(LineRenderable::VertexData);
    GLint itemCount = size/itemSize;

    Renderer::validate();
    
    //glDrawElements(GL_LINES, itemCount, GL_UNSIGNED_SHORT,0);
    glDrawArrays(GL_LINES, 0, itemCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(attributeCoord3d);
    glDeleteBuffers(1, &bufferHandle);
    
    Renderer::validate();
}
