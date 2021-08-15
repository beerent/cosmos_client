#include <Core/GUI/UIState/States/UIStateMainMenu.h>

#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GUI/UIState/States/Authentication/UIStateAuthInput.h"
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"
#include "Core/GUI/UIState/States/Live/UIStateCosmosLiveLobby.h"
#include <Core/General/DeviceMemoryInterface.h>
#include <Core/Net/RequestBuilder.h>

#include "IEngine.h"

CONST_STRING_DEF(UIStateMainMenu, UI_STATE_MAINMENU)

namespace requests {
    const std::string GET_MESSAGES = "getMessages";
    const std::string GET_ALERT = "getAlert";
}

const int EASTER_EGG_PRESS_COUNT_AMOUNT = 5;
const std::string EASTER_EGG_MESSAGE = "Know Your Cosmos // Created By Brent Ryczak // 2018 - 2021";
const std::string READ_ALERT_PREFIX = "alert_";
const std::string READ_ALERT_VALUE = "READ";
const std::string TUTORIAL_KEY = "TUTORIAL";

UIStateMainMenu::UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener): BaseStateDepricated(stateChangeListener), m_mainMenuWidget(nullptr), m_usernameEditWidget(nullptr),  m_popupWidget(nullptr), m_restConnector(nullptr), m_timer(this), m_currentMessageIndex(-1), m_currentMessageScrollIndex(-10000), m_easterEggPending(true) {}

UIStateMainMenu::~UIStateMainMenu() {}

void UIStateMainMenu::OnEnterState() {
    m_restConnector = IEngine::getEngine()->GetRestConnector();
    SendGetMessagesRequest();
    
    m_mainMenuWidget = new MainMenuWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_mainMenuWidget->init();
    
    if (!TutorialHasBeenRead()) {
        DisplayTutorial();
    } else {
        SendGetAlertRequest();
    }
    
    MainMenuWidget::onMenuItemSelectedCallBack mainMenuSelectionCallback;
    mainMenuSelectionCallback.bind(this, &UIStateMainMenu::onMainMenuItemSelected);
    m_mainMenuWidget->registerForMenuItemSelectedEvent(mainMenuSelectionCallback);
    
    UITouchButton::onButtonStateChangedCallBack usernamePressedCallback;
    usernamePressedCallback.bind(this, &UIStateMainMenu::OnUsernamePressed);
    m_mainMenuWidget->registerUsernamePressedCallback(usernamePressedCallback);
}

void UIStateMainMenu::OnExitState() {
    if (!m_messagesRequestKey.empty()) {
        m_restConnector->CloseRequest(m_messagesRequestKey);
    }
    
    if (!m_alertRequestKey.empty()) {
        m_restConnector->CloseRequest(m_alertRequestKey);
    }
    
    DeregisterTimers();
    
    m_mainMenuWidget->release();
    delete m_mainMenuWidget;
    m_mainMenuWidget = nullptr;
    
    BaseStateDepricated::OnExitState();
}

void UIStateMainMenu::onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem) {
    if (selectedItem == MainMenuWidget::LOAD_LOGIN_LEVEL) {
		ChangeState(UIStateAuthInput::UI_STATE_AUTH_INPUT);
	}

	else if (selectedItem == MainMenuWidget::LOAD_CHALLENGE_MENU_LEVEL) {
		ChangeState(UIStateChallengeMainMenu::UI_STATE_CHALLENGE_MAIN_MENU);
	}

    else if (selectedItem == MainMenuWidget::LOAD_COSMOS_LIVE_LEVEL) {
        ChangeState(UIStateCosmosLiveLobby::UI_STATE_COSMOS_LIVE_LOBBY);
    }
}

void UIStateMainMenu::SendGetMessagesRequest() {
    RequestBuilder requestBuilder;
    requestBuilder.SetEndpoint(requests::GET_MESSAGES);

    std::string requestString = requestBuilder.GetRequestString();
    m_messagesRequestKey = m_restConnector->SendRequest(requestString, this);
}

