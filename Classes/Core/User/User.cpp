#include <Core/User/User.h>

User::User(const std::string& deviceUuid, const std::string& username, const std::string& passwordHash, UserAccessLevel accessLevel) : m_deviceUuid(deviceUuid), m_username(username), m_passwordHash(passwordHash), m_accessLevel(accessLevel) {}

std::string User::GetDeviceUuid() const {
    return m_deviceUuid;
}

std::string User::GetUsername() const {
	return m_username;
}

std::string User::GetPasswordHash() const {
    return m_passwordHash;
}

UserAccessLevel User::GetAccessLevel() const {
    return m_accessLevel;
}

bool User::Equals(const User& user) const {
    const bool usernamesMatch = user.GetUsername() == m_username;
    const bool passwordsMatch = user.GetPasswordHash() == m_passwordHash;
    const bool accessLevelMatches = user.GetAccessLevel() == m_accessLevel;

    return usernamesMatch && passwordsMatch && accessLevelMatches;
}
