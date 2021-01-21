#include <Core/GUI/Widgets/Live/Closed/CosmosLiveClosedWidget.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

CosmosLiveClosedWidget::CosmosLiveClosedWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_profileWindow(nullptr), m_profileFrame(nullptr), m_title(nullptr), m_message(nullptr), m_home(nullptr), m_currentUsername(nullptr) {}

void CosmosLiveClosedWidget::Init() {
    AddProfileWindow();
    AddProfileFrame();
    AddHomeButton();
    AddTitleButton();
    AddMessageButton();
    AddUsername();
}

void CosmosLiveClosedWidget::Release() {
    m_home->release();
    delete m_home;
    
    m_title->release();
    delete m_title;
    
    m_message->release();
    delete m_message;
    
    m_currentUsername->release();
    delete m_currentUsername;
    
    m_profileWindow->release();
    delete m_profileWindow;
}

void CosmosLiveClosedWidget::AddProfileWindow() {
    m_profileWindow = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_profileWindow->setWidth(m_parentComponent->getWidth());
    m_profileWindow->setHeight(m_parentComponent->getHeight());
    m_parentComponent->addChild(m_profileWindow);
}

void CosmosLiveClosedWidget::AddProfileFrame() {
    m_profileFrame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_profileFrame->setAnchor(UIComponent::ANCHOR_CENTER);
    m_profileFrame->setWidth(900);
    m_profileFrame->setHeight(460);

    m_profileWindow->addChild(m_profileFrame);
}

void CosmosLiveClosedWidget::AddHomeButton() {
    UITouchButton::onButtonStateChangedCallBack callBack;

    m_home = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 150, 60, UIComponent::ANCHOR_TOP_LEFT, "        home        ");
    m_home->setDropShadowColor(dropShadowColor);
    m_home->setX(30);

    callBack.bind(this, &CosmosLiveClosedWidget::OnHomePressed);
    m_home->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

    m_parentComponent->addChild(m_home);
}



void CosmosLiveClosedWidget::AddTitleButton() {
    m_title = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "Cosmos Live");
    m_title->setDropShadowColor(dropShadowColor);
    m_title->setY(12.0);

    m_profileFrame->addChild(m_title);
}

void CosmosLiveClosedWidget::AddMessageButton() {
    const std::string message = "Cosmos Live Lobby is currently closed.";
    
    m_message = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_CENTER, message);
    m_message->setDropShadowColor(dropShadowColor);
    m_message->setY(12.0);

    m_profileFrame->addChild(m_message);
}

void CosmosLiveClosedWidget::AddUsername() {
    const std::string username = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    float usernameWidth = 12.5 * username.size();
    m_currentUsername = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernameWidth, 60, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_currentUsername->setDropShadowColor(dropShadowColor);
    m_currentUsername->setX(80);
    m_parentComponent->addChild(m_currentUsername);
}

void CosmosLiveClosedWidget::OnHomePressed(UITouchButton::ButtonState state) {
        m_onHomeMenuItemSelectedListener(LOAD_MAIN_MENU);
}

void CosmosLiveClosedWidget::RegisterForChallengeMenuItemSelectedEvent(onMenuItemSelectedCallBack callback) {
    m_onHomeMenuItemSelectedListener = callback;
}

void CosmosLiveClosedWidget::UnregisterForChallengeMenuItemSelectedEvent(onMenuItemSelectedCallBack callback) {
    m_onHomeMenuItemSelectedListener = nullptr;
}
