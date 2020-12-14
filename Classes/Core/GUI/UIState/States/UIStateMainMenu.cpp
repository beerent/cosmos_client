#include <Core/GUI/UIState/States/UIStateMainMenu.h>

#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GUI/UIState/States/Authentication/UIStateAuthInput.h"
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"
#include <Core/General/DeviceMemoryInterface.h>
#include <Core/Net/RequestBuilder.h>

#include "IEngine.h"

CONST_STRING_DEF(UIStateMainMenu, UI_STATE_MAINMENU)

namespace requests {
    const std::string GET_MESSAGES = "getMessages";
}

UIStateMainMenu::UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener): BaseStateDepricated(stateChangeListener), m_mainMenuWidget(nullptr), m_usernameEditWidget(nullptr), m_restConnector(nullptr), m_timer(this), m_currentMessageScrollIndex(0) {}

UIStateMainMenu::~UIStateMainMenu() {}

void UIStateMainMenu::OnEnterState() {
    m_restConnector = IEngine::getEngine()->GetRestConnector();
    SendGetMessagesRequest();
    
    m_mainMenuWidget = new MainMenuWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_mainMenuWidget->init();
    
    MainMenuWidget::onMenuItemSelectedCallBack mainMenuSelectionCallback;
    mainMenuSelectionCallback.bind(this, &UIStateMainMenu::onMainMenuItemSelected);
    m_mainMenuWidget->registerForMenuItemSelectedEvent(mainMenuSelectionCallback);
    
    UITouchButton::onButtonStateChangedCallBack usernamePressedCallback;
    usernamePressedCallback.bind(this, &UIStateMainMenu::OnUsernamePressed);
    m_mainMenuWidget->registerUsernamePressedCallback(usernamePressedCallback);
    RegisterTimers();
}

void UIStateMainMenu::OnExitState() {
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
}

void UIStateMainMenu::SendGetMessagesRequest() {
    RequestBuilder requestBuilder;
    requestBuilder.SetEndpoint(requests::GET_MESSAGES);

    std::string requestString = requestBuilder.GetRequestString();
    m_restConnector->SendRequest(requestString, this);
}

void UIStateMainMenu::RestReceived(const std::string& rest) {
    json11::Json json = JsonProvider::ParseString(rest);
    std::string request = json["request"].string_value();
    
    std::string first;
    if (request == requests::GET_MESSAGES) {
        std::vector<std::string> messages = JsonToMessages(json["payload"]);
        BuildMessagesString(messages);
        m_mainMenuWidget->SetMessage(m_messagesString);
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

void UIStateMainMenu::BuildMessagesString(std::vector<std::string> messages) {
    std::string finalMessage;
    std::string skeleton = "                                                                                                                        ";
    for (const auto message : messages) {
        std::string nextMessage = skeleton + message;
        finalMessage += " " + nextMessage;
    }
    
    m_messagesString = finalMessage;
}

void UIStateMainMenu::RegisterTimers() {
    m_timer.RegisterTimer(Timer::TimerType::CHALLENGE_QUESTION_TIMER_100_MS);
}

void UIStateMainMenu::DeregisterTimers() {
    m_timer.DeregisterTimer(Timer::TimerType::CHALLENGE_QUESTION_TIMER_100_MS);
}


void UIStateMainMenu::OnTimerEvent(Timer::TimerType type) {
    switch(type) {
        case Timer::TimerType::CHALLENGE_QUESTION_TIMER_100_MS:
            AdvanceMessageScroll();
            break;
        default:
            break;
    }
}

void UIStateMainMenu::AdvanceMessageScroll() {
    m_messagesString += m_messagesString[0];
    m_messagesString = m_messagesString.substr(1, m_messagesString.length());
    m_mainMenuWidget->UpdateMessage(m_messagesString);
}


void UIStateMainMenu::OnUsernamePressed(UITouchButton::ButtonState state) {
    if (m_usernameEditWidget != nullptr) {
        return;
    }
    
    m_mainMenuWidget->SetVisible(false);
    InitUsernameEditWidget();
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
