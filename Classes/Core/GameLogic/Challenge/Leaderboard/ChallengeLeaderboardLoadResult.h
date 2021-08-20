#pragma once

#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboard.h>
#include <vector>

class ChallengeLeaderboardLoadResult {
public:
	ChallengeLeaderboardLoadResult();
	ChallengeLeaderboardLoadResult(std::vector<ChallengeLeaderboard> leaderboards);

	bool Success() const;
	std::vector<ChallengeLeaderboard> GetLeaderboards() const;

private:
	bool m_success;
	std::vector<ChallengeLeaderboard> m_leaderboards;
};
