#pragma once

#include "Core/OpenSource/FastDelegate.h"
#include <list>
#include "Core/GUI/Components/UITouchButton.h"
#include "Core/GUI/Components/UILabel.h"
#include <Core/GUI/Components/UIComponentFactory.h>

#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboard.h>
#include <Core/GUI/Widgets/User/IUserProfileDisplayListener.h>
#include <Core/User/UIUsernameLabel.h>

#include <vector>

class ChallengeMenuWidget : public IUserProfileDisplayListener {
public:
	enum MenuItems { LOAD_MAIN_MENU, LOAD_CHALLENGE_LEVEL };
	typedef fastdelegate::FastDelegate1<MenuItems> onChallengeMenuItemSelectedCallBack;

	ChallengeMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

	void Init();
	void Release();
	void RegisterForChallengeMenuItemSelectedEvent(onChallengeMenuItemSelectedCallBack callback);
	void UnregisterForChallengeMenuItemSelectedEvent(onChallengeMenuItemSelectedCallBack callback);
    
    void AddNewGameButton();

	void SetLeaderboardContents(const ChallengeLeaderboard& leaderboard);
	void SetLeaderboardContentsLoadFailed();

private:
	std::list<onChallengeMenuItemSelectedCallBack> m_onChallengeMenuItemSelectedListeners;

	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UILabel* m_home;

	UILabel* m_challengeTitle;
	UILabel* m_leaderboardTitle;
	UIComponent* m_leaderboard;
	UILabel* m_newGameButton;

	UILabel* m_loadingLabel;

	std::vector<UILabel*> m_leaderboardEntries;
	std::vector<UIUsernameLabel*> m_leaderboardClickListeners;

	void AddHomeButton();

	void AddChallengeTitle();
	void AddLeaderboardFrame();
	void AddLeaderboardTitle();
	void AddLeaderboardContents();

	void OnHomePressed(UITouchButton::ButtonState state);
	void OnNewGamePressed(UITouchButton::ButtonState state);
};
