#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardEntry.h>

ChallengeLeaderboardEntry::ChallengeLeaderboardEntry(const User& user, int attemptId, int points)
	: m_user(user), m_attemptId(attemptId), m_points(points) {}

const User&  ChallengeLeaderboardEntry::GetUser() const {
	return m_user;
}

int ChallengeLeaderboardEntry::GetPoints() const {
	return m_points;
}