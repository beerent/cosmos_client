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
	User(const std::string& uid, const std::string& username, const std::string& passwordHash, UserAccessLevel accessLevel);
    std::string GetUid() const;
	std::string GetUsername() const;
	std::string GetPasswordHash() const;
    UserAccessLevel GetAccessLevel() const;
    bool Equals(const User& user) const;

private:
    std::string m_uid;
	std::string m_username;
	std::string m_passwordHash;
    UserAccessLevel m_accessLevel;
};
