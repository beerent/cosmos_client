#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMenuLoading.h"

#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h>

#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GameState/GameStateMachine.h"
#include "IEngine.h"

#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>

CONST_STRING_DEF(UIStateChallengeMenuLoading, UI_STATE_CHALLENGE_MENU_LOADING)

void UIStateChallengeMenuLoading::OnEnterState() {
	m_challengeMenuWidget = new ChallengeMenuWidget();
	m_challengeMenuWidget->Init(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());

	BaseStateDepricated::OnEnterState();
	SubmitLoadChallengeLeaderboardRequest();
}

void UIStateChallengeMenuLoading::OnExitState() {
	BaseStateDepricated::OnExitState();
}

void UIStateChallengeMenuLoading::SubmitLoadChallengeLeaderboardRequest() {
	ChallengeLeaderboardLoader::LoadLeaderboardListener callback;
	callback.bind(this, &UIStateChallengeMenuLoading::OnChallengeLeaderboardLoaded);

	m_leaderboardLoader.SetRestConnector(IEngine::getEngine()->GetRestConnector());
	m_leaderboardLoader.RegisterLoadLeaderboardListener(callback);
	m_leaderboardLoader.SendLoadLeaderboardRequest();
}

void UIStateChallengeMenuLoading::OnChallengeLeaderboardLoaded(ChallengeLeaderboardLoadResult result) {
}
