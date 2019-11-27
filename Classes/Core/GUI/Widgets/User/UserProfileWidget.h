#pragma once

#include "Core/OpenSource/FastDelegate.h"
#include <list>
#include "Core/GUI/Components/UITouchButton.h"
#include "Core/GUI/Components/UILabel.h"
#include <Core/GUI/Components/UIComponentFactory.h>

#include <Core/User/User.h>
#include <Core/User/Profile/UserProfile.h>

class IProfileCloser {
public:
	virtual void CloseUserProfile() = 0;
};

class UserProfileWidget {

public:
	UserProfileWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

	void Init(const User& user, IProfileCloser* profileCloser);
	void Release();
    
    void DisplayLoading();
    void TakeDownLoading();
    void DisplayUserProfile(const UserProfile& userProfile);
    
    UIComponent* GetWindow();

private:
	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	IProfileCloser* m_profileCloser;

	User m_user;

	UIComponent* m_profileWindow;
	UIComponent* m_profileFrame;
	UILabel* m_usernameLabel;
	UILabel* m_exitButton;
	UILabel* m_logoutButton;
    UILabel* m_loading;
    
    UILabel* m_challengeCount;
    UILabel* m_challengeHighScore;
    UILabel* m_challengeLeaderboardPosition;

	void AddProfileWindow();
	void AddProfileFrame();
	void AddUsernameLabel();
	void AddExitButton();
	void AddLogoutButton();
    void AddLoading();
    
    void AddChallengeCount(int count);
    void AddChallengeHighScore(int score);
    void AddChallengeLeaderboardPosition(int position);
    
	void OnExitPressed(UITouchButton::ButtonState state);
	void OnLogoutPressed(UITouchButton::ButtonState state);
};
