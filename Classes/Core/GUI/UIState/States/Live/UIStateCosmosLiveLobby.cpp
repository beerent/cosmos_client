#include "Core/GUI/UIState/States/Live/UIStateCosmosLiveLobby.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h>
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GameState/GameStateMachine.h"
#include "IEngine.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>


CONST_STRING_DEF(UIStateCosmosLiveLobby, UI_STATE_COSMOS_LIVE_LOBBY)

void UIStateCosmosLiveLobby::OnEnterState() {
    DisplayLoading();
    
    m_closedWidget = new CosmosLiveClosedWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    
    CosmosLiveClosedWidget::onMenuItemSelectedCallBack callback;
    callback.bind(this, &UIStateCosmosLiveLobby::OnMainMenuItemSelected);
    m_closedWidget->RegisterForChallengeMenuItemSelectedEvent(callback);
    
    m_cosmosLiveCoordinator.RegisterCosmosLiveSessionUpdateListener(this);
    SubmitGuestLoginRequest();
    BaseStateDepricated::OnEnterState();
}

void UIStateCosmosLiveLobby::OnExitState() {
    m_closedWidget->Release();
    delete m_closedWidget;

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
    if (IsLoadingDisplayed()) {
        TakeDownLoading();
    }
    
    switch(session.GetState()) {
        case CosmosLiveState::CLOSED:
            HandleClosedSessionUpdate();
            break;
            
        case CosmosLiveState::PRE_GAME_LOBBY:
            break;
            
        case CosmosLiveState::IN_GAME:
            break;
            
        case CosmosLiveState::POST_GAME_LOBBY:
            break;
            
        default:
            break;
    }
}

void UIStateCosmosLiveLobby::HandleClosedSessionUpdate() {
    m_closedWidget->Init();
}

void UIStateCosmosLiveLobby::OnMainMenuItemSelected(CosmosLiveClosedWidget::MenuItems selectedItem) {
    if (selectedItem == CosmosLiveClosedWidget::LOAD_MAIN_MENU) {
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

bool UIStateCosmosLiveLobby::IsLoadingDisplayed() const {
    return m_loadingWidget != nullptr;
}
