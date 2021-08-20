#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Components/UILabel.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoadResult.h>
#include <Core/GUI/Widgets/Challenge/ChallengeMenuWidget.h>
#include <Core/GameLogic/Authentication/Authenticator.h>
#include <Core/Util/SimpleTimer.h>

class UIStateChallengeMainMenu : public BaseStateDepricated, ILeaderboardListener, IAuthenticationResultListener, Timer::SimpleTimerListener {

public:
	UIStateChallengeMainMenu(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener), m_timer(this), m_currentLeaderboardIndex(-1) {};
	CONST_STRING_DEC(UI_STATE_CHALLENGE_MAIN_MENU)

	virtual void OnEnterState();
	virtual void OnExitState();
    
    virtual void OnAuthenticationResultReceived(AuthenticationResult result);
    virtual void OnTimerEvent(Timer::TimerType type);
    
	virtual STRING_ID GetStateID() { return UI_STATE_CHALLENGE_MAIN_MENU; }

private:
	ChallengeMenuWidget* m_challengeMenuWidget;

	ChallengeLeaderboardLoader m_leaderboardLoader;
	std::vector<ChallengeLeaderboard> m_leaderboards;
    int m_currentLeaderboardIndex;
    
    Authenticator m_authenticator;
    
    Timer::SimpleTimer m_timer;

    void SubmitLoadChallengeLeaderboardRequest();
    virtual void OnLeaderboardLoaded(const ChallengeLeaderboardLoadResult& result, double leaderboardChangeTimerMs);
    const ChallengeLeaderboard& GetNextLeaderboard();
    void SubmitGuestLoginRequest();
    void OnChallengeMainMenuItemSelected(ChallengeMenuWidget::MenuItems selectedItem);
    
    void RegisterTimers();
    void DeregisterTimers();
};
