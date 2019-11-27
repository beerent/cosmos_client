//
//  RenderableManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 12/18/12.
//
//

#include "Core/Rendering/RenderableManager.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/Rendering/Renderables/Model.h"
#include "Core/Rendering/Renderables/UITexturedQuadRenderable.h"
#include "Core/Rendering/Renderables/UIFontRenderable.h"
#include "Core/Rendering/Renderables/AxisRenderable.h"
#include "Core/Rendering/Renderables/LineRenderable.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/Rendering/Renderables/UI9SliceTexturedQuad.h"
#include <string>
#include <algorithm>

RenderableManager* RenderableManager::m_instance = NULL;

CONST_STRING_DEF(RenderableManager, TYPE_MODEL)
CONST_STRING_DEF(RenderableManager, UI_TEXTURED_QUAD)
CONST_STRING_DEF(RenderableManager, UI_FONT)
CONST_STRING_DEF(RenderableManager, TYPE_LINE)
CONST_STRING_DEF(RenderableManager, TYPE_AXIS)
CONST_STRING_DEF(RenderableManager, UI_9SLICE_TEXTURED_QUAD)

void RenderableManager::init(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
    RenderableManager::m_instance = this;
    m_charBuffer = new char [256];
}

RenderableObj* RenderableManager::getRenderable(StringManager::StringID name)
{
    //KKonecko: Move this to a better place. Perferably frame update.
    clean();
    
    RenderableObj *renderable = NULL;
    
    if(m_pRenderableMap.find(name) != m_pRenderableMap.end())
    {
        renderable =  m_pRenderableMap[name];
    }
    
    if(renderable == NULL)
    {
        renderable = readRenderable(name);
    }
    
    if(renderable == NULL)
    {
        renderable = getRenderable(StringManager::getIDForString("errorRenderable"));
    }
    
    if(renderable!= NULL)
    {
        renderable->m_refCount++;
        renderable->m_renderableType = name;
        renderable->m_managed = true;
    }
    
    return renderable;
}

RenderableObj* RenderableManager::readRenderable(StringManager::StringID name)
{
    std::string nameLowerStr = *StringManager::getStringFromID(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
    std::string path = m_resourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() + ".txt";
    
    std::ifstream file;
    file.open(path.c_str());
    
    PropertyCollection propertyCollection;
    
    if(file.good())
    {
        while(file.good())
        {
            file.getline(m_charBuffer, 256);
            //std::cout << m_charBuffer << std::endl;
            propertyCollection.addPropertyFromStream(m_charBuffer);
        }
        
        file.close();
    }
    else
    {
        Logger::Warning(Logger::LOADER, std::string("Failed to load " + *StringManager::getStringFromID(name)));
        return readRenderable(StringManager::getIDForString("errorRenderable"));
    }
    
    RenderableObj *pRenderable = NULL;
    
    //find type property
    PropertyCollection::Property const* property = NULL;
    
    if(propertyCollection.findPropertyByName(PropertyCollection::TYPE, &property))
    {
        StringManager::StringID typeName = property->m_data.StringIDValue;
        if( typeName == TYPE_MODEL)
        {
            if(name == StringManager::getIDForString("tempWaterRenderable"))
            {
                pRenderable = NULL;
            }
            
            pRenderable = new Model();
            pRenderable->init(propertyCollection);
            m_pRenderableMap[name] = pRenderable;
            return pRenderable;
        }
        else if( typeName == UI_TEXTURED_QUAD)
        {
            pRenderable = new UITexturedQuadRenderable();
            pRenderable->init(propertyCollection);
            m_pRenderableMap[name] = pRenderable;
            return pRenderable;
        }
        else if( typeName == UI_FONT)
        {
            pRenderable = new UIFontRenderable();
            pRenderable->init(propertyCollection);
            m_pRenderableMap[name] = pRenderable;
            return pRenderable;
        }
        else if( typeName == TYPE_LINE)
        {
            pRenderable = new LineRenderable();
            pRenderable->init(propertyCollection);
            m_pRenderableMap[name] = pRenderable;
            return pRenderable;
        }
        else if( typeName == TYPE_AXIS)
        {
            pRenderable = new AxisRenderable();
            pRenderable->init(propertyCollection);
            m_pRenderableMap[name] = pRenderable;
            return pRenderable;
        }
        else if( typeName == UI_9SLICE_TEXTURED_QUAD)
        {
            pRenderable = new UI9SliceTexturedQuad();
            pRenderable->init(propertyCollection);
            m_pRenderableMap[name] = pRenderable;
            return pRenderable;
        }
    }
    
    return NULL;
}

void RenderableManager::releaseRenderable(STRING_ID name)
{
    StringIDToRenderableMap::iterator it = m_pRenderableMap.find(name);
    
    if(it != m_pRenderableMap.end())
    {
        if(it->second->m_refCount == 0)
        {
            m_releasedRenderables[it->first] = it->second;
            m_pRenderableMap.erase(it);
        }
        else
        {
            Logger::Error("RenderableManger::releaseRenderable Tring to release renderable with an invalid ref count");
        }
    }
    else
    {
        Logger::Error("RenderableManger::releaseRenderable was unable to find renderable");
    }
}

void RenderableManager::clean()
{
    StringIDToRenderableMap::reverse_iterator it = m_releasedRenderables.rbegin();
    while(it != m_releasedRenderables.rend())
    {
        //KKonecko: this is causing some nasthy memory corruption on z renderables!
        delete(it->second);
        ++it;
    }
    
    m_releasedRenderables.clear();
}

void RenderableManager::release()
{
    StringIDToRenderableMap::iterator it = m_pRenderableMap.begin();
    while(it != m_pRenderableMap.end())
    {
        it->second->release();
        ++it;
    }
    
    delete [] m_charBuffer;
}

RenderableManager* RenderableManager::getInstance()
{
    return m_instance;
}
