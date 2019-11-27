//
//  SkyBox.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 3/20/13.
//
//
#include "SkyBox.h"
#include "Core/Entity/Entities/Camera.h"
#include "IEngine.h"
#include "Core/Rendering/Renderer.h"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif

CONST_STRING_DEF(SkyBox, SKYBOX_TRACK_CAMERA_MODE);
CONST_STRING_DEF(SkyBox, TRACK_HORIZONTAL_AXIS);
CONST_STRING_DEF(SkyBox, TRACK_ALL_AXIS);

const StringManager::StringID SkyBox::CLASS_ID_SKYBOX = StringManager::getIDForString("CLASS_ID_SKYBOX");

const glm::mat4& SkyBox::getTransformation()
{
    IEngine *engine = IEngine::getEngine();
    Camera *cam = engine->getActiveCamera();
    glm::vec3 pos = cam->getPosition();
    
    if(mTrackingMode == TRACK_HORIZONTAL_AXIS)
    {
        pos.y  = this->getPosition().y;
    }
    
    SetPosition(pos);
    validateTransformationMatrix();
    return m_transformation;
}

void SkyBox::onBeginRender(Renderer& renderer)
{
    renderer.pushTransformation(getTransformation());
    //glDisable(GL_DEPTH_WRITEMASK);
    renderer.addRenderable(this, m_renderable);
    //glEnable(GL_DEPTH_WRITEMASK);
    renderer.popTransformation();
}

void SkyBox::init(PropertyCollection const &propertyCollection, Entity* parent)
{
    Entity::init(propertyCollection, parent);
    
    mTrackingMode = TRACK_HORIZONTAL_AXIS;
    propertyCollection.getValueForProperty(SKYBOX_TRACK_CAMERA_MODE, mTrackingMode);
}
