#include <Core/GUI/Widgets/Live/Closed/CosmosLiveClosedWidget.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

CosmosLiveClosedWidget::CosmosLiveClosedWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_closedWindow(nullptr), m_closedFrame(nullptr), m_title(nullptr), m_message(nullptr), m_home(nullptr), m_currentUsername(nullptr) {}

void CosmosLiveClosedWidget::Init() {
    AddProfileWindow();
    AddProfileFrame();
    AddHomeButton();
    AddTitleButton();
    AddMessage();
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
    
    m_closedWindow->release();
    delete m_closedWindow;
}

void CosmosLiveClosedWidget::SetVisible(bool visible) {
    m_closedWindow->setVisible(visible);
    m_closedFrame->setVisible(visible);
    
    if (visible) {
        m_title->setTextString("Cosmos Live - Closed");
        m_home->setTextString("        home        ");
        m_message->setTextString("Cosmos Live Lobby is currently closed.");
    } else {
        m_title->setTextString("");
        m_home->setTextString("");
        m_message->setTextString("");
    }
}

void CosmosLiveClosedWidget::AddProfileWindow() {
    m_closedWindow = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_closedWindow->setWidth(m_parentComponent->getWidth());
    m_closedWindow->setHeight(m_parentComponent->getHeight());
    m_parentComponent->addChild(m_closedWindow);
}

void CosmosLiveClosedWidget::AddProfileFrame() {
    m_closedFrame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_closedFrame->setAnchor(UIComponent::ANCHOR_TOP_CENTER);
    m_closedFrame->setWidth(1265);
    m_closedFrame->setHeight(600);
    m_closedFrame->setY(50);

    m_closedWindow->addChild(m_closedFrame);
}

void CosmosLiveClosedWidget::AddHomeButton() {
    UITouchButton::onButtonStateChangedCallBack callBack;

    m_home = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 150, 60, UIComponent::ANCHOR_TOP_LEFT, "");
    m_home->setDropShadowColor(dropShadowColor);
    m_home->setX(30);

    callBack.bind(this, &CosmosLiveClosedWidget::OnHomePressed);
    m_home->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

    m_parentComponent->addChild(m_home);
}

void CosmosLiveClosedWidget::AddTitleButton() {
    m_title = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "");
    m_title->setDropShadowColor(dropShadowColor);
    m_title->setY(12.0);

    m_closedFrame->addChild(m_title);
}

void CosmosLiveClosedWidget::AddMessage() {
    m_message = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_CENTER, "");
    m_message->setDropShadowColor(dropShadowColor);
    m_message->setY(12.0);

    m_closedFrame->addChild(m_message);
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
