//
//  ActiveCameraController.h
//  Projectios1
//
//  Created by Kamil Konecko on 3/23/13.
//
//

#ifndef Projectios1_ActiveCameraController_h
#define Projectios1_ActiveCameraController_h
#include "IEngine.h"
#include "Core/Input/InputManager.h"

class ActiveCameraController
{
public:
    ActiveCameraController(IEngine *iEngine);
    void onCameraForwardDown();
    void onCameraForwardUp();
    void onCameraBackwardDown();
    void onCameraBackwardUp();
    
    void onCameraUpPressed();
    void onCameraUpDepressed();
    void onCameraDownPressed();
    void onCameraDownDepressed();
    
    void onCameraLeftDown();
    void onCameraLeftUp();
    void onCameraRightDown();
    void onCameraRightUp();

    void onCameraRotateUpPressed();
    void onCameraRotateUpDepressed();
    void onCameraRotateDownPressed();
    void onCameraRotateDownDepressed();
    void onCameraRotateLeftPressed();
    void onCameraRotateLeftDepressed();
    void onCameraRotateRightPressed();
    void onCameraRotateRightDepressed();
 
    void onCameraRollRightPressed();
    void onCameraRollRightDepressed();
    void onCameraRollLeftPressed();
    void onCameraRollLeftDepressed();
    
    void release();

	virtual void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data);
	void setAllowUserControl(bool userControl) { m_allowUserControl = userControl; }

private:

	float CAMERA_MOVE_RATE_START = 0.6f;
	float CAMERA_MOVE_RATE_MAX = 60.0f;
	float CAMERA_MOVE_RATE_ACCELERATION = 2.5f;

	float CAMERA_ROTATION_RATE_START = 20.0f;
	float CAMERA_ROTATION_RATE_MAX = 100.0f;
	float CAMERA_ROTATION_RATE_ACCELERATION = 2.0f;

	IEngine* m_pIEngine;
	IEngine::onDeltaTimeEventCallBack m_onDeltaTimeCallBack;

	enum InputFlags { MOVE_LEFT = 0, MOVE_RIGHT, MOVE_FOWARD, MOVE_BACKWARD, MOVE_UP, MOVE_DOWN, ROTATE_LEFT, ROTATE_RIGHT, ROTATE_UP, ROTATE_DOWN, ROLL_LEFT, ROLL_RIGHT, INPUT_FLAG_COUNT };

	int m_inputFlags = 0;
	float m_deltaValues[INPUT_FLAG_COUNT] = { 0 };
	bool m_allowUserControl;

    void OnDeltaTimeUpdate(float deltaTime);
    float getAndUpdateMovementRate(enum InputFlags flag, float deltaTime);
    void resetMovementRate(enum InputFlags flag){m_deltaValues[flag] = 0;};
    float getAndUpdateRotationRate(enum InputFlags flag, float deltaTime);
    void resetRotationRate(enum InputFlags flag){m_deltaValues[flag] = 0;};

};
#endif
