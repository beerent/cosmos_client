#include <Core/User/UserProvider.h>
#include <IEngine.h>

User INVALID_USER("", "");

UserProvider::UserProvider() : m_user(INVALID_USER), m_isLoggedIn(false), m_isLogoutQueued(false) {}

User UserProvider::GetUser() const {
	return m_user;
}

void UserProvider::SetUser(const User& user) {
	m_user = user;
}

void UserProvider::LogIn() {
	m_isLoggedIn = true;
	m_isLogoutQueued = false;
    IEngine::getEngine()->GetUserMemory()->StoreUsername(IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername());
    IEngine::getEngine()->GetUserMemory()->StorePassword(IEngine::getEngine()->GetUserProvider()->GetUser().GetPasswordHash());
}

void UserProvider::LogOut() {
	m_isLoggedIn = false;
	m_isLogoutQueued = false;
    IEngine::getEngine()->GetUserMemory()->StoreUsername("");
    IEngine::getEngine()->GetUserMemory()->StorePassword("");
}

void UserProvider::QueueLogOut() {
	m_isLogoutQueued = true;
}

bool UserProvider::IsLogOutQueued() {
	return m_isLogoutQueued;
}

bool UserProvider::IsLoggedIn() {
	return m_isLoggedIn;
}
