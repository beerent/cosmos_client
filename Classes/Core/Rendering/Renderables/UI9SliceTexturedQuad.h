//
//  UI9SliceTexturedQuad.h
//  Projectios2
//
//  Created by Kamil Konecko on 1/1/15.
//
//

#ifndef Projectios2_UI9SliceTexturedQuad_h
#define Projectios2_UI9SliceTexturedQuad_h

#include "Core/Rendering/Renderables/RenderableObj.h"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Interface.hpp"
#include "Core/Properties/PropertyCollection.h"
#include "glm/glm.hpp"
#include "Core/Rendering/TextureManager/Texture.h"
#include "Core/StringManager/StringManager.h"
#include <map>

class UI9SliceTexturedQuad : public RenderableObj
{
public:
    CONST_STRING_DEC(CLASS_ID_9_SLICE_TEXTURED_QUAD_RENDERABLE)
    CONST_STRING_DEC(TEXTURE_NAME)
    CONST_STRING_DEC(LEFT_BORDER_WIDTH_PIXEL)
    CONST_STRING_DEC(RIGHT_BORDER_WIDTH_PIXEL)
    CONST_STRING_DEC(TOP_BORDER_WIDTH_PIXEL)
    CONST_STRING_DEC(BOTTOM_BORDER_WIDTH_PIXEL)
    CONST_STRING_DEC(UV_LEFT)
    CONST_STRING_DEC(UV_RIGHT)
    CONST_STRING_DEC(UV_TOP)
    CONST_STRING_DEC(UV_BOTTOM)
    CONST_STRING_DEC(UV_LEFT_PIXEL)
    CONST_STRING_DEC(UV_RIGHT_PIXEL)
    CONST_STRING_DEC(UV_TOP_PIXEL)
    CONST_STRING_DEC(UV_BOTTOM_PIXEL)
    CONST_STRING_DEC(TEXTURE_WIDTH_PIXEL)
    CONST_STRING_DEC(TEXTURE_HEIGHT_PIXEL)
    CONST_STRING_DEC(BORDER_SCALE_MODIFIER)
    
public:
    
    UI9SliceTexturedQuad():RenderableObj(CLASS_ID_9_SLICE_TEXTURED_QUAD_RENDERABLE){};
    const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix);
    const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, glm::vec3 const &scale);
    
    void setResourceLoader(IResourceLoader *resourceLoader);
    virtual void _release();
    
    struct VertexData {
        GLfloat m_vertice[3];
        GLfloat m_textCoord[2];
    };
    
    virtual const void init(PropertyCollection const &propertyCollection);
    
private:
    struct BufferHandles
    {
        GLuint m_bufferHandle;
        
        BufferHandles():m_bufferHandle(0){}
    };
    
    static GLuint m_iboHandle;
    
    typedef unsigned int ScaleHash;
    ScaleHash getScaleHash(glm::vec3 const &scale);
    const BufferHandles& getIBOhandle(glm::vec3 const &scale);
    const BufferHandles& generateBuffer(glm::vec3 const &scale);
    
    typedef std::map<ScaleHash, BufferHandles> ScaleToBufferHandleMap;
    ScaleToBufferHandleMap m_scaleToBufferHandleMap;
    
    IResourceLoader *m_resourceLoader;
    Texture* m_texture;
    
    double uvTop;
    double uvBottom;
    double uvLeft;
    double uvRight;
    double scaleModifier;
    long pixelWidth;
    long pixelHeight;
    long uvTopPixels;
    long uvBottomPixels;
    long uvLeftPixels;
    long uvRightPixels;
    long leftBorderWidth;
    long rightBorderWidth;
    long topBorderWidth;
    long bottomBorderWidth;
};
#endif
