//
//  DebugLogic.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/4/14.
//
//

#include "Core/Debug/DebugLogic.h"
#include "Core/GameLogic/ProjectileManager.h"
#include "IEngine.h"
#include "Core/Entity/Entities/Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Core/LevelFactory/LevelFactory.h"
#include <Core/OpenSource/FastDelegate.h>
#include <Core/Rendering/Shaders/ShaderManager.h>

DebugLogic* DebugLogic::mInstance = NULL;

DebugLogic::DebugLogic()
{
	InputManager::onInputEventCallBack callBack;
	callBack.bind(this, &DebugLogic::onInputEvent);
	IEngine::getEngine()->getInputManager()->registerForInputEvents(callBack);
}

void DebugLogic::onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data)
{
	if (event == InputManager::ON_KEY_UP && data.getCharValue() == 48)
	{ 
		reloadShaders();
	}
}

void DebugLogic::reloadShaders()
{
	ShaderManager::getInstance()->reloadShaders();
}

DebugLogic* DebugLogic::getInstance()
{
    if(mInstance == NULL)
    {
        mInstance = new DebugLogic();
    }
    
    return mInstance;
}

void DebugLogic::fireProjectileFromCamera(STRING_ID type)
{
    
    Entity* projectile = ProjectileManager::getInstance()->getProjectile(type);
    Camera* camera = IEngine::getEngine()->getActiveCamera();
    glm::vec3 pos = camera->getPosition();

    glm::quat rotation = camera->getRotation();
    
    //std::cout << "Current camera postion x " << pos.x << " y " << pos.y << " z " << pos.z << std::endl;
    
    glm::mat4 rotationMatrix = glm::toMat4(rotation);
    //glm::vec4 rightVect = rotationMatrix[0];
    //glm::vec4 upVect = rotationMatrix[1];
    glm::vec4 fwdVect = rotationMatrix[2];
    glm::vec3 fwdVec3 = glm::vec3(fwdVect.x, fwdVect.y, fwdVect.z);
    projectile->SetPosition(camera->getPosition() + fwdVec3 * -1.0f);
    projectile->setVelocity(fwdVec3 * -16.0f);
}