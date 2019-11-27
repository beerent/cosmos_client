//
//  ActiveCameraController.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 3/23/13.
//
//

#include "Core/Input/ActiveCameraController.h"
#include "Core/Entity/Entities/Camera.h"
#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Core/Input/InputManager.h"

ActiveCameraController::ActiveCameraController(IEngine *iEngine) :m_allowUserControl(false)
{
    m_pIEngine = iEngine;
    m_onDeltaTimeCallBack.bind(this, &ActiveCameraController::OnDeltaTimeUpdate);
    m_pIEngine->registerForDeltaTimeEvents(m_onDeltaTimeCallBack);   

	InputManager::onInputEventCallBack callBack;
	callBack.bind(this, &ActiveCameraController::onInputEvent);
	m_pIEngine->getInputManager()->registerForInputEvents(callBack);
}

void ActiveCameraController::onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data)
{
	if (m_allowUserControl == false)
	{
		return;
	}

	if		(event == InputManager::ON_KEY_DOWN && data.getCharValue() == 'W') { onCameraForwardDown();}
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 'W') { onCameraForwardUp();}
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 'A') { onCameraLeftDown();}
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 'A') { onCameraLeftUp();}
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 'D') { onCameraRightDown();}
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 'D') { onCameraRightUp();}
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 'S') { onCameraBackwardDown(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 'S') { onCameraBackwardUp(); }
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 37) { onCameraRotateLeftPressed(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 37) { onCameraRotateLeftDepressed(); }
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 38) { onCameraRotateUpPressed(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 38) { onCameraRotateUpDepressed(); }
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 39) { onCameraRotateRightPressed(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 39) { onCameraRotateRightDepressed(); }
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 40) { onCameraRotateDownPressed(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 40) { onCameraRotateDownDepressed(); }
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 'R') { onCameraUpPressed(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 'R') { onCameraUpDepressed(); }
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 'F') { onCameraDownPressed(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 'F') { onCameraDownDepressed(); }
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 'Q') { onCameraRollLeftPressed(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 'Q') { onCameraRollLeftDepressed(); }
	else if (event == InputManager::ON_KEY_DOWN && data.getCharValue() == 'E') { onCameraRollRightPressed(); }
	else if (event == InputManager::ON_KEY_UP	&& data.getCharValue() == 'E') { onCameraRollRightDepressed(); }
}

