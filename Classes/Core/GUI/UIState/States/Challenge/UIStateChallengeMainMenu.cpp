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
    m_challengeMenuWidget->RegisterChangeLeaderboardListener(this);

	BaseStateDepricated::OnEnterState();
	SubmitLoadChallengeLeaderboardRequest();
    m_challengeMenuWidget->WaitingForLeaderboard();
    
    if (IEngine::getEngine()->GetUserProvider()->IsLoggedIn() == false) {
        SubmitGuestLoginRequest();
    } else {
        m_challengeMenuWidget->AddNewGameButton();
    }
}

void UIStateChallengeMainMenu::OnExitState() {
	m_challengeMenuWidget->Release();
    delete m_challengeMenuWidget;
    
    m_leaderboardLoader.UnregisterLoadLeaderboardListener();
    m_authenticator.UnregisterAuthenticationResultListener();
    DeregisterTimers();
	BaseStateDepricated::OnExitState();
}

void UIStateChallengeMainMenu::SubmitLoadChallengeLeaderboardRequest() {
    m_leaderboardLoader.RegisterLoadLeaderboardListener(this);
	m_leaderboardLoader.SetRestConnector(IEngine::getEngine()->GetRestConnector());
	m_leaderboardLoader.SendLoadLeaderboardRequest();
}

void UIStateChallengeMainMenu::OnLeaderboardLoaded(const ChallengeLeaderboardLoadResult& result, double leaderboardChangeTimerMs) {
	if (result.Success()) {
        m_timer.SetTimerTime(Timer::TimerType::CHALLENGE_LEADERBOARD_CHANGE_TIMER, leaderboardChangeTimerMs);
        m_leaderboards = result.GetLeaderboards();
		m_challengeMenuWidget->SetLeaderboardContents(GetNextLeaderboard(), m_currentLeaderboardIndex);
        RegisterTimers();
	} else {
		m_challengeMenuWidget->SetLeaderboardContentsLoadFailed();
	}
}

const ChallengeLeaderboard& UIStateChallengeMainMenu::GetNextLeaderboard() {
    if (m_currentLeaderboardIndex < m_leaderboards.size() - 1) {
        m_currentLeaderboardIndex++;
    } else {
        m_currentLeaderboardIndex = 0;
    }

    return m_leaderboards[m_currentLeaderboardIndex];
}

const ChallengeLeaderboard& UIStateChallengeMainMenu::GetPreviousLeaderboard() {
    if (m_currentLeaderboardIndex > 0) {
        m_currentLeaderboardIndex--;
    } else {
        m_currentLeaderboardIndex = m_leaderboards.size() - 1;
    }

    return m_leaderboards[m_currentLeaderboardIndex];
}

void UIStateChallengeMainMenu::SubmitGuestLoginRequest() {
    m_authenticator.SetUser(IEngine::getEngine()->GetUserProvider()->GetUser());
    
    m_authenticator.RegisterAuthenticationResultListener(this);
    m_authenticator.SetRestConnector(IEngine::getEngine()->GetRestConnector());
    m_authenticator.SendGuestAuthenticationRequest();
}

void UIStateChallengeMainMenu::OnAuthenticationResultReceived(AuthenticationResult result) {
    if (AuthenticationResult::SUCCESS == result) {
        IEngine::getEngine()->GetUserProvider()->LogIn();
        m_challengeMenuWidget->AddNewGameButton();
    }
}

void UIStateChallengeMainMenu::OnChallengeMainMenuItemSelected(ChallengeMenuWidget::MenuItems selectedItem) {
	if (selectedItem == ChallengeMenuWidget::LOAD_CHALLENGE_LEVEL) {
		ChangeState(UIStateChallengeMode::UI_STATE_CHALLENGE_MODE);
	} else if (selectedItem == ChallengeMenuWidget::LOAD_MAIN_MENU) {
		ChangeState(UIStateMainMenu::UI_STATE_MAINMENU);
	}
}

void UIStateChallengeMainMenu::OnTimerEvent(Timer::TimerType type) {
    switch(type)
    {
        case Timer::TimerType::CHALLENGE_LEADERBOARD_CHANGE_TIMER:
            m_challengeMenuWidget->SetLeaderboardContents(GetNextLeaderboard(), m_currentLeaderboardIndex);
            break;
    }
}

void UIStateChallengeMainMenu::RegisterTimers() {
    m_timer.RegisterTimer(Timer::TimerType::CHALLENGE_LEADERBOARD_CHANGE_TIMER);
}

void UIStateChallengeMainMenu::DeregisterTimers() {
    m_timer.DeregisterTimer(Timer::TimerType::CHALLENGE_LEADERBOARD_CHANGE_TIMER);
}

void UIStateChallengeMainMenu::OnLeftArrowPressed() {
    DeregisterTimers();
    m_challengeMenuWidget->SetLeaderboardContents(GetPreviousLeaderboard(), m_currentLeaderboardIndex);
}

void UIStateChallengeMainMenu::OnRightArrowPressed() {
    DeregisterTimers();
    m_challengeMenuWidget->SetLeaderboardContents(GetNextLeaderboard(), m_currentLeaderboardIndex);
}
