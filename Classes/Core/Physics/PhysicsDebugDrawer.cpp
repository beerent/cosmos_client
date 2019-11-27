//
//  PhysicsDebugDrawer.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 2/19/14.
//
//

#include "Core/Physics/PhysicsDebugDrawer.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Renderables/LineRenderable.h"
#include "Core/Rendering/RenderableManager.h"
#include "Core/Debug/Logger/Logger.h"

#define PHYSICS_TO_WORLD_SCALE 1.0f

PhysicsDebugDrawer::PhysicsDebugDrawer()
{
    m_renderable = static_cast<LineRenderable*>(RenderableManager::getInstance()->getRenderable(StringManager::getIDForString("lineRenderable")));
    mRenderTargetID = StringManager::getIDForString("Physics Debug Drawer");
    mLinesWriten = 0;
}

void PhysicsDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
    /*
    sprintf(Logger::GetTempCharBuffer(), "PhysicsDebugDrawer::drawLine from x %f from y %f from z %f to x %f to y %f to z %f",
                                        from.x(), from.y(), from.z(),
                                        to.x(), to.y(), to.z());
    Logger::Log(Logger::MEMORY, Logger::GetTempCharBuffer());
    */
    
    if(mLinesWriten > PHYSICSDEBUGDRAWER_MAXLINES)
    {
        return;
    }
    
    LineRenderable::VertexData &lineStart = mLineData[mLinesWriten * 2];
    lineStart.m_vertice[0] = from.x() * PHYSICS_TO_WORLD_SCALE;
    lineStart.m_vertice[1] = from.y() * PHYSICS_TO_WORLD_SCALE;
    lineStart.m_vertice[2] = from.z() * PHYSICS_TO_WORLD_SCALE;

    lineStart.m_color[0] = color.x();
    lineStart.m_color[1] = color.y();
    lineStart.m_color[2] = color.z();
    
    LineRenderable::VertexData &lineEnd = mLineData[mLinesWriten * 2 + 1];
    
    lineEnd.m_vertice[0] = to.x()* PHYSICS_TO_WORLD_SCALE;
    lineEnd.m_vertice[1] = to.y()* PHYSICS_TO_WORLD_SCALE;
    lineEnd.m_vertice[2] = to.z()* PHYSICS_TO_WORLD_SCALE;
    
    lineEnd.m_color[0] = color.x();
    lineEnd.m_color[1] = color.y();
    lineEnd.m_color[2] = color.z();
    
    mLinesWriten++;
    
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
    /*
    sprintf(Logger::GetTempCharBuffer(), "PhysicsDebugDrawer::draw3dText textString %s", textString);
    Logger::Log(Logger::MEMORY, Logger::GetTempCharBuffer());
     */
}


void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
    /*
    sprintf(Logger::GetTempCharBuffer(), "PhysicsDebugDrawer::drawContactPoint");
    Logger::Log(Logger::MEMORY, Logger::GetTempCharBuffer());
    */
}
    
void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
    sprintf(Logger::GetTempCharBuffer(), "PhysicsDebugDrawer::reportErrorWarning warningString %s", warningString);
    Logger::Log(Logger::MEMORY, Logger::GetTempCharBuffer());
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
    
}

int	PhysicsDebugDrawer::getDebugMode() const
{
    /*
    return btIDebugDraw::DBG_DrawWireframe |
           btIDebugDraw::DBG_DrawContactPoints |
           btIDebugDraw::DBG_DrawText |
           btIDebugDraw::DBG_DrawConstraints |
            btIDebugDraw::DBG_DrawConstraintLimits |
            btIDebugDraw::DBG_DrawNormals;
     */
    
    return btIDebugDraw::DBG_DrawWireframe;
}


void PhysicsDebugDrawer::onBeginRender(Renderer& renderer)
{
    renderer.addRenderable(this, m_renderable);
}

void PhysicsDebugDrawer::onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView)
{
    //LineRenderable::VertexData buffer[PHYSICSDEBUGDRAWER_MAXLINES * 2];
    //int test = sizeof(buffer);
    //test = sizeof(GLfloat) * 6 * 2 * 1024;
    
    int linesToWrite = 2 * mLinesWriten;
    const int maxLines = 1332;// 32k limit
    LineRenderable::VertexData *writePtr = mLineData;
    LineRenderable::VertexData *endPtr = &mLineData[linesToWrite - 1];

    while(writePtr < endPtr)
    {
        if(linesToWrite > maxLines)
        {
            linesToWrite -= maxLines;
            m_renderable->onRender(renderer, modelViewProjectionMatrix, writePtr, sizeof(LineRenderable::VertexData) * maxLines);
            writePtr += maxLines;
        }
        else
        {
            m_renderable->onRender(renderer, modelViewProjectionMatrix, writePtr, sizeof(LineRenderable::VertexData) * linesToWrite);
            writePtr += linesToWrite;
        }
    }
    
    mLinesWriten = 0;
}
