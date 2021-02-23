#include "Core/GUI/UIState/States/Live/UIStateCosmosLive.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h>
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GameState/GameStateMachine.h"
#include "IEngine.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>
#include <Core/GameLogic/Live/CosmosLiveStates.h>


CONST_STRING_DEF(UIStateCosmosLive, UI_STATE_COSMOS_LIVE_LOBBY)

//static CosmosLiveSession currentSession = CosmosLiveSession(CosmosLiveState::INVALID, std::time_t(), 0, 0, 0);

void UIStateCosmosLive::OnEnterState() {
    DisplayLoading();
    m_loadingWidget->SetVisible(true);
    
    DisplayClosed();
    DisplayPreGameLobby();
    DisplayInGame();
    
    m_cosmosLiveCoordinator.RegisterCosmosLiveSessionUpdateListener(this);
    SubmitGuestLoginRequest();
    
    m_preGameLobbyWidget->RegisterCosmosLiveChatReceiver(this);
    
    BaseStateDepricated::OnEnterState();
}

void UIStateCosmosLive::OnExitState() {
    m_cosmosLiveCoordinator.DeregisterCosmosLiveSessionUpdateListener();
    TakeDownAllWidgets();
    BaseStateDepricated::OnExitState();
}

void UIStateCosmosLive::SubmitGuestLoginRequest() {
    m_authenticator.SetUser(IEngine::getEngine()->GetUserProvider()->GetUser());
    
    m_authenticator.RegisterAuthenticationResultListener(this);
    m_authenticator.SetRestConnector(IEngine::getEngine()->GetRestConnector());
    m_authenticator.SendGuestAuthenticationRequest();
}

void UIStateCosmosLive::OnAuthenticationResultReceived(AuthenticationResult result) {
    // This is not the ideal location to set visiblity but it works... I think the
    // UI needs to load in place before we actually set it not visible. Otherwise you see the
    // drawing of the UI.
    m_preGameLobbyWidget->SetVisible(false);
    m_closedWidget->SetVisible(false);
    
    if (AuthenticationResult::SUCCESS == result) {
        m_cosmosLiveCoordinator.Start();
    } else {
        //display some error
    }
}

void UIStateCosmosLive::OnCosmosLiveSessionUpdated(const CosmosLiveSession& session) {
    if (m_activeState == session.GetState()) {
        UpdateCurrentSession(session);
    } else {
        ChangeCurrentSession(session);
    }
}

void UIStateCosmosLive::UpdateCurrentSession(const CosmosLiveSession& session) {
    switch(session.GetState()) {
        case CosmosLiveState::PRE_GAME_LOBBY:
            UpdatePreGameLobby(session);
            break;
            
        case CosmosLiveState::IN_GAME:
            UpdateInGame(session);
            break;
        
            
        default:
            break;
    }
}

void UIStateCosmosLive::UpdatePreGameLobby(const CosmosLiveSession& session) {
    m_preGameLobbyWidget->UpdateActiveUsers(session.GetPlayerCount());
    m_preGameLobbyWidget->UpdateChats(session.GetChats());
    m_preGameLobbyWidget->UpdateTimeUntilGametime(session.GetSecondsToStart());
}

void UIStateCosmosLive::UpdateInGame(const CosmosLiveSession& session) {
    int x = 5;
    //m_preGameLobbyWidget->UpdateActiveUsers(session.GetPlayerCount());
    //m_preGameLobbyWidget->UpdateChats(session.GetChats());
    //m_preGameLobbyWidget->UpdateTimeUntilGametime(session.GetSecondsToStart());
}

void UIStateCosmosLive::ChangeCurrentSession(const CosmosLiveSession& session) {
    DeactivateState(m_activeState);
    ActivateState(session);
    m_activeState = session.GetState();
}

