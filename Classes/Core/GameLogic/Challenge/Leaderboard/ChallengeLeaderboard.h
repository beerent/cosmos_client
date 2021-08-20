#pragma once

#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardEntry.h>
#include <vector>

class ChallengeLeaderboard {
public:
	ChallengeLeaderboard(const std::string& title);
	void AddEntry(ChallengeLeaderboardEntry entry);
	ChallengeLeaderboardEntry GetEntryInPlace(int index) const;
	std::vector<ChallengeLeaderboardEntry> GetAllEntries() const;
	int Size() const;
    std::string GetTitle() const;

private:
    std::string m_title;
	std::vector<ChallengeLeaderboardEntry> m_orderedEntries;
};
