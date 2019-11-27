//
//  LineRenderable.h
//  Projectios1
//
//  Created by Kamil Konecko on 11/25/13.
//
//

#ifndef Projectios1_LineRenderable_h
#define Projectios1_LineRenderable_h

#include "RenderableObj.h"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include"Interface.hpp"
#include "Core/Properties/PropertyCollection.h"
#include "Core/StringManager/StringManager.h"

class LineRenderable : public RenderableObj
{
public:
    CONST_STRING_DEC(CLASS_ID_LINE_RENDERABLE)
    
    LineRenderable():RenderableObj(CLASS_ID_LINE_RENDERABLE){};

    struct VertexData {
        GLfloat m_vertice[3];
        GLfloat m_color[3];
    };
    
    static const VertexData XYZ_ARROWS[];
    
    void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, const VertexData *lineData, size_t lineDataSize);

protected:
private:
};

#endif
