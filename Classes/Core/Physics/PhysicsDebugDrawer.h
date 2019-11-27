//
//  PhysicsDebugDrawer.h
//  Projectios1
//
//  Created by Kamil Konecko on 2/19/14.
//
//

#ifndef Projectios1_PhysicsDebugDrawer_h
#define Projectios1_PhysicsDebugDrawer_h
#include "btBulletDynamicsCommon.h"
#include "Core/Rendering/IRenderTarget.h"
#include "Core/StringManager/StringManager.h"
#include "Core/Rendering/Renderables/LineRenderable.h"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif

#define PHYSICSDEBUGDRAWER_MAXLINES 40480 * 10

class PhysicsDebugDrawer : public btIDebugDraw , public IRenderTarget
{
    
public:
    
    PhysicsDebugDrawer();
    
    virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
  	virtual void	draw3dText(const btVector3& location,const char* textString);
    virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
    virtual void	reportErrorWarning(const char* warningString);
	virtual void	setDebugMode(int debugMode);
	virtual int		getDebugMode() const;
    virtual void    onBeginRender(Renderer& renderer);
    virtual void    onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView);

    virtual StringManager::StringID getRenderTargetID(){return mRenderTargetID;}
    

    
private:
    LineRenderable* m_renderable;
    STRING_ID mRenderTargetID;

    LineRenderable::VertexData mLineData[PHYSICSDEBUGDRAWER_MAXLINES * 2];
    int mLinesWriten;
    
};

#endif
