#include "Core/GUI/UIState/States/Live/UIStateCosmosLiveLobby.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h>
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GameState/GameStateMachine.h"
#include "IEngine.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>
#include <Core/GameLogic/Live/CosmosLiveStates.h>


CONST_STRING_DEF(UIStateCosmosLiveLobby, UI_STATE_COSMOS_LIVE_LOBBY)

static CosmosLiveSession currentSession = CosmosLiveSession(CosmosLiveState::INVALID, std::time_t(), 0, 0, 0);

void UIStateCosmosLiveLobby::OnEnterState() {
    m_cosmosLiveCoordinator.UpdateSession(currentSession);
    m_cosmosLiveCoordinator.RegisterCosmosLiveSessionUpdateListener(this);
    
    switch (currentSession.GetState()) {
        case CosmosLiveState::PRE_GAME_LOBBY:
            DisplayPreGameLobby();
            m_cosmosLiveCoordinator.Start();
            break;
            
        case CosmosLiveState::CLOSED:
            DisplayClosed();
            m_cosmosLiveCoordinator.Start();
            break;
            
        case CosmosLiveState::INVALID:
        default:
            DisplayLoading();
            SubmitGuestLoginRequest();
            break;
    }
    
    BaseStateDepricated::OnEnterState();
}

void UIStateCosmosLiveLobby::OnExitState() {
    m_cosmosLiveCoordinator.DeregisterCosmosLiveSessionUpdateListener();
    BaseStateDepricated::OnExitState();
}

void UIStateCosmosLiveLobby::SubmitGuestLoginRequest() {
    m_authenticator.SetUser(IEngine::getEngine()->GetUserProvider()->GetUser());
    
    m_authenticator.RegisterAuthenticationResultListener(this);
    m_authenticator.SetRestConnector(IEngine::getEngine()->GetRestConnector());
    m_authenticator.SendGuestAuthenticationRequest();
}

void UIStateCosmosLiveLobby::OnAuthenticationResultReceived(AuthenticationResult result) {
    if (AuthenticationResult::SUCCESS == result) {
        m_cosmosLiveCoordinator.Start();
    } else {
        //display some error
    }
}

void UIStateCosmosLiveLobby::OnCosmosLiveSessionUpdated(const CosmosLiveSession& session) {
    TakeDownCurrentState();
    
    currentSession = session;
    ChangeState(UIStateCosmosLiveLobby::UI_STATE_COSMOS_LIVE_LOBBY);
}

void UIStateCosmosLiveLobby::TakeDownCurrentState() {
    switch(currentSession.GetState()) {
        case CosmosLiveState::INVALID:
            TakeDownLoading();
            break;

        case CosmosLiveState::CLOSED:
            TakeDownClosed();
            break;

        case CosmosLiveState::PRE_GAME_LOBBY:
            TakeDownPreGameLobby();
            break;
    }
}

void UIStateCosmosLiveLobby::OnMainMenuItemSelected(CosmosLiveClosedWidget::MenuItems selectedItem) {
    currentSession = CosmosLiveSession(CosmosLiveState::INVALID, std::time_t(), 0, 0, 0);
    TakeDownClosed();
    
    if (selectedItem == CosmosLiveClosedWidget::LOAD_MAIN_MENU) {
        ChangeState(UIStateMainMenu::UI_STATE_MAINMENU);
    }
}

void UIStateCosmosLiveLobby::OnMainMenuItemSelected(CosmosLivePreGameLobbyWidget::MenuItems selectedItem) {
    currentSession = CosmosLiveSession(CosmosLiveState::INVALID, std::time_t(), 0, 0, 0);
    TakeDownPreGameLobby();
    
    if (selectedItem == CosmosLivePreGameLobbyWidget::LOAD_MAIN_MENU) {
        ChangeState(UIStateMainMenu::UI_STATE_MAINMENU);
    }
}

void UIStateCosmosLiveLobby::DisplayLoading() {
    m_loadingWidget = new LoadingWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_loadingWidget->Init();
}

void UIStateCosmosLiveLobby::TakeDownLoading() {
    m_loadingWidget->Release();
    delete m_loadingWidget;
}

void UIStateCosmosLiveLobby::DisplayClosed() {
    m_closedWidget = new CosmosLiveClosedWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    
    CosmosLiveClosedWidget::onMenuItemSelectedCallBack callback;
    callback.bind(this, &UIStateCosmosLiveLobby::OnMainMenuItemSelected);
    m_closedWidget->RegisterForChallengeMenuItemSelectedEvent(callback);
    
    m_closedWidget->Init();
}

void UIStateCosmosLiveLobby::TakeDownClosed() {
    m_closedWidget->Release();
    delete m_closedWidget;
}

void UIStateCosmosLiveLobby::DisplayPreGameLobby() {
    m_preGameLobbyWidget = new CosmosLivePreGameLobbyWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    
    CosmosLivePreGameLobbyWidget::onMenuItemSelectedCallBack callback;
    callback.bind(this, &UIStateCosmosLiveLobby::OnMainMenuItemSelected);
    m_preGameLobbyWidget->RegisterForChallengeMenuItemSelectedEvent(callback);
    
    m_preGameLobbyWidget->Init();
    m_preGameLobbyWidget->UpdateActiveUsers(currentSession.GetPlayerCount());
}

void UIStateCosmosLiveLobby::TakeDownPreGameLobby() {
    m_preGameLobbyWidget->Release();
    delete m_preGameLobbyWidget;
}
