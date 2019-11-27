#pragma once

#include <string>

class User {
public:
	User(const std::string& username, const std::string& passwordHash);
	std::string GetUsername() const;
	std::string GetPasswordHash() const;

private:
	std::string m_username;
	std::string m_passwordHash;
};