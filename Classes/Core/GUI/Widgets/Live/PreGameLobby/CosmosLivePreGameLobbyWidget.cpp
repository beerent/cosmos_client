#include <Core/GUI/Widgets/Live/PreGameLobby/CosmosLivePreGameLobbyWidget.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

CosmosLivePreGameLobbyWidget::CosmosLivePreGameLobbyWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_profileWindow(nullptr), m_profileFrame(nullptr), m_title(nullptr), m_activeUsers(nullptr), m_home(nullptr), m_currentUsername(nullptr), m_chat0(nullptr), m_chat2(nullptr), m_chat1(nullptr), m_chat3(nullptr), m_chat4(nullptr), m_chat5(nullptr), m_chat6(nullptr), m_chat7(nullptr), m_chat8(nullptr), m_chat9(nullptr) {}

void CosmosLivePreGameLobbyWidget::Init() {
    AddProfileWindow();
    AddProfileFrame();
    AddHomeButton();
    AddTitleButton();
    AddActiveUsers();
    AddUsername();
    AddChats();
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
    m_parentComponent->addChild(m_profileWindow);
}

void CosmosLivePreGameLobbyWidget::AddProfileFrame() {
    m_profileFrame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_profileFrame->setAnchor(UIComponent::ANCHOR_CENTER);
    m_profileFrame->setWidth(900);
    m_profileFrame->setHeight(700);

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

void CosmosLivePreGameLobbyWidget::UpdateChats(const std::vector<CosmosLiveChat>& chats) {
    std::vector<CosmosLiveChat> fullChats = chats;
    while (fullChats.size() < 10) {
        fullChats.push_back(CosmosLiveChat("", "", ""));
    }
    
    int position = 0;
    for (const auto& chat : chats) {
        UpdateChat(chat, position++);
    }
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

void CosmosLivePreGameLobbyWidget::AddChats() {
    const std::string text = "";
    float textWidth = 12.5 * text.size();
    
    m_chat0 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat0->setDropShadowColor(dropShadowColor); m_chat0->setY(70 + ( 40 * (1 + 0))); m_profileFrame->addChild(m_chat0);
    
    m_chat1 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat1->setDropShadowColor(dropShadowColor); m_chat1->setY(70 + ( 40 * (1 + 1))); m_profileFrame->addChild(m_chat1);
    
    m_chat2 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat2->setDropShadowColor(dropShadowColor); m_chat2->setY(70 + ( 40 * (1 + 2))); m_profileFrame->addChild(m_chat2);
    
    m_chat3 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat3->setDropShadowColor(dropShadowColor); m_chat3->setY(70 + ( 40 * (1 + 3))); m_profileFrame->addChild(m_chat3);
    
    m_chat4 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat4->setDropShadowColor(dropShadowColor); m_chat4->setY(70 + ( 40 * (1 + 4))); m_profileFrame->addChild(m_chat4);
    
    m_chat5 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat5->setDropShadowColor(dropShadowColor); m_chat5->setY(70 + ( 40 * (1 + 5))); m_profileFrame->addChild(m_chat5);
    
    m_chat6 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat6->setDropShadowColor(dropShadowColor); m_chat6->setY(70 + ( 40 * (1 + 6))); m_profileFrame->addChild(m_chat6);
    
    m_chat7 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat7->setDropShadowColor(dropShadowColor); m_chat7->setY(70 + ( 40 * (1 + 7))); m_profileFrame->addChild(m_chat7);
    
    m_chat8 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat8->setDropShadowColor(dropShadowColor); m_chat8->setY(70 + ( 40 * (1 + 8))); m_profileFrame->addChild(m_chat8);
    
    m_chat9 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat9->setDropShadowColor(dropShadowColor); m_chat9->setY(70 + ( 40 * (1 + 9))); m_profileFrame->addChild(m_chat9);
}

void CosmosLivePreGameLobbyWidget::UpdateChat(const CosmosLiveChat& chat, int position) {
    const std::string chatString = chat.GetUser() + ": " + chat.GetMessage();
    
    switch (position) {
        case 0: m_chat0->setTextString(chatString); break;
        case 1: m_chat1->setTextString(chatString); break;
        case 2: m_chat2->setTextString(chatString); break;
        case 3: m_chat3->setTextString(chatString); break;
        case 4: m_chat4->setTextString(chatString); break;
        case 5: m_chat5->setTextString(chatString); break;
        case 6: m_chat6->setTextString(chatString); break;
        case 7: m_chat7->setTextString(chatString); break;
        case 8: m_chat8->setTextString(chatString); break;
        case 9: m_chat9->setTextString(chatString); break;
    }
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
