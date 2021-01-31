#include <Core/GUI/Widgets/Live/PreGameLobby/CosmosLivePreGameLobbyWidget.h>
#include <IEngine.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

CosmosLivePreGameLobbyWidget::CosmosLivePreGameLobbyWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_profileWindow(nullptr), m_profileFrame(nullptr), m_title(nullptr), m_activeUsers(nullptr), m_timeUntilGametime(nullptr), m_home(nullptr), m_currentUsername(nullptr), m_chat0(nullptr), m_chat2(nullptr), m_chat1(nullptr), m_chat3(nullptr), m_chat4(nullptr), m_chat5(nullptr), m_chat6(nullptr), m_chat7(nullptr), m_chat8(nullptr), m_chat9(nullptr), m_addChatButton(nullptr), m_chatText(nullptr), m_keyboardManager(nullptr), m_timer(this), m_cursorOn(false), m_editingChat(false), m_cosmosLiveChatReceiver(nullptr) {}

void CosmosLivePreGameLobbyWidget::Init() {
    m_keyboardManager = IEngine::getEngine()->GetKeyboardManager();
    m_keyboardManager->RegisterKeyboardListener(this);
    
    AddProfileWindow();
    AddProfileFrame();
    AddHomeButton();
    AddTitleButton();
    AddActiveUsers();
    AddTimeUntilGametime();
    AddUsername();
    AddChats();
    AddAddChatButton();
}

void CosmosLivePreGameLobbyWidget::Release() {
    m_home->release();
    delete m_home;
    
    m_title->release();
    delete m_title;
    
    m_activeUsers->release();
    delete m_activeUsers;
    
    m_timeUntilGametime->release();
    delete m_timeUntilGametime;
    
    m_currentUsername->release();
    delete m_currentUsername;
    
    m_profileWindow->release();
    delete m_profileWindow;
}

void CosmosLivePreGameLobbyWidget::SetVisible(bool visible) {
    m_currentUsername->setVisible(visible);
    m_activeUsers->setVisible(visible);
    m_timeUntilGametime->setVisible(visible);
    m_title->setVisible(visible);
    m_home->setVisible(visible);
    m_addChatButton->setVisible(visible);
    m_profileFrame->setVisible(visible);
    m_profileWindow->setVisible(visible);
}

void CosmosLivePreGameLobbyWidget::OnDeletePressed() {
    std::string currentChat = m_chat;
    if (currentChat.empty() == false) {
        currentChat.pop_back();
    }
    
    m_chat = currentChat;
    DisplayCursor();
    m_timer.ResetTimer(Timer::TimerType::CURSOR_BLINK_550_MS);
}

void CosmosLivePreGameLobbyWidget::OnCharacterPressed(char c) {
    m_chat += c;
}

void CosmosLivePreGameLobbyWidget::OnEnterPressed() {
    OnSendChat();
}

void CosmosLivePreGameLobbyWidget::AddProfileWindow() {
    m_profileWindow = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_profileWindow->setWidth(m_parentComponent->getWidth());
    m_profileWindow->setHeight(m_parentComponent->getHeight());
    m_parentComponent->addChild(m_profileWindow);
}

void CosmosLivePreGameLobbyWidget::AddProfileFrame() {
    m_profileFrame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_profileFrame->setAnchor(UIComponent::ANCHOR_TOP_CENTER);
    m_profileFrame->setWidth(900);
    m_profileFrame->setHeight(600);
    m_profileFrame->setY(50);

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
    if (m_editingChat) {
        return;
    }
    
    std::string usersString = "players: " + std::to_string(users);
    m_activeUsers->setTextString(usersString);
}

void CosmosLivePreGameLobbyWidget::UpdateTimeUntilGametime(double secondsRemaining) {
    if (m_editingChat) {
        return;
    }
    
    std::string timeString = "gametime: " + SecondsToMinutesSecondsString(secondsRemaining);
    m_timeUntilGametime->setTextString(timeString);
}

