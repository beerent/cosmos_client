//
//  UITexturedQuadRenderable
//  Projectios1
//
//  Created by Kamil Konecko on 11/1/12.
//
//

#ifndef Projectios1_TutTexturedRenderable_h
#define Projectios1_TutTexturedRenderable_h

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

class UITexturedQuadRenderable : public RenderableObj
{
public:
    CONST_STRING_DEC(CLASS_ID_TEXTURED_QUAD_RENDERABLE)
    
private:
    static const StringManager::StringID TEXTURE_NAME;
    static const StringManager::StringID UV_LEFT;
    static const StringManager::StringID UV_RIGHT;
    static const StringManager::StringID UV_TOP;
    static const StringManager::StringID UV_BOTTOM;
    static const StringManager::StringID UV_LEFT_PIXEL;
    static const StringManager::StringID UV_RIGHT_PIXEL;
    static const StringManager::StringID UV_TOP_PIXEL;
    static const StringManager::StringID UV_BOTTOM_PIXEL;
    static const StringManager::StringID TEXTURE_WIDTH_PIXEL;
    static const StringManager::StringID TEXTURE_HEIGHT_PIXEL;
    
public:
    
    UITexturedQuadRenderable():RenderableObj(CLASS_ID_TEXTURED_QUAD_RENDERABLE){};
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
    GLuint m_bufferHandle;
    static GLuint m_iboHandle;
    IResourceLoader *m_resourceLoader;
    Texture* m_texture;
};
#endif
