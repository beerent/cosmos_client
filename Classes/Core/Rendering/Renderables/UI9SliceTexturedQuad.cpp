//
//  UI9SliceTexturedQuad.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 1/1/15.
//
//

#include "UI9SliceTexturedQuad.h"
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
#include "Core/Debug/Logger/Logger.h"

CONST_STRING_DEF(UI9SliceTexturedQuad, CLASS_ID_9_SLICE_TEXTURED_QUAD_RENDERABLE)
CONST_STRING_DEF(UI9SliceTexturedQuad, TEXTURE_NAME)
CONST_STRING_DEF(UI9SliceTexturedQuad, LEFT_BORDER_WIDTH_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, RIGHT_BORDER_WIDTH_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, TOP_BORDER_WIDTH_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, BOTTOM_BORDER_WIDTH_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, UV_LEFT)
CONST_STRING_DEF(UI9SliceTexturedQuad, UV_RIGHT)
CONST_STRING_DEF(UI9SliceTexturedQuad, UV_TOP)
CONST_STRING_DEF(UI9SliceTexturedQuad, UV_BOTTOM)
CONST_STRING_DEF(UI9SliceTexturedQuad, UV_LEFT_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, UV_RIGHT_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, UV_TOP_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, UV_BOTTOM_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, TEXTURE_WIDTH_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, TEXTURE_HEIGHT_PIXEL)
CONST_STRING_DEF(UI9SliceTexturedQuad, BORDER_SCALE_MODIFIER)

GLuint UI9SliceTexturedQuad::m_iboHandle = 0;

const void UI9SliceTexturedQuad::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix)
{
    onRender(renderer, modelViewProjectionMatrix, glm::vec3(1.0, 1.0, 1.0));
}

