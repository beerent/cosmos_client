#pragma once

#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardEntry.h>
#include <vector>

class ChallengeLeaderboard {
public:
	ChallengeLeaderboard();
	void AddEntry(ChallengeLeaderboardEntry entry);
	ChallengeLeaderboardEntry GetEntryInPlace(int index) const;
	std::vector<ChallengeLeaderboardEntry> GetAllEntries() const;
	int Size() const;

private:
	std::vector<ChallengeLeaderboardEntry> m_orderedEntries;
};