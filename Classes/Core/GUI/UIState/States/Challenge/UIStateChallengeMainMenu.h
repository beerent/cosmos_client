#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Components/UILabel.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoadResult.h>
#include <Core/GUI/Widgets/Challenge/ChallengeMenuWidget.h>

class UIStateChallengeMainMenu : public BaseStateDepricated {

public:
	UIStateChallengeMainMenu(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(UI_STATE_CHALLENGE_MAIN_MENU)

	virtual void OnEnterState();
	virtual void OnExitState();

	virtual STRING_ID GetStateID() { return UI_STATE_CHALLENGE_MAIN_MENU; }

private:
	ChallengeMenuWidget* m_challengeMenuWidget;

	ChallengeLeaderboardLoader m_leaderboardLoader;
	ChallengeLeaderboard m_leaderboard;

	void SubmitLoadChallengeLeaderboardRequest();
	void OnChallengeLeaderboardLoaded(ChallengeLeaderboardLoadResult result);
	void OnChallengeMainMenuItemSelected(ChallengeMenuWidget::MenuItems selectedItem);
};