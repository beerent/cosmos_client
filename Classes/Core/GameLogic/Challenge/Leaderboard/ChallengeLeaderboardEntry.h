#pragma once

#include <string>
#include <Core/User/User.h>

class ChallengeLeaderboardEntry {
public:
	ChallengeLeaderboardEntry(const User& user, int attemptId, int points);
	const User& GetUser() const;
	int GetPoints() const;
private:
	User m_user;
	int m_attemptId;
	int m_points;
};