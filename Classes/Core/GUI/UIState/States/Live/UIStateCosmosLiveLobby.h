#pragma once

#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include <Core/GameLogic/Authentication/Authenticator.h>
#include <Core/GameLogic/Live/CosmosLiveCoordinator.h>

#include <Core/GUI/Widgets/Loading/LoadingWidget.h>
#include <Core/GUI/Widgets/Live/Closed/CosmosLiveClosedWidget.h>

class UIStateCosmosLiveLobby : public BaseStateDepricated, IAuthenticationResultListener, ICosmosLiveSessionUpdateListener {

public:
    UIStateCosmosLiveLobby(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener), m_loadingWidget(nullptr), m_closedWidget(nullptr) {};
    CONST_STRING_DEC(UI_STATE_COSMOS_LIVE_LOBBY)

    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual void OnAuthenticationResultReceived(AuthenticationResult result);
    virtual void OnCosmosLiveSessionUpdated(const CosmosLiveSession& session);
    
    virtual STRING_ID GetStateID() { return UI_STATE_COSMOS_LIVE_LOBBY; }

private:
    void SubmitGuestLoginRequest();
    void HandleClosedSessionUpdate();
    void OnMainMenuItemSelected(CosmosLiveClosedWidget::MenuItems selectedItem);
    void DisplayLoading();
    void TakeDownLoading();
    bool IsLoadingDisplayed() const;

    Authenticator m_authenticator;
    CosmosLiveCoordinator m_cosmosLiveCoordinator;
    LoadingWidget* m_loadingWidget;
    CosmosLiveClosedWidget* m_closedWidget;

    // ChallengeLeaderboardLoader m_leaderboardLoader;
    // ChallengeLeaderboard m_leaderboard;

    // void SubmitLoadChallengeLeaderboardRequest();
    // virtual void OnLeaderboardLoaded(const ChallengeLeaderboardLoadResult& result);
    // void SubmitGuestLoginRequest();
    // void OnChallengeMainMenuItemSelected(ChallengeMenuWidget::MenuItems selectedItem);
};
