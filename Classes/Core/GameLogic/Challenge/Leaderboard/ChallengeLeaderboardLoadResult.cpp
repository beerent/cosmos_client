#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoadResult.h>

ChallengeLeaderboard emptyLeaderboard("");

ChallengeLeaderboardLoadResult::ChallengeLeaderboardLoadResult() : m_success(false) {}

ChallengeLeaderboardLoadResult::ChallengeLeaderboardLoadResult(std::vector<ChallengeLeaderboard> leaderboards)
  : m_success(true), m_leaderboards(leaderboards) {}

bool ChallengeLeaderboardLoadResult::Success() const {
	return m_success;
}

std::vector<ChallengeLeaderboard> ChallengeLeaderboardLoadResult::GetLeaderboards() const {
	return m_leaderboards;
}
