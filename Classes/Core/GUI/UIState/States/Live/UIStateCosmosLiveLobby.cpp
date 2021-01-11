#include "Core/GUI/UIState/States/Live/UIStateCosmosLiveLobby.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h>
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GameState/GameStateMachine.h"
#include "IEngine.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>


CONST_STRING_DEF(UIStateCosmosLiveLobby, UI_STATE_COSMOS_LIVE_LOBBY)

void UIStateCosmosLiveLobby::OnEnterState() {
    SubmitGuestLoginRequest();
    BaseStateDepricated::OnEnterState();
}

void UIStateCosmosLiveLobby::OnExitState() {
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
        //IEngine::getEngine()->GetUserProvider()->LogIn();
        //m_challengeMenuWidget->AddNewGameButton();
    } else {
        //display some error
    }
}