const void UI9SliceTexturedQuad::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, glm::vec3 const &scale)
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
    
    const BufferHandles &bufferHandles = getIBOhandle(scale);
    
    glBindBuffer(GL_ARRAY_BUFFER, bufferHandles.m_bufferHandle);
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
                          sizeof(UI9SliceTexturedQuad::VertexData),             // no extra data between each position
                          (GLvoid*) offsetof(struct UI9SliceTexturedQuad::VertexData, m_vertice)                // pointer to the C array
                          );
    Renderer::validate();
    
    
    glEnableVertexAttribArray(attributeTextureCoordinate);
    glVertexAttribPointer(
                          attributeTextureCoordinate, // attribute
                          2,                 // number of elements per vertex, here (x,y)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          sizeof(UI9SliceTexturedQuad::VertexData),                 // no extra data between each position
                          (GLvoid*) offsetof(struct UI9SliceTexturedQuad::VertexData, m_textCoord)                  // pointer to the C array
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

void UI9SliceTexturedQuad::_release()
{
    RenderableObj::_release();
    ScaleToBufferHandleMap::iterator it = m_scaleToBufferHandleMap.begin();
    ScaleToBufferHandleMap::iterator itEnd = m_scaleToBufferHandleMap.end();

    while(it != itEnd)
    {
        glDeleteBuffers(1, &it->second.m_bufferHandle);
        it++;
    }
    m_scaleToBufferHandleMap.clear();
}

UI9SliceTexturedQuad::ScaleHash UI9SliceTexturedQuad::getScaleHash(glm::vec3 const &scale)
{
    ScaleHash hash = static_cast<unsigned int>(scale.x * 10000) << 16;
    hash |= static_cast<unsigned int>(scale.y * 10000);
    return hash;
}

void UI9SliceTexturedQuad::setResourceLoader(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
}

const UI9SliceTexturedQuad::BufferHandles& UI9SliceTexturedQuad::getIBOhandle(glm::vec3 const &scale)
{
    ScaleHash hash = getScaleHash(scale);
    ScaleToBufferHandleMap::const_iterator it = m_scaleToBufferHandleMap.find(hash);
    if(it != m_scaleToBufferHandleMap.end())
    {
        return it->second;
    }
    
    return  generateBuffer(scale);
}

const UI9SliceTexturedQuad::BufferHandles& UI9SliceTexturedQuad::generateBuffer(glm::vec3 const &scale)
{
    //Calculate width percentages
    GLfloat topBorderWidthPercent        = (double) topBorderWidth / (double) pixelHeight;
    GLfloat leftBorderWidthPercent       = (double) leftBorderWidth / (double) pixelWidth;
    GLfloat bottomBorderWidthPercent     = (double) bottomBorderWidth / (double) pixelHeight;
    GLfloat rightBorderWidthPercent      = (double) rightBorderWidth / (double) pixelWidth;
    
    //Source mappings
    GLfloat uvLeftBorder                 = uvLeft + leftBorderWidthPercent;
    GLfloat uvCenterBorder               = uvRight - rightBorderWidthPercent;
    
    GLfloat uvTopBorder                  = uvTop - topBorderWidthPercent;
    GLfloat uvMiddleBorder               = uvBottom + bottomBorderWidthPercent;
    
    
    //this is a magic number!!! Get rid of it
    GLfloat vectorBorderScalerHorizontal = 1.0f / scale.x * scaleModifier;
    GLfloat vectorBorderScalerVertical   = 1.0f / scale.y * scaleModifier;
    

    
    //Target mappings
    GLfloat vectorLeftBorder             = (GLfloat) leftBorderWidth  * vectorBorderScalerHorizontal;
    GLfloat vectorCenterBorder           = 1.0f - (GLfloat) rightBorderWidth  * vectorBorderScalerHorizontal;
    
    GLfloat vectorTopBorder              = (GLfloat) topBorderWidth   * vectorBorderScalerVertical;
    GLfloat vectorMiddleBorder           = 1.0f - (GLfloat)(topBorderWidth + bottomBorderWidth) * vectorBorderScalerVertical + vectorTopBorder;
    

    
    //m_scaleToBufferHandleMap.insert(std::pair<ScaleHash, BufferHandles>(getScaleHash(scale), BufferHandles()));
    ScaleHash hash = getScaleHash(scale);
    BufferHandles bufferHandles;
    
    struct UI9SliceTexturedQuad::VertexData vertexData3d[] = {
        
        /*
         {{  1.0, 0.0,0.0}, {uvRight, uvTop}},
         {{  0.0, 0.0,0.0}, {uvLeft, uvTop}},
         {{  0.0, 1.0,0.0}, {uvLeft, uvBottom}},
         {{  1.0, 1.0,0.0}, {uvRight, uvBottom}}
         */
        
        //Left Upper Corner
        {{  vectorLeftBorder, 0.0,0.0},                 {static_cast<GLfloat>(uvLeftBorder),  static_cast<GLfloat>(uvTop)}},
        {{  0.0, 0.0,0.0},                              {static_cast<GLfloat>(uvLeft),        static_cast<GLfloat>(uvTop)}},
        {{  0.0, vectorTopBorder,0.0},                  {static_cast<GLfloat>(uvLeft),        static_cast<GLfloat>(uvTopBorder)}},
        {{  vectorLeftBorder, vectorTopBorder,0.0},     {static_cast<GLfloat>(uvLeftBorder),  static_cast<GLfloat>(uvTopBorder)}}
        ,
        //Left middle Corner
        {{  vectorLeftBorder, vectorTopBorder,0.0},     {static_cast<GLfloat>(uvLeftBorder),  static_cast<GLfloat>(uvTopBorder)}},
        {{  0.0, vectorTopBorder,0.0},                  {static_cast<GLfloat>(uvLeft),        static_cast<GLfloat>(uvTopBorder)}},
        {{  0.0, vectorMiddleBorder,0.0},               {static_cast<GLfloat>(uvLeft),        static_cast<GLfloat>(uvMiddleBorder)}},
        {{  vectorLeftBorder, vectorMiddleBorder,0.0},  {static_cast<GLfloat>(uvLeftBorder),  static_cast<GLfloat>(uvMiddleBorder)}}
        ,
        //Left bottom Corner
        {{  vectorLeftBorder, vectorMiddleBorder,0.0},  {static_cast<GLfloat>(uvLeftBorder),  static_cast<GLfloat>(uvMiddleBorder)}},
        {{  0.0, vectorMiddleBorder,0.0},               {static_cast<GLfloat>(uvLeft),        static_cast<GLfloat>(uvMiddleBorder)}},
        {{  0.0, 1.0, 0.0},                             {static_cast<GLfloat>(uvLeft),        static_cast<GLfloat>(uvBottom)}},
        {{  vectorLeftBorder, 1.0 ,0.0},                {static_cast<GLfloat>(uvLeftBorder),  static_cast<GLfloat>(uvBottom)}}
        ,
        //center Upper Corner
        {{  vectorCenterBorder, 0.0,0.0},                 {uvCenterBorder,  static_cast<GLfloat>(uvTop)}},
        {{  vectorLeftBorder, 0.0,0.0},                   {uvLeftBorder,    static_cast<GLfloat>(uvTop)}},
        {{  vectorLeftBorder, vectorTopBorder,0.0},       {uvLeftBorder,    uvTopBorder}},
        {{  vectorCenterBorder, vectorTopBorder,0.0},     {uvCenterBorder,  uvTopBorder}}
        ,
        //center middle Corner
        {{  vectorCenterBorder, vectorTopBorder,0.0},     {uvCenterBorder,  uvTopBorder}},
        {{  vectorLeftBorder, vectorTopBorder,0.0},       {uvLeftBorder,    uvTopBorder}},
        {{  vectorLeftBorder, vectorMiddleBorder,0.0},    {uvLeftBorder,    uvMiddleBorder}},
        {{  vectorCenterBorder, vectorMiddleBorder,0.0},  {uvCenterBorder,  uvMiddleBorder}}
        ,
        //center bottom Corner
        {{  vectorCenterBorder, vectorMiddleBorder,0.0},  {uvCenterBorder,  uvMiddleBorder}},
        {{  vectorLeftBorder, vectorMiddleBorder,0.0},    {uvLeftBorder,    uvMiddleBorder}},
        {{  vectorLeftBorder, 1.0, 0.0},                  {uvLeftBorder,    static_cast<GLfloat>(uvBottom)}},
        {{  vectorCenterBorder, 1.0 ,0.0},                {uvCenterBorder,  static_cast<GLfloat>(uvBottom)}}
        ,
        //right Upper Corner
        {{  1.0, 0.0,0.0},                                {static_cast<GLfloat>(uvRight),         static_cast<GLfloat>(uvTop)}},
        {{  vectorCenterBorder, 0.0,0.0},                 {uvCenterBorder,  static_cast<GLfloat>(uvTop)}},
        {{  vectorCenterBorder, vectorTopBorder,0.0},     {uvCenterBorder,  uvTopBorder}},
        {{  1.0, vectorTopBorder,0.0},                    {static_cast<GLfloat>(uvRight),         uvTopBorder}}
        ,
        //Right middle Corner
        {{  1.0, vectorTopBorder,0.0},                    {static_cast<GLfloat>(uvRight),         uvTopBorder}},
        {{  vectorCenterBorder, vectorTopBorder,0.0},     {uvCenterBorder,  uvTopBorder}},
        {{  vectorCenterBorder, vectorMiddleBorder,0.0},  {uvCenterBorder,  uvMiddleBorder}},
        {{  1.0, vectorMiddleBorder,0.0},                 {static_cast<GLfloat>(uvRight),         uvMiddleBorder}}
        ,
        //Left bottom Corner
        {{  1.0, vectorMiddleBorder,0.0},                 {static_cast<GLfloat>(uvRight),         uvMiddleBorder}},
        {{  vectorCenterBorder, vectorMiddleBorder,0.0},  {uvCenterBorder,  uvMiddleBorder}},
        {{  vectorCenterBorder, 1.0, 0.0},                {uvCenterBorder,  static_cast<GLfloat>(uvBottom)}},
        {{  1.0, 1.0 ,0.0},                               {static_cast<GLfloat>(uvRight),         static_cast<GLfloat>(uvBottom)}}
    };
    
    glGenBuffers(1, &bufferHandles.m_bufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, bufferHandles.m_bufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData3d), vertexData3d, GL_STATIC_DRAW);

    int error = glGetError();
    
    if (error != 0) {
        Logger::Error("OPEN GL ERROR");
    }
    
    //KKonecko: Only generate the index bufferonce since we do not need it to be unique.
    if(m_iboHandle == 0)
    {
        GLushort cubeIndexes[] = {
            // front
            0,  1,  2,
            2,  3,  0
            ,
            4,  5,  6,
            6,  7,  4
            ,
            8,  9, 10,
            10, 11, 8
            ,
            12, 13,14,
            14, 15,12
            ,
            16, 17,18,
            18, 19,16
            ,
            20, 21,22,
            22, 23,20
            ,
            24, 25,26,
            26, 27,24
            ,
            28, 29,30,
            30, 31,28
            ,
            32, 33,34,
            34, 35,32
        };
        
        glGenBuffers(1, &m_iboHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexes), cubeIndexes, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    if(m_scaleToBufferHandleMap.size() > 20)
    {
        //We need a deffered clean up pump or someother mechanism to clean up unused buffers.
        Logger::Warning(Logger::MEMORY, "m_scaleToBufferHandleMap has excessivbe buffers!");
    }
    
    m_scaleToBufferHandleMap[hash] = bufferHandles;
    return m_scaleToBufferHandleMap[hash];
}

const void UI9SliceTexturedQuad::init(PropertyCollection const &propertyCollection)
{
    RenderableObj::init(propertyCollection);
    
    StringManager::StringID textureName;
    uvTop            = 0.0f;
    uvBottom         = 1.0f;
    uvLeft           = 0.0f;
    uvRight          = 1.0f;
    pixelWidth          = -1;
    pixelHeight         = -1;
    uvTopPixels         = -1;
    uvBottomPixels      = -1;
    uvLeftPixels        = -1;
    uvRightPixels       = -1;
    leftBorderWidth     = -1;
    rightBorderWidth    = -1;
    topBorderWidth      = -1;
    bottomBorderWidth   = -1;
    scaleModifier       = 1.0;
    
    propertyCollection.getValueForProperty(TEXTURE_NAME, textureName);
    propertyCollection.getValueForProperty(UV_TOP, uvTop);
    propertyCollection.getValueForProperty(UV_BOTTOM, uvBottom);
    propertyCollection.getValueForProperty(UV_LEFT, uvLeft);
    propertyCollection.getValueForProperty(UV_RIGHT, uvRight);
    propertyCollection.getValueForProperty(UV_TOP_PIXEL, uvTopPixels);
    propertyCollection.getValueForProperty(UV_BOTTOM_PIXEL, uvBottomPixels);
    propertyCollection.getValueForProperty(UV_LEFT_PIXEL, uvLeftPixels);
    propertyCollection.getValueForProperty(UV_RIGHT_PIXEL, uvRightPixels);
    propertyCollection.getValueForProperty(TEXTURE_WIDTH_PIXEL, pixelWidth);
    propertyCollection.getValueForProperty(TEXTURE_HEIGHT_PIXEL, pixelHeight);
    propertyCollection.getValueForProperty(LEFT_BORDER_WIDTH_PIXEL, leftBorderWidth);
    propertyCollection.getValueForProperty(RIGHT_BORDER_WIDTH_PIXEL, rightBorderWidth);
    propertyCollection.getValueForProperty(TOP_BORDER_WIDTH_PIXEL, topBorderWidth);
    propertyCollection.getValueForProperty(BOTTOM_BORDER_WIDTH_PIXEL, bottomBorderWidth);
    propertyCollection.getValueForProperty(BORDER_SCALE_MODIFIER, scaleModifier);
    
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
}
