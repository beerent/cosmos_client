//
//  Renderables.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 7/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Core/Rendering/Renderables/RenderableObj.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/Rendering/RenderableManager.h"
#include <stdio.h>

CONST_STRING_DEF(RenderableObj, RENDER_GROUP);
CONST_STRING_DEF(RenderableObj, NORMAL);
const StringManager::StringID RenderableObj::TRANSPARENT_STR = StringManager::getIDForString("TRANSPAREN");
CONST_STRING_DEF(RenderableObj, BACKGROUND);

RenderableObj::RenderableObj(STRING_ID type)
{
    m_refCount = 0;
    m_classID = type;
    m_renderableType = 0;
    m_managed = false;
    m_assetName = StringManager::UNDEFINED;
    mRenderGroup = NORMAL;
}

const void RenderableObj::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix)
{
}

const void RenderableObj::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView)
{
    onRender(renderer, modelViewProjectionMatrix);
}

const void RenderableObj::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, glm::vec3 const &scale)
{
    onRender(renderer, modelViewProjectionMatrix);
}

const void RenderableObj::init(PropertyCollection const &propertyCollection)
{
    m_initialized = true;
    int numberOfProperties = propertyCollection.getPropertyCount();
    for(int i=0; i < numberOfProperties; ++i)
    {
        const PropertyCollection::Property *property = propertyCollection.getPropertyAtIndex(i);
        if(property->m_name == RENDER_GROUP)
        {
            mRenderGroup = property->m_data.StringIDValue;
            if(mRenderGroup != NORMAL &&
               mRenderGroup != TRANSPARENT_STR  &&
               mRenderGroup != BACKGROUND)
            {
                std::string warningText = StringManager::getStringFromID(m_assetName)->c_str();
                //Logger::Warning(Logger::LOADER, warningText + " has an invalid render group type! Forcing Normal");
                mRenderGroup = NORMAL;
            }
        }
    }
}

void RenderableObj::release()
{
    sprintf(Logger::GetTempCharBuffer(), "RenderableObj::release classID %s renderableType %s refCount %d", ID_TO_CSTR(m_classID), ID_TO_CSTR(m_renderableType), m_refCount);
    Logger::Log(Logger::MEMORY, Logger::GetTempCharBuffer());
    
    if(m_initialized == false)
    {
        Logger::Error("Releaseing an uninitialized renderable");
    }
    
    if(m_refCount > 0)
    {
        m_refCount--;
    }
    
    if(m_refCount == 0)
    {
        _release();
    }
    else if(m_refCount < 0)
    {
        Logger::Error("Releaseing a renderable with a negative ref count");
    }
}

void RenderableObj::_release()
{
    sprintf(Logger::GetTempCharBuffer(), "*** RenderableObj::_release classID %s renderableType %s refCount %d managed %d", ID_TO_CSTR(m_classID), ID_TO_CSTR(m_renderableType), m_refCount, m_managed);
    Logger::Log(Logger::MEMORY, Logger::GetTempCharBuffer());
    
    if(m_managed == true)
    {
        RenderableManager::getInstance()->releaseRenderable(m_renderableType);
    }
    
    m_initialized = false;
}