void UIStateCosmosLive::DeactivateState(CosmosLiveState state) {
    switch(state) {
        case CosmosLiveState::INVALID:
            m_loadingWidget->SetVisible(false);
            break;

        case CosmosLiveState::CLOSED:
            m_closedWidget->SetVisible(false);
            break;

        case CosmosLiveState::PRE_GAME_LOBBY:
            m_preGameLobbyWidget->SetVisible(false);
            break;
            
        case CosmosLiveState::IN_GAME:
            m_inGameWidget->SetVisible(false);
            break;
            
        default:
            break;
    }
}

void UIStateCosmosLive::ActivateState(const CosmosLiveSession& session) {
    switch (session.GetState()) {
        case CosmosLiveState::PRE_GAME_LOBBY:
            m_preGameLobbyWidget->SetVisible(true);
            UpdatePreGameLobby(session);
            break;
            
        case CosmosLiveState::CLOSED:
            m_closedWidget->SetVisible(true);
            break;
            
        case CosmosLiveState::IN_GAME:
            UpdateInGame(session);
            m_inGameWidget->SetVisible(true);
            break;
            
        case CosmosLiveState::INVALID:
        default:
            DisplayLoading();
            break;
    }
}

void UIStateCosmosLive::OnMainMenuItemSelected(CosmosLiveClosedWidget::MenuItems selectedItem) {
    if (selectedItem == CosmosLiveClosedWidget::LOAD_MAIN_MENU) {
        ChangeState(UIStateMainMenu::UI_STATE_MAINMENU);
    }
}

void UIStateCosmosLive::OnMainMenuItemSelected(CosmosLivePreGameLobbyWidget::MenuItems selectedItem) {
    if (selectedItem == CosmosLivePreGameLobbyWidget::LOAD_MAIN_MENU) {
        ChangeState(UIStateMainMenu::UI_STATE_MAINMENU);
    }
}

void UIStateCosmosLive::DisplayLoading() {
    m_loadingWidget = new LoadingWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_loadingWidget->Init();
}

void UIStateCosmosLive::TakeDownLoading() {
    m_loadingWidget->Release();
    delete m_loadingWidget;
}

void UIStateCosmosLive::DisplayClosed() {
    m_closedWidget = new CosmosLiveClosedWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    
    CosmosLiveClosedWidget::onMenuItemSelectedCallBack callback;
    callback.bind(this, &UIStateCosmosLive::OnMainMenuItemSelected);
    m_closedWidget->RegisterForChallengeMenuItemSelectedEvent(callback);
    
    m_closedWidget->Init();
}

void UIStateCosmosLive::TakeDownClosed() {
    m_closedWidget->Release();
    delete m_closedWidget;
}

void UIStateCosmosLive::DisplayPreGameLobby() {
    m_preGameLobbyWidget = new CosmosLivePreGameLobbyWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    
    CosmosLivePreGameLobbyWidget::onMenuItemSelectedCallBack callback;
    callback.bind(this, &UIStateCosmosLive::OnMainMenuItemSelected);
    m_preGameLobbyWidget->RegisterForChallengeMenuItemSelectedEvent(callback);
    
    m_preGameLobbyWidget->Init();
}

void UIStateCosmosLive::TakeDownPreGameLobby() {
    m_preGameLobbyWidget->Release();
    delete m_preGameLobbyWidget;
}

void UIStateCosmosLive::DisplayInGame() {
    m_inGameWidget = new CosmosLiveInGameWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    
//    CosmosLivePreGameLobbyWidget::onMenuItemSelectedCallBack callback;
//    callback.bind(this, &UIStateCosmosLive::OnMainMenuItemSelected);
//    m_preGameLobbyWidget->RegisterForChallengeMenuItemSelectedEvent(callback);
    
    m_inGameWidget->Init();
}

void UIStateCosmosLive::TakeDownInGame() {
    //m_preGameLobbyWidget->Release();
    //delete m_preGameLobbyWidget;
}

void UIStateCosmosLive::TakeDownAllWidgets() {
    TakeDownLoading();
    TakeDownClosed();
    TakeDownPreGameLobby();
    TakeDownInGame();
}

void UIStateCosmosLive::ChatReceived(const std::string& chat) {
    m_cosmosLiveCoordinator.SendChat(chat);
}
