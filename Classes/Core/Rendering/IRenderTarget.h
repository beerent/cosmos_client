//
//  IRenderTarget.h
//  Projectios1
//
//  Created by Kamil Konecko on 2/19/14.
//
//

#ifndef Projectios1_IRenderTarget_h
#define Projectios1_IRenderTarget_h

#include "glm/glm.hpp"
#include "Core/StringManager/StringManager.h"

class Renderer;
class RenderableObj;

class IRenderTarget
{
public:
    virtual StringManager::StringID getRenderTargetID() = 0;
    virtual void onBeginRender(Renderer& renderer) = 0;
    virtual void onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView) = 0;
    
};

#endif
