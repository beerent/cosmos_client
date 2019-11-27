#include <Core/User/User.h>

User::User(const std::string& username, const std::string& passwordHash) : m_username(username), m_passwordHash(passwordHash) {}

std::string User::GetUsername() const {
	return m_username;
}

std::string User::GetPasswordHash() const {
	return m_passwordHash;
}