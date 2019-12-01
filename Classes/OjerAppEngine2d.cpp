#include <OjerAppEngine2d.h>
#include <Core/GUI/UIState/States/UIStateMainMenu.h>

static const float RevolutionsPerSecond = 1;
struct Vertex;
struct TVertex;
class TriangleRenderable;

void OjerAppEngine2d::registerForDeltaTimeEvents(IEventHandler* handler)
{
    mDispatcher.registerForEventType(GameEvents::GAME_DELTA_TIME_EVENT, handler);
}

void OjerAppEngine2d::unregisterForDeltaTimeEvents(IEventHandler* handler)
{
    mDispatcher.unRegisterForEventType(GameEvents::GAME_DELTA_TIME_EVENT, handler);
}

void OjerAppEngine2d::ResetPhysics()
{
    if(m_physicsManager != NULL)
    {
        delete m_physicsManager;
    }
    
    m_physicsManager = new PhysicsManager();
}

IOjerAppEngine2d* CreateRenderer()
{
    return new OjerAppEngine2d();
}

OjerAppEngine2d::OjerAppEngine2d() : m_authenticationRequestActive(false)
{
	m_pUIRoot = NULL;
	m_pUIGameLayerRoot = NULL;
	m_pUIDebugLayerRoot = NULL;

	InputManager::onInputEventCallBack callBack;
	callBack.bind(this, &OjerAppEngine2d::OnInputEvent);

	m_inputManager.registerForInputEvents(callBack);
	m_dragState = VIEW_DRAG_OFF;
	mRequestPending = false;
	m_scaleFactor = 0.0f;
	m_desiredAngle = 90.0f;
	m_currentAngle = 90.0f;

	m_userProvider = new UserProvider();
}

float OjerAppEngine2d::getUIScale() const
{
    return m_uiScale;
}

unsigned long OjerAppEngine2d::getCurrentFrameCount() const
{
    return mFrameCount;
}

UIComponent* OjerAppEngine2d::getUIRoot()
{
    return m_pUIGameLayerRoot;
}

ActiveCameraController* OjerAppEngine2d::getActiveCameraController()
{
    return m_activeCameraController;
};

Camera* OjerAppEngine2d::getActiveCamera()
{
    return m_pActiveCamera;
}

void OjerAppEngine2d::toggleDebugMenu()
{
    if(m_pUIDebugLayerRoot->getVisible())
    {
        m_pUIDebugLayerRoot->setVisible(false);
        m_pUIGameLayerRoot->setVisible(true);
    }
    else
    {
        m_pUIDebugLayerRoot->setVisible(true);
        m_pUIGameLayerRoot->setVisible(false);
    }
}

void OjerAppEngine2d::OnInputEvent(InputManager::InputEvent event, InputManager::InputEventData data)
{
    bool captured = false;

    //KKonecko: Hack Hard Coded screen orientation input remapping.
    ivec2 pos;
    if (m_desiredAngle == 90.0)
    {
        pos.x = (m_renderer.getViewPortHeight() - data.getPosition().y * m_scaleFactor) * m_uiScale;
        pos.y = (data.getPosition().x * m_scaleFactor) * m_uiScale;
    }
	else if(m_desiredAngle == 270.0)
	{
        pos.x = (data.getPosition().y * m_scaleFactor) * m_uiScale;
        pos.y = (m_renderer.getViewPortWidth() - data.getPosition().x * m_scaleFactor) * m_uiScale;
    }
	else
	{
		pos.x = (data.getPosition().x * m_scaleFactor) * m_uiScale;
		pos.y = (data.getPosition().y * m_scaleFactor) * m_uiScale;
	}

    data.setPosition(pos);
    
    switch(event)
    {
        case InputManager::ON_PRESS:
            m_pUIRoot->onInputEvent(event, data, captured);
            m_dragState = VIEW_DRAG_ON;
            m_lastDragPosition = pos;
            break;
        case InputManager::ON_RELEASE:
            m_dragState = VIEW_DRAG_OFF;
            break;
        case InputManager::ON_DRAG:
            if(m_dragState == VIEW_DRAG_ON)
            {
                ivec2 movePosition = pos;
                ivec2 moveDelta = movePosition - m_lastDragPosition; 
                m_lastDragPosition = movePosition;
                moveDelta.y *= -1;
            }
            break;
        default:
            break;
    }
    
    //std::cout << "OnInputEvent " << event << std::endl;
    return;
}

