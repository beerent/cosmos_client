#pragma once

#include <Core/User/User.h>

class UserProvider {
public:

	UserProvider();
	User GetUser();
	void SetUser(const User& user);
    void ResetUser();

	void LogIn();
	void LogOut();
	void QueueLogOut();
	bool IsLogOutQueued();
	bool IsLoggedIn();

private:
    void InitUser();
    
	User m_user;
	bool m_isLoggedIn;
	bool m_isLogoutQueued;
};
