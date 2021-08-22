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

class IChangeLeaderboardListener {
public:
    virtual void OnLeftArrowPressed() = 0;
    virtual void OnRightArrowPressed() = 0;
};

class ChallengeMenuWidget : public IUserProfileDisplayListener {
public:
	enum MenuItems { LOAD_MAIN_MENU, LOAD_CHALLENGE_LEVEL };
	typedef fastdelegate::FastDelegate1<MenuItems> onChallengeMenuItemSelectedCallBack;

	ChallengeMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

	void Init();
	void Release();
	void RegisterForChallengeMenuItemSelectedEvent(onChallengeMenuItemSelectedCallBack callback);
	void UnregisterForChallengeMenuItemSelectedEvent(onChallengeMenuItemSelectedCallBack callback);
    
    void RegisterChangeLeaderboardListener(IChangeLeaderboardListener* changeLeaderboardListener);
    
    void AddNewGameButton();

	void SetLeaderboardContents(const ChallengeLeaderboard& leaderboard);
	void SetLeaderboardContentsLoadFailed();
    
    void WaitingForLeaderboard() {
        m_waitingForLeaderboard = true;
    }
    
    void FinishWaitingForLeaderboard() {
        m_waitingForLeaderboard = false;
    }

private:
	std::list<onChallengeMenuItemSelectedCallBack> m_onChallengeMenuItemSelectedListeners;

	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;
    
    IChangeLeaderboardListener* m_changeLeaderboardListener;

	UILabel* m_home;
    UILabel* m_userBestScore;

	UILabel* m_challengeTitle;
	UILabel* m_leaderboardTitle;
	UIComponent* m_leaderboard;
	UILabel* m_newGameButton;
    UILabel* m_currentUsername;

	UILabel* m_loadingLabel;
    
    UILabel* m_leaderboardEntry0;
    UILabel* m_leaderboardEntry1;
    UILabel* m_leaderboardEntry2;
    UILabel* m_leaderboardEntry3;
    UILabel* m_leaderboardEntry4;
    UILabel* m_leaderboardEntry5;
    UILabel* m_leaderboardEntry6;
    UILabel* m_leaderboardEntry7;
    UILabel* m_leaderboardEntry8;
    UILabel* m_leaderboardEntry9;
    
    UILabel* m_leftLeaderboardArrow;
    UILabel* m_rightLeaderboardArrow;
    
    int m_maxPointsSize;
    
    bool m_waitingForLeaderboard;
    bool IsWaitingForLeaderboard() {
        return m_waitingForLeaderboard;
    }

	std::vector<UILabel*> m_leaderboardEntries;
	std::vector<UIUsernameLabel*> m_leaderboardClickListeners;

    void AddCurrentUserUsername();
	void AddHomeButton();
    void AddUserBestScore();

	void AddChallengeTitle();
	void AddLeaderboardFrame();
	void AddLeaderboardTitle();
    void AddEmptyLeaderboard();
	void AddLeaderboardLoading();
    void TakeDownLeaderboardLoading();
    void AddLeftLeaderboardArrow();
    void TakeDownLeftLeaderboardArrow();
    void AddRightLeaderboardArrow();
    void TakeDownRightLeaderboardArrow();
    
    void SetLeaderboardEntry(const ChallengeLeaderboardEntry& challengeLeaderboardEntry, int postition);
    void SetLeaderboardString(const std::string& leaderboardString, int position);
    std::string ChallengeLeaderboardEntryToString(const ChallengeLeaderboardEntry& challengeLeaderboardEntry, int position);

	void OnHomePressed(UITouchButton::ButtonState state);
	void OnNewGamePressed(UITouchButton::ButtonState state);
    
    void OnLeftLeaderboardPressed(UITouchButton::ButtonState state);
    void OnRightLeaderboardPressed(UITouchButton::ButtonState state);
};
