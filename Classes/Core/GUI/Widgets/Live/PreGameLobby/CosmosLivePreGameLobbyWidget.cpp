#include <Core/GUI/Widgets/Live/PreGameLobby/CosmosLivePreGameLobbyWidget.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

CosmosLivePreGameLobbyWidget::CosmosLivePreGameLobbyWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_profileWindow(nullptr), m_profileFrame(nullptr), m_title(nullptr), m_activeUsers(nullptr), m_home(nullptr), m_currentUsername(nullptr) {}

void CosmosLivePreGameLobbyWidget::Init() {
    AddProfileWindow();
    AddProfileFrame();
    AddHomeButton();
    AddTitleButton();
    AddActiveUsers();
    AddUsername();
}

void CosmosLivePreGameLobbyWidget::Release() {
    m_home->release();
    delete m_home;
    
    m_title->release();
    delete m_title;
    
    m_activeUsers->release();
    delete m_activeUsers;
    
    m_currentUsername->release();
    delete m_currentUsername;
    
    m_profileWindow->release();
    delete m_profileWindow;
}

void CosmosLivePreGameLobbyWidget::SetVisible(bool visible) {
    m_currentUsername->setVisible(visible);
    m_activeUsers->setVisible(visible);
    m_title->setVisible(visible);
    m_home->setVisible(visible);
    m_profileFrame->setVisible(visible);
    m_profileWindow->setVisible(visible);
}

void CosmosLivePreGameLobbyWidget::AddProfileWindow() {
    m_profileWindow = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_profileWindow->setWidth(m_parentComponent->getWidth());
    m_profileWindow->setHeight(m_parentComponent->getHeight());
    m_profileWindow->setVisible(false);
    m_parentComponent->addChild(m_profileWindow);
}

void CosmosLivePreGameLobbyWidget::AddProfileFrame() {
    m_profileFrame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_profileFrame->setAnchor(UIComponent::ANCHOR_CENTER);
    m_profileFrame->setWidth(900);
    m_profileFrame->setHeight(460);

    m_profileWindow->addChild(m_profileFrame);
}

void CosmosLivePreGameLobbyWidget::AddHomeButton() {
    UITouchButton::onButtonStateChangedCallBack callBack;

    m_home = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 150, 60, UIComponent::ANCHOR_TOP_LEFT, "        home        ");
    m_home->setDropShadowColor(dropShadowColor);
    m_home->setX(30);

    callBack.bind(this, &CosmosLivePreGameLobbyWidget::OnHomePressed);
    m_home->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

    m_parentComponent->addChild(m_home);
}

void CosmosLivePreGameLobbyWidget::AddTitleButton() {
    m_title = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "Cosmos Live - Pre Game Lobby");
    m_title->setDropShadowColor(dropShadowColor);
    m_title->setY(12.0);

    m_profileFrame->addChild(m_title);
}

void CosmosLivePreGameLobbyWidget::UpdateActiveUsers(int users) {
    std::string usersString = std::to_string(users) + " user";
    if (users != 1) {
        usersString += "s";
    }
    
    m_activeUsers->setTextString(usersString);
}

void CosmosLivePreGameLobbyWidget::AddActiveUsers() {
    const std::string message = "";
    
    m_activeUsers = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 150, 60, UIComponent::ANCHOR_TOP_CENTER, message);
    m_activeUsers->setDropShadowColor(dropShadowColor);

    m_parentComponent->addChild(m_activeUsers);
}

void CosmosLivePreGameLobbyWidget::AddUsername() {
    const std::string username = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    float usernameWidth = 12.5 * username.size();
    m_currentUsername = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernameWidth, 60, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_currentUsername->setDropShadowColor(dropShadowColor);
    m_currentUsername->setX(80);
    m_parentComponent->addChild(m_currentUsername);
}

void CosmosLivePreGameLobbyWidget::OnHomePressed(UITouchButton::ButtonState state) {
        m_onHomeMenuItemSelectedListener(LOAD_MAIN_MENU);
}

void CosmosLivePreGameLobbyWidget::RegisterForChallengeMenuItemSelectedEvent(onMenuItemSelectedCallBack callback) {
    m_onHomeMenuItemSelectedListener = callback;
}

void CosmosLivePreGameLobbyWidget::UnregisterForChallengeMenuItemSelectedEvent(onMenuItemSelectedCallBack callback) {
    m_onHomeMenuItemSelectedListener = nullptr;
}