void UIStateMainMenu::SendGetAlertRequest() {
    RequestBuilder requestBuilder;
    requestBuilder.SetEndpoint(requests::GET_ALERT);

    std::string requestString = requestBuilder.GetRequestString();
    m_alertRequestKey = m_restConnector->SendRequest(requestString, this);
}

void UIStateMainMenu::RestReceived(const std::string& rest) {
    json11::Json json = JsonProvider::ParseString(rest);
    std::string request = json["request"].string_value();
    
    std::string first;
    if (request == requests::GET_MESSAGES) {
        m_messagesRequestKey.clear();
        m_messages = JsonToMessages(json["payload"]);
        if (!m_messages.empty()) {
            AdvanceMessageIndex();
            RegisterTimers();
        }
    } else if (request == requests::GET_ALERT) {
        m_alertRequestKey.clear();
        Alert alert = JsonToAlert(json["payload"]);
        if (!AlertHasBeenRead(alert.GetKey()) && IsValidAlert(alert)) {
            DisplayPopup(alert);
        }
    }
}

std::vector<std::string> UIStateMainMenu::JsonToMessages(const json11::Json& json) {
    auto messagesJson = json["messages"].array_items();

    std::vector<std::string> messages;
    for (auto& messageJson : messagesJson) {
        std::string message = messageJson["message"].string_value();
        messages.push_back(message);
    }

    return messages;
}

Alert UIStateMainMenu::JsonToAlert(const json11::Json& json) {
    std::string key = json["key"].string_value();
    std::string title = json["title"].string_value();
    auto linesJson = json["lines"].array_items();

    std::vector<std::string> lines;
    for (auto& lineJson : linesJson) {
        std::string line = lineJson.string_value();
        lines.push_back(line);
    }

    return Alert(key, title, lines);
}

void UIStateMainMenu::AdvanceMessageIndex() {
    if (m_messages.empty()) {
        return;
    }
    
    if (m_currentMessageIndex == m_messages.size() - 1) {
        m_currentMessageIndex = 0;
    } else {
        m_currentMessageIndex++;
    }
}

void UIStateMainMenu::RegisterTimers() {
    m_timer.RegisterTimer(Timer::TimerType::MESSAGE_SCROLL_TIMER);
}

void UIStateMainMenu::DeregisterTimers() {
    m_timer.DeregisterTimer(Timer::TimerType::MESSAGE_SCROLL_TIMER);
}


void UIStateMainMenu::OnTimerEvent(Timer::TimerType type) {
    switch(type) {
        case Timer::TimerType::MESSAGE_SCROLL_TIMER:
            AdvanceMessageScroll();
            break;
        default:
            break;
    }
}

bool UIStateMainMenu::EasterEggIsActive() const {
    return !m_easterEggPending;
}

bool UIStateMainMenu::ShouldActivateEasterEgg() const {
    return !EasterEggIsActive() && m_mainMenuWidget->GetAppVersionPressCount() == EASTER_EGG_PRESS_COUNT_AMOUNT;
}

void UIStateMainMenu::ActivateEasterEgg() {
    m_easterEggPending = false;
    m_currentMessageIndex = 0;
    m_messages.clear();
    m_messages.push_back(EASTER_EGG_MESSAGE);
}

void UIStateMainMenu::AdvanceMessageScroll() {
    if (ShouldActivateEasterEgg()) {
        ActivateEasterEgg();
    }
    
    if (EasterEggIsActive()) {
        m_mainMenuWidget->ActivateRainbowMessageColor();
    }

    std::string currentMessage = m_messages[m_currentMessageIndex];
    
    int targetSize = 1650 + (10 * currentMessage.size());
    if (m_currentMessageScrollIndex >= targetSize) {
        AdvanceMessageIndex();
        currentMessage = m_messages[m_currentMessageIndex];
        m_currentMessageScrollIndex = -10000;
    }
    
    if (m_currentMessageScrollIndex == -10000) {
        m_currentMessageScrollIndex = -1 * (14 * currentMessage.size() / 2);
    }
    
    m_mainMenuWidget->UpdateMessage(currentMessage, m_currentMessageScrollIndex += 5);
}


