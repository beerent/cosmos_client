#include <Core/User/User.h>

User::User(const std::string& uid, const std::string& username, const std::string& passwordHash, UserAccessLevel accessLevel) : m_uid(uid), m_username(username), m_passwordHash(passwordHash), m_accessLevel(accessLevel) {}

std::string User::GetUid() const {
    return m_uid;
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