std::string CosmosLivePreGameLobbyWidget::SecondsToMinutesSecondsString(double secondsRemaining) const {
    if (secondsRemaining <= 0) {
        return "00:00";
    }
    
    if (secondsRemaining < 60) {
        std::string secondString = std::to_string(secondsRemaining);
        if (secondsRemaining < 10) {
            return "00:0" + secondString.substr(0, 1);
        }

        if (secondString.size() > 3) {
            secondString = secondString.substr(0, 2);
        }
        
        if (secondString.size() == 1) {
            secondString = "0" + secondString;
        }
        
        return "00:" + (secondString);
    }
    
    int minutes = secondsRemaining / 60;
    int seconds = secondsRemaining - (minutes * 60);
    
    std::string minuteString = std::to_string(minutes);
    if (minuteString.size() == 1) {
        minuteString = "0" + minuteString;
    }

    std::string secondString = std::to_string(seconds);
    if (secondString.size() == 1) {
        secondString = "0" + secondString;
    }
    
    return minuteString + ":" + secondString;
}

void CosmosLivePreGameLobbyWidget::UpdateChats(const std::vector<CosmosLiveChat>& chats) {
    std::vector<CosmosLiveChat> fullChats = chats;
    while (fullChats.size() < 10) {
        fullChats.push_back(CosmosLiveChat("", "", ""));
    }
    
    for (int i = 0; i < 10; i++) {
        const CosmosLiveChat &chat = chats[i];
        UpdateChat(chat, 9 - i);
    }
}

void CosmosLivePreGameLobbyWidget::AddActiveUsers() {
    const std::string message = "";
    
    m_activeUsers = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 150, 60, UIComponent::ANCHOR_TOP_CENTER, message);
    m_activeUsers->setDropShadowColor(dropShadowColor);
    m_activeUsers->setX(-260);

    m_parentComponent->addChild(m_activeUsers);
}

void CosmosLivePreGameLobbyWidget::AddTimeUntilGametime() {
    const std::string message = "time";
    
    m_timeUntilGametime = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 150, 60, UIComponent::ANCHOR_TOP_CENTER, message);
    m_timeUntilGametime->setDropShadowColor(dropShadowColor);
    m_timeUntilGametime->setX(230);

    m_parentComponent->addChild(m_timeUntilGametime);
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
    
    const int basePadding = 50;
    const int rowPadding = 41;
    
    m_chat0 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat0->setDropShadowColor(dropShadowColor); m_chat0->setY(basePadding + ( rowPadding * (1 + 0))); m_profileFrame->addChild(m_chat0);
    
    m_chat1 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat1->setDropShadowColor(dropShadowColor); m_chat1->setY(basePadding + ( rowPadding * (1 + 1))); m_profileFrame->addChild(m_chat1);
    
    m_chat2 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat2->setDropShadowColor(dropShadowColor); m_chat2->setY(basePadding + ( rowPadding * (1 + 2))); m_profileFrame->addChild(m_chat2);
    
    m_chat3 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat3->setDropShadowColor(dropShadowColor); m_chat3->setY(basePadding + ( rowPadding * (1 + 3))); m_profileFrame->addChild(m_chat3);
    
    m_chat4 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat4->setDropShadowColor(dropShadowColor); m_chat4->setY(basePadding + ( rowPadding * (1 + 4))); m_profileFrame->addChild(m_chat4);
    
    m_chat5 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat5->setDropShadowColor(dropShadowColor); m_chat5->setY(basePadding + ( rowPadding * (1 + 5))); m_profileFrame->addChild(m_chat5);
    
    m_chat6 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat6->setDropShadowColor(dropShadowColor); m_chat6->setY(basePadding + ( rowPadding * (1 + 6))); m_profileFrame->addChild(m_chat6);
    
    m_chat7 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat7->setDropShadowColor(dropShadowColor); m_chat7->setY(basePadding + ( rowPadding * (1 + 7))); m_profileFrame->addChild(m_chat7);
    
    m_chat8 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat8->setDropShadowColor(dropShadowColor); m_chat8->setY(basePadding + ( rowPadding * (1 + 8))); m_profileFrame->addChild(m_chat8);
    
    m_chat9 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_chat9->setDropShadowColor(dropShadowColor); m_chat9->setY(basePadding + ( rowPadding * (1 + 9))); m_profileFrame->addChild(m_chat9);
}

