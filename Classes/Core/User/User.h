#pragma once

#include <string>

enum UserAccessLevel {
    INVALID=-1,
    GUEST,
    MEMBER,
    ADMIN
};

class User {
public:
	User(const std::string& username, const std::string& passwordHash, UserAccessLevel accessLevel);
	std::string GetUsername() const;
	std::string GetPasswordHash() const;
    UserAccessLevel GetAccessLevel() const;

private:
	std::string m_username;
	std::string m_passwordHash;
    UserAccessLevel m_accessLevel;
};