void ActiveCameraController::OnDeltaTimeUpdate(float deltaTime)
{
    
	if (m_allowUserControl == false)
	{
		return;
	}

    if(m_inputFlags == 0)
    {
        //KKonecko: TODO: this does not need to happen every frame.
        for(int i = 0; i < INPUT_FLAG_COUNT; i++)
        {
            m_deltaValues[i] = 0;
        }
        
        return;
    }
    
    
    float moveRate = 0;
    float rotateRate = 0;
    
    Camera* camera = m_pIEngine->getActiveCamera();
    glm::vec3 pos = camera->getPosition();
    glm::quat rotation = camera->getRotation();
 
    //std::cout << "Current camera postion x " << pos.x << " y " << pos.y << " z " << pos.z << std::endl;
    
    glm::mat4 rotationMatrix = glm::toMat4(rotation);
    glm::vec4 rightVect = rotationMatrix[0];
    glm::vec4 upVect = rotationMatrix[1];
    glm::vec4 fwdVect = rotationMatrix[2];
    
    if(m_inputFlags & (1 << ROTATE_RIGHT))
    {
        rotateRate = getAndUpdateRotationRate(ROTATE_RIGHT, deltaTime);
        glm::quat newRotation = glm::angleAxis<float>(-rotateRate,upVect.x,upVect.y,upVect.z);
        rotation = newRotation * rotation;
    }
    else
    {
        resetRotationRate(ROTATE_RIGHT);
    }
    
    if(m_inputFlags & (1 << ROTATE_LEFT))
    {
        rotateRate = getAndUpdateRotationRate(ROTATE_LEFT, deltaTime);
        glm::quat newRotation = glm::angleAxis<float>(rotateRate,upVect.x,upVect.y,upVect.z);
        rotation = newRotation * rotation;
    }
    else
    {
        resetRotationRate(ROTATE_LEFT);
    }

    
    if(m_inputFlags & (1 << ROTATE_UP))
    {
        rotateRate = getAndUpdateRotationRate(ROTATE_UP, deltaTime);
        glm::quat newRotation = glm::angleAxis<float>(rotateRate,rightVect.x,rightVect.y,rightVect.z);
        rotation = newRotation * rotation;
    }
    else
    {
        resetRotationRate(ROTATE_UP);
    }
    
    if(m_inputFlags & (1 << ROTATE_DOWN))
    {
        rotateRate = getAndUpdateRotationRate(ROTATE_DOWN, deltaTime);
        glm::quat newRotation = glm::angleAxis<float>(-rotateRate,rightVect.x,rightVect.y,rightVect.z);
        rotation = newRotation * rotation;
    }
    else
    {
        resetRotationRate(ROTATE_DOWN);
    }

    
    if(m_inputFlags & (1 << ROLL_LEFT))
    {
        rotateRate = getAndUpdateRotationRate(ROLL_LEFT, deltaTime);
        glm::quat newRotation = glm::angleAxis<float>(rotateRate,fwdVect.x,fwdVect.y,fwdVect.z);
        rotation = newRotation * rotation;
    }
    else
    {
        resetRotationRate(ROLL_LEFT);
    }

    
    if(m_inputFlags & (1 << ROLL_RIGHT))
    {
        rotateRate = getAndUpdateRotationRate(ROLL_RIGHT, deltaTime);
        glm::quat newRotation = glm::angleAxis<float>(-rotateRate,fwdVect.x,fwdVect.y,fwdVect.z);
        rotation = newRotation * rotation;
    }
    else
    {
        resetRotationRate(ROLL_RIGHT);
    }

    
    glm::vec4 combinedDelta(0.0f, 0.0f, 0.0f, 0.0f);
    
    if(m_inputFlags & (1 << MOVE_FOWARD))
    {
        moveRate = getAndUpdateMovementRate(MOVE_FOWARD, deltaTime);
        combinedDelta += moveRate * -fwdVect;
    }
    else
    {
        moveRate = getAndUpdateMovementRate(MOVE_FOWARD, deltaTime);
        resetMovementRate(MOVE_FOWARD);
    }
    
    if(m_inputFlags & (1 << MOVE_BACKWARD))
    {
        moveRate = getAndUpdateMovementRate(MOVE_BACKWARD, deltaTime);
        combinedDelta += moveRate * fwdVect;
    }
    else
    {
        resetMovementRate(MOVE_BACKWARD);
    }

    if(m_inputFlags & (1 << MOVE_LEFT))
    {
        moveRate = getAndUpdateMovementRate(MOVE_LEFT, deltaTime);
        combinedDelta += moveRate * -rightVect;
    }
    else
    {
        resetMovementRate(MOVE_LEFT);
    }

    if(m_inputFlags & (1 << MOVE_RIGHT))
    {
        moveRate = getAndUpdateMovementRate(MOVE_RIGHT, deltaTime);
        combinedDelta += moveRate * rightVect;
    }
    else
    {
        resetMovementRate(MOVE_RIGHT);
    }
    
    if(m_inputFlags & (1 << MOVE_UP))
    {
        moveRate = getAndUpdateMovementRate(MOVE_UP, deltaTime);
        combinedDelta = moveRate * upVect;
    }
    else
    {
        resetMovementRate(MOVE_UP);
    }

    if(m_inputFlags & (1 << MOVE_DOWN))
    {
        moveRate = getAndUpdateMovementRate(MOVE_DOWN, deltaTime);
        combinedDelta += moveRate * -upVect;
    }
    else
    {
        resetMovementRate(MOVE_DOWN);
    }

    pos.x += combinedDelta.x;
    pos.y += combinedDelta.y;
    pos.z += combinedDelta.z;

    //std::cout << "New camera postion x " << pos.x << " y " << pos.y << " z " << pos.z << std::endl;
    
    camera->SetPosition(pos);
    camera->SetRotation( glm::normalize(rotation));
}

float ActiveCameraController::getAndUpdateMovementRate(enum InputFlags flag, float deltaTime)
{
    float currentRate = m_deltaValues[flag];
    if(currentRate < CAMERA_MOVE_RATE_START)
    {
        currentRate = CAMERA_MOVE_RATE_START;
    }
    else if (currentRate < CAMERA_MOVE_RATE_MAX)
    { 
        currentRate += currentRate * CAMERA_MOVE_RATE_ACCELERATION * deltaTime;
    }
    else
    {
        currentRate = CAMERA_MOVE_RATE_MAX;
    }
    
    m_deltaValues[flag] = currentRate;
    
    return currentRate * deltaTime;
}

