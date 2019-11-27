#pragma once

#include "Core/Properties/PropertyCollection.h"
#include "Core/Rendering/Shaders/ShaderManager.h"
#include "glm/gtc/quaternion.hpp"
#include "Core/StringManager/StringManager.h"

class Renderer;
class RenderableManager;
class Entity;

class RenderableObj
{
public:

    CONST_STRING_DEC(RENDER_GROUP)
    CONST_STRING_DEC(NORMAL)
    CONST_STRING_DEC(BACKGROUND)
    
    RenderableObj(STRING_ID type);

	CONST_STRING_DEC( TRANSPARENT_STR)

    friend RenderableManager;
    
    virtual const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix);
    virtual const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView);
    virtual const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, glm::vec3 const &scale);

    virtual void release();
    virtual const void init(PropertyCollection const &propertyCollection);
    const STRING_ID getRenderGroup()const {return mRenderGroup;}
    
    STRING_ID getAssetName(){return m_assetName;};
    STRING_ID getType()const{return m_renderableType;}
    STRING_ID getClassID()const{return m_classID;}
    
protected:
    virtual void _release();
    STRING_ID m_assetName;
    
private:
    mutable bool m_initialized;
    int m_refCount;
    STRING_ID m_classID;
    STRING_ID m_renderableType;
    STRING_ID mRenderGroup;
    bool m_managed;
};