void OjerAppEngine2d::SetDimentions(int width, int height)
{
	//m_uiScale = 1.0f / ((width * m_scaleFactor) / 768.0f);
#ifdef __APPLE__
		float adjustedWidth = width * m_scaleFactor;
		float ratio = adjustedWidth / 768.0f;
		m_uiScale = 1.0f / ratio;
		m_renderer.SetViewPortSize(width * m_scaleFactor, height * m_scaleFactor);
#else
    //target 1024/768
		float targetWidth = 1024.0f;
		float targetHeigth = 768.0f;
		float widthRatio = width / targetWidth;
		float heightRatio = height / targetHeigth;
	
		float ratio = 1;

		if (widthRatio < heightRatio)
		{
			ratio = width  / targetWidth;
		}
		else
		{
			ratio = height / targetHeigth;
		}

		m_uiScale = 1.0f / ratio;
		m_renderer.SetViewPortSize(width * m_scaleFactor, height * m_scaleFactor);

		if (m_pUIRoot != NULL)
		{
			m_pUIRoot->setWidth(width * m_uiScale);
			m_pUIRoot->setHeight(height * m_uiScale);
			m_pUIGameLayerRoot->setWidth(width * m_uiScale);
			m_pUIGameLayerRoot->setHeight(height * m_uiScale);
			//m_pUIDebugLayerRoot->setWidth(width * m_uiScale);
			//m_pUIDebugLayerRoot->setHeight(height * m_uiScale);
		}
#endif
}

void OjerAppEngine2d::Initialize(int width, int height, IResourceLoader* resourceLoader, float scaleFactor)
{    
    IEngine::mp_instance = this;
    m_scaleFactor = scaleFactor;
	SetDimentions(width, height);    
    m_resourceLoader = resourceLoader;
	float uiScale = m_uiScale * m_scaleFactor;

	m_renderer.Initialize();
    //std::cout << "Resource Loader " << m_resourceLoader->getResourcePath() << std::endl;

    //m_stringManager.init();
    mMaterialManager.init(m_resourceLoader);
    m_meshManager.init(m_resourceLoader);
    m_textureManager.init(m_resourceLoader);
    m_renderableManager.init(m_resourceLoader);
    m_entityFactory.init(m_resourceLoader);
    m_uiComponentFactory.init(m_resourceLoader);
    m_fontManager.init(m_resourceLoader);
	m_shaderManager.init(m_resourceLoader);
	mLuaScriptRepository.init(m_resourceLoader);

    m_activeCameraController = new ActiveCameraController(this);
    m_pActiveCamera = dynamic_cast<Camera*>(m_entityFactory.createEntity(StringManager::getIDForString("cameraArchetype")));
    m_pActiveCamera->SetPosition(glm::vec3(0.0,0.0,15.0));
    glm::quat rotation = glm::angleAxis<float>(0,0.0,0.0,1.0);
    m_pActiveCamera->SetRotation(rotation);
    m_renderer.setActiveCamera(m_pActiveCamera);
    
    m_levelFactory = new LevelFactory();
    m_levelFactory->init(m_resourceLoader);
    
    m_pUIRoot = m_uiComponentFactory.createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_pUIRoot->setWidth(height * uiScale);
    m_pUIRoot->setHeight(width * uiScale);
    
    m_pUIGameLayerRoot = m_uiComponentFactory.createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_pUIGameLayerRoot->setWidth(height * uiScale);
    m_pUIGameLayerRoot->setHeight(width * uiScale);

    m_pUIDebugLayerRoot = m_uiComponentFactory.createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_pUIDebugLayerRoot->setWidth(height * uiScale);
    m_pUIDebugLayerRoot->setHeight(width * uiScale);
    
    
    m_pUIRoot->addChild(m_pUIGameLayerRoot);
    m_pUIRoot->addChild(m_pUIDebugLayerRoot);
    
    mGameStateMachine = GameStateMachine::GetInstance();
    mGameStateMachine->SetState(GameMainMenuState::MAIN_MENU_STATE);
    
    LoginUserIfRemembered();
    
    //mDebugMenuWidget = new DebugMenuWidget();
    //mDebugMenuWidget->init(&m_uiComponentFactory, m_pUIDebugLayerRoot);

    //mDebugOverLayWidget = new DebugOverLayWidget();
    //mDebugOverLayWidget->init(&m_uiComponentFactory, m_pUIRoot);

    //m_pUIDebugLayerRoot->setVisible(false);
    
    m_physicsManager = NULL;

	m_rollingAccumulatedDeltaTime = 0;
	mRuningAverage = new RunningAverage(100, 0.2);

	mArchetypeRepository = new PropertyCollectionRepository(*m_resourceLoader);
}

