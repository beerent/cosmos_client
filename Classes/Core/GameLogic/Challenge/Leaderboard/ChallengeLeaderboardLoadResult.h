#pragma once

#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboard.h>

class ChallengeLeaderboardLoadResult {
public:
	ChallengeLeaderboardLoadResult();
	ChallengeLeaderboardLoadResult(ChallengeLeaderboard leaderboard);

	bool Success();
	ChallengeLeaderboard GetLeaderboard();

private:
	bool m_success;
	ChallengeLeaderboard m_leaderboard;
};