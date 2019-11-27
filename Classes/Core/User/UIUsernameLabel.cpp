#include <Core/User/UIUsernameLabel.h>

UIUsernameLabel::UIUsernameLabel(const User& user, UILabel* usernameLabel) : m_user(user), m_userProfileDisplayListenerSet(false) {
	m_usernameLabel = usernameLabel;
	RegisterForUsernamePresses();
}

UILabel* UIUsernameLabel::GetUsernameLabel() const {
	return m_usernameLabel;
}

void UIUsernameLabel::RegisterForUsernamePresses() {
	UITouchButton::onButtonStateChangedCallBack callBack;
	callBack.bind(this, &UIUsernameLabel::OnUsernamePress);
	m_usernameLabel->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
}

void UIUsernameLabel::RegisterUserProfileDisplayListener(IUserProfileDisplayListener* userProfileDisplayListener) {
	m_userProfileDisplayListener = userProfileDisplayListener;
	m_userProfileDisplayListenerSet = true;
}

void UIUsernameLabel::OnUsernamePress(UITouchButton::ButtonState state) {
	if (m_userProfileDisplayListenerSet) {
		m_userProfileDisplayListener->DisplayUserProfile(m_user);
	}
}