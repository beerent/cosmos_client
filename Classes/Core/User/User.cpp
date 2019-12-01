#include <Core/User/User.h>

User::User(const std::string& username, const std::string& passwordHash, UserAccessLevel accessLevel) : m_username(username), m_passwordHash(passwordHash), m_accessLevel(accessLevel) {}

std::string User::GetUsername() const {
	return m_username;
}

std::string User::GetPasswordHash() const {
	return m_passwordHash;
}

UserAccessLevel User::GetAccessLevel() const {
    return m_accessLevel;
}
