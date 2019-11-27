#pragma once

#include <Core/GUI/Components/UILabel.h>
#include <Core/User/User.h>
#include <Core/GUI/Widgets/User/IUserProfileDisplayListener.h>

class UIUsernameLabel {
public:
	UIUsernameLabel(const User& user, UILabel* label);
	UILabel* GetUsernameLabel() const;
	void RegisterUserProfileDisplayListener(IUserProfileDisplayListener* userProfileDisplayListener);

private:
	UILabel* m_usernameLabel;
	User m_user;

	IUserProfileDisplayListener* m_userProfileDisplayListener;
	bool m_userProfileDisplayListenerSet;

	void RegisterForUsernamePresses();
	void OnUsernamePress(UITouchButton::ButtonState state);
};