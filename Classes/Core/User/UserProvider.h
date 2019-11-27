#pragma once

#include <Core/User/User.h>

class UserProvider {
public:

	UserProvider();
	User GetUser() const;
	void SetUser(const User& user);

	void LogIn();
	void LogOut();
	void QueueLogOut();
	bool IsLogOutQueued();
	bool IsLoggedIn();

private:
	User m_user;
	bool m_isLoggedIn;
	bool m_isLogoutQueued;
};