void UIStateMainMenu::OnUsernamePressed(UITouchButton::ButtonState state) {
    if (m_usernameEditWidget != nullptr) {
        return;
    }
    
    m_mainMenuWidget->SetVisible(false);
    InitUsernameEditWidget();
}

void UIStateMainMenu::DisplayPopup(const Alert& alert) {
    InitPopup(alert);
    m_mainMenuWidget->SetVisible(false);
    m_mainMenuWidget->SetPartialVisible(true);
    m_popupWidget->SetVisible(true);
}

void UIStateMainMenu::InitPopup(const Alert& alert) {
    m_popupWidget = new PopupWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_popupWidget->Init(this, alert.GetKey(), alert.GetTitle(), alert.GetLines());
}

void UIStateMainMenu::ReleasePopup() {
    m_popupWidget->Release();
    delete m_popupWidget;
    m_popupWidget = nullptr;
}

void UIStateMainMenu::ClosePopup(const std::string& key) {
    MarkAlertAsRead(key);
    
    ReleasePopup();
    m_mainMenuWidget->SetVisible(true);
}

bool UIStateMainMenu::AlertHasBeenRead(const std::string& alertKey) const {
    return READ_ALERT_VALUE == IEngine::getEngine()->GetDeviceUtil()->ReadFromDeviceStorage(READ_ALERT_PREFIX + alertKey);
}

bool UIStateMainMenu::IsValidAlert(const Alert& alert) const {
    return !alert.GetKey().empty() && !alert.GetTitle().empty() && !alert.GetLines().empty();
}

void UIStateMainMenu::MarkAlertAsRead(const std::string& alertKey) {
    IEngine::getEngine()->GetDeviceUtil()->WriteToDeviceStorage(READ_ALERT_PREFIX + alertKey, READ_ALERT_VALUE);
}
        
bool UIStateMainMenu::TutorialHasBeenRead() const {
    return AlertHasBeenRead(TUTORIAL_KEY);
}

void UIStateMainMenu::DisplayTutorial() {
    std::string key = TUTORIAL_KEY;
    std::string title = "Know Your Cosmos Introduction";
    
    std::vector<std::string> lines;
    lines.push_back("Welcome to Know Your Cosmos... Glad that you decided to stop by!");
    lines.push_back("");
    lines.push_back("Fight your way to the top of the Cosmos Quiz leaderboard.");
    lines.push_back("Don't forget to swing by the Cosmic Chat to introduce yourself!");
    lines.push_back("");
    lines.push_back("Feel free to create a custom username by clicking on your username from the main menu.");

    
    Alert alert(key, title, lines);
    DisplayPopup(alert);
}

void UIStateMainMenu::CloseEditUsername(User newUser) {
    ReleaseUsernameEditWidget();
    HandleNewUser(newUser);
    m_mainMenuWidget->SetVisible(true);
}

void UIStateMainMenu::InitUsernameEditWidget() {
    m_usernameEditWidget = new UsernameEditWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_usernameEditWidget->RegisterEditUsernameCloser(this);
    m_usernameEditWidget->Init();
}


void UIStateMainMenu::ReleaseUsernameEditWidget() {
    m_usernameEditWidget->Release();
    delete m_usernameEditWidget;
    m_usernameEditWidget = nullptr;
}

void UIStateMainMenu::HandleNewUser(User newUser) {
    if (!newUser.GetUsername().empty() && !newUser.Equals(IEngine::getEngine()->GetUserProvider()->GetUser())) {
        IEngine::getEngine()->GetUserProvider()->SetUser(newUser);
        IEngine::getEngine()->GetUserProvider()->LogOut();
    }
    
    if (DeviceMemoryInterface().ReadRememberUsername()) {
        DeviceMemoryInterface().StoreUsername(IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername());
    } else {
        DeviceMemoryInterface().StoreUsername("");
    }
}
