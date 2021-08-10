#include <Core/User/UserProvider.h>

#include <Core/Util/UsernameGenerator.h>
#include <Core/General/DeviceMemoryInterface.h>
#include <IEngine.h>

const User INVALID_USER("", "", "", UserAccessLevel::INVALID);
const std::string GUEST_PASSWORD = "guest";

UserProvider::UserProvider() : m_user(INVALID_USER), m_isLoggedIn(false), m_isLogoutQueued(false) {
}

void UserProvider::InitUser() {
    DeviceMemoryInterface deviceMemoryInterface;
    
    std::string uid = IEngine::getEngine()->GetDeviceUtil()->GetDeviceUid();

    std::string username = UsernameGenerator().GetRandomUsername();
    const std::string rememberedUsername = deviceMemoryInterface.ReadUsername();
    
    bool useRememberedUsername = deviceMemoryInterface.ReadRememberUsername() && !rememberedUsername.empty();
    if (useRememberedUsername) {
        username = rememberedUsername;
    }
    
    User user = User(uid, username, GUEST_PASSWORD, UserAccessLevel::GUEST);
    SetUser(user);
}

User UserProvider::GetUser() {
    if (m_user.Equals(INVALID_USER)) {
        InitUser();
    }
    
	return m_user;
}

void UserProvider::SetUser(const User& user) {
	m_user = user;
}

void UserProvider::ResetUser() {
    m_user = INVALID_USER;
    m_isLoggedIn = false;
}

void UserProvider::LogIn() {
	m_isLoggedIn = true;
	m_isLogoutQueued = false;
}

void UserProvider::LogOut() {
	m_isLoggedIn = false;
	m_isLogoutQueued = false;
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
