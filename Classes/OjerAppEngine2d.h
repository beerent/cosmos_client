#pragma once

#include "Interface.hpp"
#include "IOjerAppEngine2d.hpp"

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include <cmath>
#include <iostream>
#include <string>
#include "Core/Input/InputManager.h"
#include "Core/OpenSource/FastDelegate.h"
#include "Core/Entity/Entities/Primative.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Renderables/RenderableObj.h"
#include "Core/Rendering/Renderables/TutTexturedRenderable.h"
#include "Core/Rendering/TextureManager/TextureManager.h"
#include "Core/Rendering/Renderables/Model.h"
#include "Core/Rendering/MeshManager/MeshManager.h"
#include "Core/StringManager/StringManager.h"
#include "Core/Rendering/RenderableManager.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/LevelFactory/Level.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/Entity/Entities/Camera.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "IEngine.h"
#include "Core/Input/ActiveCameraController.h"
#include "Core/Memory/DynamicObjectTrackerManager.h"
#include "Core/Rendering/FontManager/FontManager.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/GameState/GameStateMachine.h"
#include "Core/GameState/States/GameMainMenuState.h"
#include "Core/GUI/Widgets/DebugMenuWidget.h"
#include "Core/GUI/Widgets/DebugOverLayWidget.h"
#include "Core/Rendering/MaterialManager/MaterialManager.h"
#include "Core/Rendering/LightingManager/LightingManager.h"
#include "Core/Events/EventDispatcher.h"
#include "Core/Events/GameEvents.h"
#include "ResourceLoader.h"
#include "Core/Rendering/Shaders/ShaderManager.h"
#include "Core/Debug/DebugLogic.h"
#include "Core/Entity/EntityFactory.h"
#include <sstream>
#include "Core/Generic/RunningAverage.h"
#include "Core/GameLogic/AI/NPCAI/NCPAIFactory.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/LUA/LuaScriptRepository.h"
#include <Core/Net/IRestConnector.h>
#include <Core/User/UserProvider.h>
#include <Core/User/IUserMemory.h>
#include <Core/Keyboard/KeyboardManager.h>
#include <Core/GameLogic/Authentication/Authenticator.h>
#include <Core/GUI/Widgets/User/UsernameWidget.h>

class OjerAppEngine2d : public IOjerAppEngine2d, public IEngine {

public:

	enum DragState { VIEW_DRAG_OFF, VIEW_DRAG_ON };
	enum GameState { MAIN_MENU, LEVEL_TEST_MODE };

	OjerAppEngine2d();
	void SetDimentions(int width, int height);
	void Initialize(int width, int height, IResourceLoader *resourceLoader, float scaleFactor);
	void Render();
	void OnDeltaTime(float timeStep);
	void OnRotate(DeviceOrientation newOrientation);
	void OnFingerUp(ivec2 oldLocation, ivec2 location);
	void OnFingerDown(ivec2 location);
	void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
	void OnKeyDown(const std::string& key);
	void OnKeyUp(const std::string& key);
	void OnInputEvent(InputManager::InputEvent event, InputManager::InputEventData data);

	void SetRestConnector(IRestConnector* restConnector);
    void SetUserMemory(IUserMemory* userMemory);

	float getLoopingAccumulatedDeltaTime();
	void registerForDeltaTimeEvents(onDeltaTimeEventCallBack callBack);
	void unregisterForDeltaTimeEvents(onDeltaTimeEventCallBack callBack);

	void registerForDeltaTimeEvents(IEventHandler* handler);
	void unregisterForDeltaTimeEvents(IEventHandler* handler);


	Camera* getActiveCamera();
	ActiveCameraController* getActiveCameraController();

	virtual UIComponent* getUIRoot();

	void ResetPhysics();
	unsigned long getCurrentFrameCount() const;
	void toggleDebugMenu();
	float getUIScale() const;
	float getCurrentAngle() const { return m_currentAngle; }
	LightingManager* getLightingManager() { return &mLightingManager; }
	IPhysicsManager* getPhysicsManager() { return m_physicsManager; }
	InputManager* getInputManager() { return &m_inputManager; }
	PropertyCollectionRepository& getArchetypeRepository();
	Lua::ScriptRepository& getLuaScriptRepository();
	IRestConnector* GetRestConnector();
    KeyboardManager* GetKeyboardManager();
    IUserMemory* GetUserMemory();
	UserProvider* GetUserProvider();
	NPCAIFactory& getNPCFactory();

	void DisplayUIBlockingComponent(UIComponent* component);
	void CompleteUIBlockingComponent();
    
    void AuthenticationResultReceived(Authenticator::AuthenticationResult result);
    void DisplayActiveUser();
    void TakeDownActiveUser();

private:

	std::list<IEngine::onDeltaTimeEventCallBack> m_onDeltatTimeEventListeners;
	float RotationDirection() const;

	void ApplyRotation(float degrees) const;
	float m_desiredAngle;
	float m_currentAngle;
	float m_rollingAccumulatedDeltaTime;
	Camera  *m_pActiveCamera;
	Renderer m_renderer;
	DragState m_dragState;
	ivec2 m_lastDragPosition;
	IResourceLoader* m_resourceLoader;
	TextureManager m_textureManager;
	MeshManager m_meshManager;
	FontManager m_fontManager;
	ShaderManager m_shaderManager;
	InputManager m_inputManager;
	RenderableManager m_renderableManager;
	EntityFactory m_entityFactory;
	LevelFactory *m_levelFactory;
	UIComponentFactory m_uiComponentFactory;
	ActiveCameraController* m_activeCameraController;
	PhysicsManager *m_physicsManager;
	MaterialManager mMaterialManager;
	RunningAverage* mRuningAverage;
	NPCAIFactory* mNPCAIFactory;
	PropertyCollectionRepository* mArchetypeRepository;
	Lua::ScriptRepository mLuaScriptRepository;
	IRestConnector* mRestConnector;
    KeyboardManager m_keyboardManager;
	UserProvider* m_userProvider;
    IUserMemory* m_userMemory;

	GameStateMachine *mGameStateMachine;

	LightingManager mLightingManager;
    Authenticator m_authenticator;

	float m_scaleFactor;
	float m_uiScale;

	UIComponent *m_pUIRoot;
	UIComponent *m_pUIGameLayerRoot;
	UIComponent *m_UIBlockingComponent;
	UIComponent *m_pUIDebugLayerRoot;

	DebugMenuWidget *mDebugMenuWidget;
	DebugOverLayWidget *mDebugOverLayWidget;
	UsernameWidget* m_usernameWidget;
	GameState mRequestedTransitionState;
	STRING_ID mRequestedTransitionLevel;
	bool mRequestPending;
	int m_currentInputID = 0;
	int m_activeInputCount = 0;
	unsigned long mFrameCount = 0;
	bool m_authenticationRequestActive;

	struct InputIDPosition
	{
		ivec2 m_position;
		int m_id;
	};

	std::vector<InputIDPosition> m_activeTuchInputs;

	EventDispatcher mDispatcher;

	void updateAccumulatedDeltaTime(float deltaTime);
    void LoginUserIfRemembered();

	int getInputID(ivec2 position);
	int findInputID(ivec2 position, int minDistance);
	void deleteInputID(int id);
	void updateInputID(ivec2 position, int id);

};