void OjerAppEngine2d::LoginUserIfRemembered() {
    const std::string username = GetUserMemory()->RetrieveUsername();
    const std::string password = GetUserMemory()->RetrievePassword();
    
    User user(username, password, UserAccessLevel::INVALID);
    m_authenticator.SetUser(user);
    Authenticator::AuthenticationResultListener callback;
    callback.bind(this, &OjerAppEngine2d::AuthenticationResultReceived);
    m_authenticator.RegisterAuthenticationResultListener(callback);
    m_authenticator.SetRestConnector(GetRestConnector());
    m_authenticator.SendAuthenticationRequest();
	m_authenticationRequestActive = true;
}

void OjerAppEngine2d::AuthenticationResultReceived(Authenticator::AuthenticationResult result) {
    if (Authenticator::AuthenticationResult::SUCCESS == result) {
		if (m_authenticationRequestActive) {
			m_authenticationRequestActive = false;
			//m_userProvider->SetUser(m_authenticator.GetUser());
			//m_userProvider->LogIn();
			mGameStateMachine->SetState(GameMainMenuState::MAIN_MENU_STATE);
		}

		m_usernameWidget = new UsernameWidget(&m_uiComponentFactory, m_pUIGameLayerRoot);
		m_usernameWidget->Init();
    }
}

void OjerAppEngine2d::DisplayUIBlockingComponent(UIComponent* component) {
	m_pUIGameLayerRoot->setVisible(false);
	m_UIBlockingComponent = component;
	m_pUIRoot->addChild(m_UIBlockingComponent);
}

void OjerAppEngine2d::CompleteUIBlockingComponent() {
	if (m_userProvider->IsLogOutQueued()) {
		m_userProvider->LogOut();
		m_usernameWidget->Release();
		mGameStateMachine->SetState(GameMainMenuState::MAIN_MENU_STATE);
	}

	m_UIBlockingComponent->setVisible(false);
	m_pUIGameLayerRoot->setVisible(true);
}

void OjerAppEngine2d::ApplyRotation(float degrees) const
{
}

void OjerAppEngine2d::Render() 
{
    //kkonecko: make sure we do layout!
    m_pUIRoot->onLayout();
    Level *currentLevel = LevelFactory::getActiveLevel();

    if(currentLevel != NULL)
    {
        //KKonecko: For now render everything, eventualy we will only collect renderables that are in view.
        Level::StrIDToEntityPtrMap& entities = currentLevel->getEntities();
        Level::StrIDToEntityPtrMap::iterator it = entities.begin();
        
        while(it != entities.end())
        {
            Entity *entity = it->second;
            m_renderer.AddRenderTarget(entity);
            ++it;
        }

        if(m_physicsManager != NULL)
        {
            m_renderer.AddRenderTarget(m_physicsManager->getPhysicsDebugDrawer());
        }
    }
    
    m_renderer.onBeginRender();
    
    //KKonecko: Now lets render UI!
    m_renderer.onBeginRenderUI(m_pUIRoot);
}

float OjerAppEngine2d::RotationDirection() const
{
    float delta = m_desiredAngle - m_currentAngle;
    if (delta == 0)
    {
        return 0;
    }
    
    bool counterclockwise = ((delta > 0 && delta <= 180) || (delta < -180));
    return counterclockwise ? +1 : -1;
}

void OjerAppEngine2d::registerForDeltaTimeEvents(onDeltaTimeEventCallBack callBack)
{
    std::list<onDeltaTimeEventCallBack>::iterator it = m_onDeltatTimeEventListeners.begin();
    
#ifdef debug
    while(it != m_onDeltatTimeEventListeners.end())
    {
        if(*it == callBack)
        {
            std::cout << "***********************************************************" << std::endl;
            std::cout << "Warning Adding Duplicate Entry In registerForDeltaTimeEvents" << std::endl;
            std::cout << "***********************************************************" << std::endl;
        }
        it++;
    }
#endif
    
    m_onDeltatTimeEventListeners.push_back(callBack);
}

float OjerAppEngine2d::getLoopingAccumulatedDeltaTime()
{
	return m_rollingAccumulatedDeltaTime;
}

void OjerAppEngine2d::unregisterForDeltaTimeEvents(onDeltaTimeEventCallBack callBack)
{
    m_onDeltatTimeEventListeners.remove(callBack);
}