float ActiveCameraController::getAndUpdateRotationRate(enum InputFlags flag, float deltaTime)
{
    float currentRate = m_deltaValues[flag];
    if(currentRate < CAMERA_ROTATION_RATE_START)
    {
        currentRate = CAMERA_ROTATION_RATE_START;
    }
    else if (currentRate < CAMERA_ROTATION_RATE_MAX)
    {
        currentRate += currentRate * CAMERA_ROTATION_RATE_ACCELERATION * deltaTime;
    }
    else
    {
        currentRate = CAMERA_ROTATION_RATE_MAX;
    }
    
    m_deltaValues[flag] = currentRate;
    
    return currentRate * deltaTime;
}

void ActiveCameraController::onCameraUpDepressed()
{
    m_inputFlags = m_inputFlags & ~(1 << MOVE_UP);
}

void ActiveCameraController::onCameraUpPressed()
{
    m_inputFlags = m_inputFlags | (1 << MOVE_UP);
}

void ActiveCameraController::onCameraDownDepressed()
{
    m_inputFlags = m_inputFlags & ~(1 << MOVE_DOWN);
}

void ActiveCameraController::onCameraDownPressed()
{
    m_inputFlags = m_inputFlags | (1 << MOVE_DOWN);
}

void ActiveCameraController::onCameraForwardDown()
{
    m_inputFlags = m_inputFlags | (1 << MOVE_FOWARD);
}

void ActiveCameraController::onCameraForwardUp()
{
    m_inputFlags = m_inputFlags & ~(1 << MOVE_FOWARD);
}

void ActiveCameraController::onCameraBackwardDown()
{
    m_inputFlags = m_inputFlags | (1 << MOVE_BACKWARD);
}

void ActiveCameraController::onCameraBackwardUp()
{
    m_inputFlags = m_inputFlags & ~(1 << MOVE_BACKWARD);
}

void ActiveCameraController::onCameraLeftDown()
{
    m_inputFlags = m_inputFlags | (1 << MOVE_LEFT);
}

void ActiveCameraController::onCameraLeftUp()
{
    m_inputFlags = m_inputFlags & ~(1 << MOVE_LEFT);
}

void ActiveCameraController::onCameraRightDown()
{
    m_inputFlags = m_inputFlags | (1 << MOVE_RIGHT);
}

void ActiveCameraController::onCameraRightUp()
{
    m_inputFlags = m_inputFlags & ~(1 << MOVE_RIGHT);
}

void ActiveCameraController::onCameraRotateUpPressed()
{
    m_inputFlags = m_inputFlags | (1 << ROTATE_UP);
}

void ActiveCameraController::onCameraRotateUpDepressed()
{
    m_inputFlags = m_inputFlags & ~(1 << ROTATE_UP);
}

void ActiveCameraController::onCameraRotateDownPressed()
{
    m_inputFlags = m_inputFlags | (1 << ROTATE_DOWN);
}

void ActiveCameraController::onCameraRotateDownDepressed()
{
    m_inputFlags = m_inputFlags & ~(1 << ROTATE_DOWN);
}

void ActiveCameraController::onCameraRotateLeftPressed()
{
    m_inputFlags = m_inputFlags | (1 << ROTATE_LEFT);
}

void ActiveCameraController::onCameraRotateLeftDepressed()
{
    m_inputFlags = m_inputFlags & ~(1 << ROTATE_LEFT);
}

void ActiveCameraController::onCameraRotateRightPressed()
{
    m_inputFlags = m_inputFlags | (1 << ROTATE_RIGHT);
}

void ActiveCameraController::onCameraRotateRightDepressed()
{
    m_inputFlags = m_inputFlags & ~(1 << ROTATE_RIGHT);
}

void ActiveCameraController::onCameraRollRightPressed()
{
    m_inputFlags = m_inputFlags | (1 << ROLL_RIGHT);
}

void ActiveCameraController::onCameraRollRightDepressed()
{
    m_inputFlags = m_inputFlags & ~(1 << ROLL_RIGHT);
}

void ActiveCameraController::onCameraRollLeftPressed()
{
    m_inputFlags = m_inputFlags | (1 << ROLL_LEFT);
}

void ActiveCameraController::onCameraRollLeftDepressed()
{
    m_inputFlags = m_inputFlags & ~(1 << ROLL_LEFT);
}

void ActiveCameraController::release()
{
    m_pIEngine->unregisterForDeltaTimeEvents(m_onDeltaTimeCallBack);
}