void CosmosLivePreGameLobbyWidget::UpdateChat(const CosmosLiveChat& chat, int position) {
    std::string chatString = chat.GetUser() + ": " + chat.GetMessage();
    while (chatString.size() < 50) {
        chatString += " ";
    }
    
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

void CosmosLivePreGameLobbyWidget::AddAddChatButton() {
    UITouchButton::onButtonStateChangedCallBack callBack;

    m_addChatButton = UIComponentFactory::getInstance()->createUILabel("KYCQuestionButtonArchetype", 500.0, 90.0, UIComponent::ANCHOR_BOTTOM_CENTER, "Add Chat");
    m_addChatButton->setDropShadowColor(dropShadowColor);
    m_addChatButton->setY(25);

    callBack.bind(this, &CosmosLivePreGameLobbyWidget::OnAddChatPressed);
    m_addChatButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    m_profileWindow->addChild(m_addChatButton);
}

void CosmosLivePreGameLobbyWidget::OnAddChatPressed(UITouchButton::ButtonState state) {
    m_keyboardManager->ActivateKeyboard();
    HideMenuBar();
    MoveFrameUp();
    m_addChatButton->release();
    DisplayChatBox();
    m_timer.RegisterTimer(Timer::TimerType::CURSOR_BLINK_550_MS);
    m_editingChat = true;
}

void CosmosLivePreGameLobbyWidget::OnSendChat() {
    if (!m_chat.empty() && m_cosmosLiveChatReceiver != nullptr) {
        m_cosmosLiveChatReceiver->ChatReceived(m_chat);
    }
    
    m_keyboardManager->DeactivateKeyboard();
    ShowMenuBar();
    MoveFrameDown();
    AddAddChatButton();
    m_chat.clear();
    m_chatText->release();
    m_timer.DeregisterTimer(Timer::TimerType::CURSOR_BLINK_550_MS);
    m_editingChat = false;
}

void CosmosLivePreGameLobbyWidget::MoveFrameUp() {
    m_profileFrame->setY(-300);
}

void CosmosLivePreGameLobbyWidget::MoveFrameDown() {
    m_profileFrame->setY(50);
}

void CosmosLivePreGameLobbyWidget::DisplayChatBox() {
    const std::string message = "chat: ";
    
    m_chatText = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 150, 60, UIComponent::ANCHOR_BOTTOM_CENTER, message);
    m_chatText->setDropShadowColor(dropShadowColor);
    m_chatText->setY(-60);

    m_profileFrame->addChild(m_chatText);
}

void CosmosLivePreGameLobbyWidget::DisplayCursor() {
    m_chatText->setTextString("chat: " + m_chat + "|");
}

void CosmosLivePreGameLobbyWidget::HideCursor() {
    m_chatText->setTextString("chat: " + m_chat + " ");
}

void CosmosLivePreGameLobbyWidget::OnTimerEvent(Timer::TimerType type) {
    switch(type) {
        case Timer::TimerType::CURSOR_BLINK_550_MS:
            if (m_editingChat == false) {
                break;
            }
            
            if (m_cursorOn) {
                HideCursor();
                m_cursorOn = false;
            } else {
                m_cursorOn = true;
                DisplayCursor();
            }
            
            break;
        default:
            break;
    }
}

void CosmosLivePreGameLobbyWidget::HideMenuBar() {
    m_home->setVisible(false);
    m_activeUsers->setVisible(false);
    m_timeUntilGametime->setVisible(false);
    m_currentUsername->setVisible(false);
}

void CosmosLivePreGameLobbyWidget::ShowMenuBar() {
    m_home->setVisible(true);
    m_activeUsers->setVisible(true);
    m_timeUntilGametime->setVisible(true);
    m_currentUsername->setVisible(true);
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

void CosmosLivePreGameLobbyWidget::RegisterCosmosLiveChatReceiver(ICosmosLiveChatReceiver* receiver) {
    m_cosmosLiveChatReceiver = receiver;
}

void CosmosLivePreGameLobbyWidget::DeregisterCosmosLiveChatReceiver() {
    m_cosmosLiveChatReceiver = nullptr;
}