void OjerAppEngine2d::OnDeltaTime(float timeStep)
{
    
	++mFrameCount;
	
	float averagedTime = mRuningAverage->average(timeStep);

	/*	
	if ((timeStep / cappedTimeStep) - 1.0 > 0.3 )
	{
		std::stringstream ss;
		ss << "TimeStamp " << timeStep << " ratio " << timeStep / cappedTimeStep <<"\n";
		Logger::Log(Logger::DEFUALT, ss.str());
	}

	*/

	updateAccumulatedDeltaTime(averagedTime);

    if(m_physicsManager != NULL)
    {
        m_physicsManager->onDeltaTime(averagedTime);
    }
    
    GameEvents::GameDeltaTimeEvent gameDeltaTimeEvent(averagedTime);
    mDispatcher.dispatchEvent(gameDeltaTimeEvent);
    
    //KKonecko: Update onDeltaTime listeners
    std::list<onDeltaTimeEventCallBack>::iterator it = m_onDeltatTimeEventListeners.begin();
    std::list<onDeltaTimeEventCallBack> safeCallList;
    
    while(it != m_onDeltatTimeEventListeners.end())
    {
        safeCallList.push_back(*it);
        it++;
    }

    it = safeCallList.begin();
    
    while(it != safeCallList.end())
    {
        (*it)(averagedTime);
        it++;
    }

    DynamicObjectTrackerManager::getInstance()->clean();
    
    
    float direction = RotationDirection();
    if (direction == 0)
    {
        return;
    }
    
    float degrees = timeStep * 360 * RevolutionsPerSecond;
    m_currentAngle += degrees * direction;

    // Normalize the angle to [0, 360)
    if (m_currentAngle >= 360)
    {
        m_currentAngle -= 360;
    }
    else if (m_currentAngle < 0)
    {
        m_currentAngle += 360;
    }
    
    // If the rotation direction changed, then we overshot the desired angle.
    if (RotationDirection() != direction)
    {
        m_currentAngle = m_desiredAngle;
    }
    
	if (m_physicsManager != NULL)
	{
		m_physicsManager->waitUntilPhysicsIsComplete();
	}
}

void OjerAppEngine2d::OnRotate(DeviceOrientation orientation)
{
    float angle = m_desiredAngle;
    
    switch (orientation) {
        case DeviceOrientationLandscapeLeft:
            angle = 270;
            break;

//        case DeviceOrientationPortraitUpsideDown:
//            angle = 180;
//            break;

        case DeviceOrientationLandscapeRight:
            angle = 90;
            break;
 
		case DeviceOrientationPortrait:
            angle = 0;
			break;

        default:
            break;
    }
    
    m_desiredAngle = angle;
}

//KKonecko: Find another home for these!

void OjerAppEngine2d::updateAccumulatedDeltaTime(float deltaTime)
{
	m_rollingAccumulatedDeltaTime += deltaTime;
	const float MAX_LOOPING_VALUE = 1000.0f;

	if (m_rollingAccumulatedDeltaTime > MAX_LOOPING_VALUE)
	{
		m_rollingAccumulatedDeltaTime -= MAX_LOOPING_VALUE;
	}
}

int OjerAppEngine2d::getInputID(ivec2 position)
{
    InputIDPosition inputIDPosition;
    inputIDPosition.m_id = ++m_currentInputID;
    inputIDPosition.m_position = position;
    m_activeTuchInputs.push_back(inputIDPosition);
    
    std::cout << "getInputID id " << inputIDPosition.m_id << " count " << m_activeTuchInputs.size() << std::endl;

    return m_currentInputID;
}

void OjerAppEngine2d::updateInputID(ivec2 position, int id)
{
    std::vector<InputIDPosition>::iterator it = m_activeTuchInputs.begin();
    
    while(it != m_activeTuchInputs.end())
    {
        if(id == (*it).m_id)
        {
            (*it).m_position = position;
            return;
        }

        it++;
    }

    std::cout << "Failed to update position for input ID  " << id << std::endl;
}

void OjerAppEngine2d::deleteInputID(int id)
{
    std::vector<InputIDPosition>::iterator it = m_activeTuchInputs.begin();
    
    while(it != m_activeTuchInputs.end())
    {
        if(id == (*it).m_id)
        {
            m_activeTuchInputs.erase(it);
            return;
        }
        
        it++;
    }
}

