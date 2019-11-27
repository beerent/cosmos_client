#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoadResult.h>

ChallengeLeaderboard emptyLeaderboard;

ChallengeLeaderboardLoadResult::ChallengeLeaderboardLoadResult() : m_success(false), m_leaderboard(emptyLeaderboard) {}

ChallengeLeaderboardLoadResult::ChallengeLeaderboardLoadResult(ChallengeLeaderboard leaderboard)
  : m_success(true), m_leaderboard(leaderboard) {}

bool ChallengeLeaderboardLoadResult::Success() {
	return m_success;
}

ChallengeLeaderboard ChallengeLeaderboardLoadResult::GetLeaderboard() {
	return m_leaderboard;
}
