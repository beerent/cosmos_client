#include "Core/GUI/UIState/States/Live/UIStateCosmosLiveLobby.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h>
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GameState/GameStateMachine.h"
#include "IEngine.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>
#include <Core/GameLogic/Live/CosmosLiveStates.h>


CONST_STRING_DEF(UIStateCosmosLiveLobby, UI_STATE_COSMOS_LIVE_LOBBY)

static CosmosLiveState currentState = CosmosLiveState::INVALID;

void UIStateCosmosLiveLobby::OnEnterState() {
    if (currentState == CosmosLiveState::INVALID) {
        DisplayLoading();

        m_cosmosLiveCoordinator.RegisterCosmosLiveSessionUpdateListener(this);
        SubmitGuestLoginRequest();
    } else if (currentState == CosmosLiveState::CLOSED) {
        DisplayClosed();
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
    currentState = session.GetState();
    ChangeState(UIStateCosmosLiveLobby::UI_STATE_COSMOS_LIVE_LOBBY);
    
    TakeDownLoading();
}

void UIStateCosmosLiveLobby::OnMainMenuItemSelected(CosmosLiveClosedWidget::MenuItems selectedItem) {
    currentState = CosmosLiveState::INVALID;
    TakeDownClosed();
    
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
