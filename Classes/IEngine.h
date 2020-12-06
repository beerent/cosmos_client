//
//  IEngine.h
//  Projectios1
//
//  Created by Kamil Konecko on 3/23/13.
//
//

#ifndef Projectios1_IEngine_h
#define Projectios1_IEngine_h

#include "Core/OpenSource/FastDelegate.h"
#include <Core/Net/IRestConnector.h>
#include <Core/User/UserProvider.h>
#include <Core/User/IUserMemory.h>
#include <Core/Keyboard/KeyboardManager.h>
#include <Core/GameLogic/Authentication/Authenticator.h>
#include <Core/GameLogic/Rate/IRateAppController.h>

class Camera;
class ActiveCameraController;
class UIComponent;
class LightingManager;
class IPhysicsManager;
class IEventHandler;
class InputManager;
class NPCAIFactory;
class PropertyCollectionRepository;
namespace Lua {
	class ScriptRepository;
}

class IEngine
{
protected:
    static IEngine *mp_instance;
    
public:
    static IEngine* getEngine(){return mp_instance;}
    typedef fastdelegate::FastDelegate1<float> onDeltaTimeEventCallBack;
    virtual Camera* getActiveCamera() = 0;
    virtual ActiveCameraController* getActiveCameraController() = 0;
	virtual float getLoopingAccumulatedDeltaTime() = 0;
    virtual void registerForDeltaTimeEvents(onDeltaTimeEventCallBack callBack)=0;
    virtual void unregisterForDeltaTimeEvents(onDeltaTimeEventCallBack callBack)=0;
    virtual void registerForDeltaTimeEvents(IEventHandler* handler) = 0;
    virtual void unregisterForDeltaTimeEvents(IEventHandler* handler) = 0;
	virtual UIComponent* getUIRoot() = 0;
    virtual void ResetPhysics() = 0;
    virtual unsigned long getCurrentFrameCount() const = 0;
    virtual void toggleDebugMenu() = 0;
    virtual float getUIScale() const = 0;
    virtual float getCurrentAngle() const = 0;
    virtual LightingManager* getLightingManager() = 0;
    virtual IPhysicsManager* getPhysicsManager() = 0;
	virtual InputManager* getInputManager() = 0;
	virtual NPCAIFactory& getNPCFactory() = 0;
	virtual PropertyCollectionRepository& getArchetypeRepository() = 0;
	virtual Lua::ScriptRepository& getLuaScriptRepository() = 0;
	virtual IRestConnector* GetRestConnector() = 0;
    virtual KeyboardManager* GetKeyboardManager() = 0;
	virtual UserProvider* GetUserProvider() = 0;
    virtual IRateAppController* GetRateAppController() = 0;
    virtual IUserMemory* GetUserMemory() = 0;

	virtual void OnAuthenticationResultReceived(AuthenticationResult result) = 0;
	
	virtual void DisplayUIBlockingComponent(UIComponent* component) = 0;
	virtual void CompleteUIBlockingComponent() = 0;
};

#endif
