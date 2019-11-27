///
//  RenderableManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 12/18/12.
//
//

#ifndef Projectios1_RenderableManager_h
#define Projectios1_RenderableManager_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Interface.hpp"
#include "Core/OpenSource/Vector.hpp"
#include "Core/StringManager/StringManager.h"
#include "Core/Rendering/Renderables/RenderableObj.h"

class RenderableManager
{
public:
    
    CONST_STRING_DEC(TYPE_MODEL);
    CONST_STRING_DEC(UI_TEXTURED_QUAD);
    CONST_STRING_DEC(UI_FONT);
    CONST_STRING_DEC(TYPE_LINE);
    CONST_STRING_DEC(TYPE_AXIS);
    CONST_STRING_DEC(UI_9SLICE_TEXTURED_QUAD);

private:
    IResourceLoader *m_resourceLoader;
    typedef std::map<StringManager::StringID, RenderableObj*> StringIDToRenderableMap;

    StringIDToRenderableMap m_pRenderableMap;
    StringIDToRenderableMap m_releasedRenderables;
    
    static RenderableManager * m_instance;
    char *m_charBuffer;
    RenderableObj* readRenderable(StringManager::StringID name);
    
public:
    void init(IResourceLoader *resourceLoader);
    RenderableObj* getRenderable(STRING_ID name);
    void releaseRenderable(STRING_ID name);
    
    void clean();
    void release();
    static RenderableManager* getInstance();
};


#endif
