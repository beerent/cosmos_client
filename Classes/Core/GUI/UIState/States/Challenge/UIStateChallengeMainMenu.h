#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Components/UILabel.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoadResult.h>
#include <Core/GUI/Widgets/Challenge/ChallengeMenuWidget.h>
#include <Core/GameLogic/Authentication/Authenticator.h>

class UIStateChallengeMainMenu : public BaseStateDepricated, ILeaderboardListener, IAuthenticationResultListener {

public:
	UIStateChallengeMainMenu(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(UI_STATE_CHALLENGE_MAIN_MENU)

	virtual void OnEnterState();
	virtual void OnExitState();
    
    virtual void OnAuthenticationResultReceived(AuthenticationResult result);
    
	virtual STRING_ID GetStateID() { return UI_STATE_CHALLENGE_MAIN_MENU; }

private:
	ChallengeMenuWidget* m_challengeMenuWidget;

	ChallengeLeaderboardLoader m_leaderboardLoader;
	ChallengeLeaderboard m_leaderboard;
    
    Authenticator m_authenticator;

    void SubmitLoadChallengeLeaderboardRequest();
    virtual void OnLeaderboardLoaded(const ChallengeLeaderboardLoadResult& result);
    void SubmitGuestLoginRequest();
    void OnChallengeMainMenuItemSelected(ChallengeMenuWidget::MenuItems selectedItem);
};
