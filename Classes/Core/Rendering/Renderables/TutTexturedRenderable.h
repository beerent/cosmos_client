//
//  TutTexturedRenderable.h
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
#include "Core/StringManager/StringManager.h"

class TutTexturedRenderable : public RenderableObj
{
public:
    CONST_STRING_DEC(CLASS_ID_TUTTEXTURE_RENDERABLE)

    TutTexturedRenderable():RenderableObj(CLASS_ID_TUTTEXTURE_RENDERABLE){};
    const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix);
    void setResourceLoader(IResourceLoader *resourceLoader);
    virtual void _release();
    
    struct VertexData {
        GLfloat m_vertice[3];
        GLfloat m_textCoord[2];
    };

    virtual const void init(PropertyCollection const &propertyCollection);

private:
    bool m_tweenFwrd;
    GLfloat m_tweenPercentage;
    GLuint m_bufferHandle;
    GLuint m_iboHandle;
    GLuint m_textureID;
    GLint m_textureHandle;
    IResourceLoader *m_resourceLoader;
};
#endif