int OjerAppEngine2d::findInputID(ivec2 position, int minDistance)
{
    int closestDistance = 1000000;
    bool found = false;

    std::vector<InputIDPosition>::iterator it = m_activeTuchInputs.begin();
    std::vector<InputIDPosition>::iterator closest = it;

    std::cout << "findInputID id count " << m_activeTuchInputs.size() << std::endl;
    
    while(it != m_activeTuchInputs.end())
    {
        ivec2 distaceFromInput = (*it).m_position - position;
        int distance = std::sqrt(std::abs(distaceFromInput.x * distaceFromInput.x + distaceFromInput.y * distaceFromInput.y));
        
        std::cout << "Comparing id " << (*it).m_id <<  " x " << (*it).m_position.x << " y " << (*it).m_position.y <<  " distance " << distance << std::endl;

        if(distance < closestDistance && distance < minDistance)
        {
            closestDistance = distance;
            closest = it;
            found = true;
        }
        
        it++;
    }

    if(!found)
    {
        std::cout << "Failed to find any inputs " << std::endl;
        return -1;
    }
    
    std::cout << "***************************** " << closestDistance << std::endl;
    std::cout << "closestDistance " << closestDistance << std::endl;
    std::cout << "***************************** " << closestDistance << std::endl;

    return (*closest).m_id;
}

void OjerAppEngine2d::OnFingerUp(ivec2 oldLocation, ivec2 location)
{
    std::cout << "***************************** " << std::endl;
    std::cout << "OnFingerUp location x " << location.x << " y " << location.y << std::endl;
    
    InputManager::InputEventData eD;
    eD.setPosition(location);
    int id = findInputID(location, 250);

    //KKonecko: Hack to pop all buttons near the finger up area. Prevents button sticking, this is temporary as I want to rewrite input code
    while(id != -1)
    {
        deleteInputID(id);
        eD.setID(id);
        std::cout << "Deleting id " << eD.getID() << std::endl;

        m_inputManager.onHardwareInputEvent(InputManager::DEVICE_TOUCH_UP, eD);
        id =findInputID(location, 100);
    }
    
    std::cout << "***************************** " << std::endl;

}

void OjerAppEngine2d::OnFingerDown(ivec2 location)
{
    std::cout << "***************************** " << std::endl;

    int id = findInputID(location, 100);
    if(id != -1)
    {
        OnFingerUp(location, location);
    }
    
    InputManager::InputEventData eD;
    eD.setPosition(location);
    eD.setID(getInputID(location));
    std::cout << "OnFingerDown location x " << location.x << " y " << location.y << " id " << eD.getID() << std::endl;
    m_inputManager.onHardwareInputEvent(InputManager::DEVICE_TOUCH_DOWN, eD);
    std::cout << "***************************** " << std::endl;

}

void OjerAppEngine2d::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    //std::cout << "OnFingerMove location x " << oldLocation.x << " y " << oldLocation.y << std::endl;
    updateInputID(newLocation, findInputID(oldLocation, 5));
    
    InputManager::InputEventData eD;
    eD.setPosition(newLocation);          
    m_inputManager.onHardwareInputEvent(InputManager::DEVICE_TOUCH_MOVE, eD);    
}

void OjerAppEngine2d::OnKeyDown(const std::string& key)
{
	InputManager::InputEventData eD;
	eD.setCharValue(*key.c_str());
	m_inputManager.onHardwareInputEvent(InputManager::DEVICE_KEY_DOWN, eD);
}

void OjerAppEngine2d::OnKeyUp(const std::string& key)
{
	InputManager::InputEventData eD;
	eD.setCharValue(*key.c_str());
	m_inputManager.onHardwareInputEvent(InputManager::DEVICE_KEY_UP, eD);
}

NPCAIFactory& OjerAppEngine2d::getNPCFactory() {
	return *mNPCAIFactory;
}

PropertyCollectionRepository& OjerAppEngine2d::getArchetypeRepository() {
	return *mArchetypeRepository;
}

Lua::ScriptRepository& OjerAppEngine2d::getLuaScriptRepository() {
	return mLuaScriptRepository;
}

void OjerAppEngine2d::SetRestConnector(IRestConnector* restConnector) {
	mRestConnector = restConnector;
}

IRestConnector* OjerAppEngine2d::GetRestConnector() {
	return mRestConnector;
}

void OjerAppEngine2d::SetUserMemory(IUserMemory* userMemory) {
    m_userMemory = userMemory;
}

IUserMemory* OjerAppEngine2d::GetUserMemory() {
    return m_userMemory;
}

KeyboardManager* OjerAppEngine2d::GetKeyboardManager() {
    return &m_keyboardManager;
}

UserProvider* OjerAppEngine2d::GetUserProvider() {
	return m_userProvider;
}
