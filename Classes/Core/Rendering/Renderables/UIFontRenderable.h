//
//  UIFontRenderable.h
//  Projectios1
//
//  Created by Kamil Konecko on 9/29/13.
//
//

#ifndef Projectios1_UIFontRenderable_h
#define Projectios1_UIFontRenderable_h

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
#include "Core/Rendering/FontManager/FontManager.h"
#include <map>
#include "Core/StringManager/StringManager.h"
#include "IEngine.h"

class UIFontRenderable : public RenderableObj
{
public:
    CONST_STRING_DEC(CLASS_ID_FONT_RENDERABLE)
    
private:
    CONST_STRING_DEC(TEXTURE_NAME);
    CONST_STRING_DEC(FONT_NAME);
    CONST_STRING_DEC(FONT_3D);
    
    static const int CLEAN_UP_INTERVAL_FRAMES;
    static const int MAX_LIFE_TIME_FRAMES_STRING_BUFFER;
    
public:
    
    UIFontRenderable():RenderableObj(CLASS_ID_FONT_RENDERABLE), mLastCleanUpFrame(0){};
    const Font* getFont()const {return m_font;};
    
    const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix);
    const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, glm::vec3 const &scale, const char* text, glm::vec4 const &color, float xOffset = 0.0f, float yOffset = 0.0f);
    
    void setResourceLoader(IResourceLoader *resourceLoader);
    virtual void _release();
    
    struct VertexData
    {
        GLfloat m_vertice[3];
        GLfloat m_textCoord[2];
    };
    
    struct StringVertexBuffer
    {
        UIFontRenderable::VertexData *mVertexBuffer;
        GLushort *mIndexBuffer;
        unsigned int mCount;
        unsigned int mLastFrameCount;
    };
    
    virtual const void init(PropertyCollection const &propertyCollection);
    
    typedef std::map<const char*, StringVertexBuffer*>  CharToBufferMap;
    
private:
    IEngine::onDeltaTimeEventCallBack mOnDeltaTimeCallBack;
    void OnDeltaTimeUpdate(float deltatime);
    
    const StringVertexBuffer * getBufferForString(const char* );
    CharToBufferMap mCharToBufferMap;
    GLuint m_bufferHandle;
    static GLuint m_iboHandle;
    IResourceLoader *m_resourceLoader;
    Texture* m_texture;
    Font *m_font;
    bool m_font3d;
    int mLastCleanUpFrame;
};
#endif
