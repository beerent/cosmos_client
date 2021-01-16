#pragma once

#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include <Core/GameLogic/Authentication/Authenticator.h>
#include <Core/GameLogic/Live/CosmosLiveCoordinator.h>

class UIStateCosmosLiveLobby : public BaseStateDepricated, IAuthenticationResultListener, ICosmosLiveSessionUpdateListener {

public:
    UIStateCosmosLiveLobby(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
    CONST_STRING_DEC(UI_STATE_COSMOS_LIVE_LOBBY)

    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual void OnAuthenticationResultReceived(AuthenticationResult result);
    virtual void OnCosmosLiveSessionUpdated(const CosmosLiveSession& session);
    
    virtual STRING_ID GetStateID() { return UI_STATE_COSMOS_LIVE_LOBBY; }

private:
    Authenticator m_authenticator;
    
    CosmosLiveCoordinator m_cosmosLiveCoordinator;
    
    void SubmitGuestLoginRequest();
    // ChallengeMenuWidget* m_challengeMenuWidget;

    // ChallengeLeaderboardLoader m_leaderboardLoader;
    // ChallengeLeaderboard m_leaderboard;

    // void SubmitLoadChallengeLeaderboardRequest();
    // virtual void OnLeaderboardLoaded(const ChallengeLeaderboardLoadResult& result);
    // void SubmitGuestLoginRequest();
    // void OnChallengeMainMenuItemSelected(ChallengeMenuWidget::MenuItems selectedItem);
};
