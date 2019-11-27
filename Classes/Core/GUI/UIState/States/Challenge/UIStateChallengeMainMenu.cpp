#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h>
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GameState/GameStateMachine.h"
#include "IEngine.h"
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>


CONST_STRING_DEF(UIStateChallengeMainMenu, UI_STATE_CHALLENGE_MAIN_MENU)

void UIStateChallengeMainMenu::OnEnterState() {
	m_challengeMenuWidget = new ChallengeMenuWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
	m_challengeMenuWidget->Init();

	ChallengeMenuWidget::onChallengeMenuItemSelectedCallBack callback;
	callback.bind(this, &UIStateChallengeMainMenu::OnChallengeMainMenuItemSelected);
	m_challengeMenuWidget->RegisterForChallengeMenuItemSelectedEvent(callback);

	BaseStateDepricated::OnEnterState();
	SubmitLoadChallengeLeaderboardRequest();
}

void UIStateChallengeMainMenu::OnExitState() {
	m_challengeMenuWidget->Release();
	BaseStateDepricated::OnExitState();
}

void UIStateChallengeMainMenu::SubmitLoadChallengeLeaderboardRequest() {
	ChallengeLeaderboardLoader::LoadLeaderboardListener callback;
	callback.bind(this, &UIStateChallengeMainMenu::OnChallengeLeaderboardLoaded);

	m_leaderboardLoader.SetRestConnector(IEngine::getEngine()->GetRestConnector());
	m_leaderboardLoader.RegisterLoadLeaderboardListener(callback);
	m_leaderboardLoader.SendLoadLeaderboardRequest();
}

void UIStateChallengeMainMenu::OnChallengeLeaderboardLoaded(ChallengeLeaderboardLoadResult result) {
	if (result.Success()) {
		m_challengeMenuWidget->SetLeaderboardContents(result.GetLeaderboard());
	} else {
		m_challengeMenuWidget->SetLeaderboardContentsLoadFailed();
	}
}

void UIStateChallengeMainMenu::OnChallengeMainMenuItemSelected(ChallengeMenuWidget::MenuItems selectedItem) {
	STRING_ID levelID = StringManager::UNDEFINED;
	
	if (selectedItem == ChallengeMenuWidget::LOAD_CHALLENGE_LEVEL) {
		ChangeState(UIStateChallengeMode::UI_STATE_CHALLENGE_MODE);
	} else if (selectedItem == ChallengeMenuWidget::LOAD_MAIN_MENU) {
		ChangeState(UIStateMainMenu::UI_STATE_MAINMENU);
	}
}