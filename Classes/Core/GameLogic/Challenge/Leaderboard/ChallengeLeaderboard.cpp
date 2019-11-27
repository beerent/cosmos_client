#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboard.h>

ChallengeLeaderboard::ChallengeLeaderboard() {}

void ChallengeLeaderboard::AddEntry(ChallengeLeaderboardEntry entry) {
	for (auto it = m_orderedEntries.begin(); it != m_orderedEntries.end(); it++) {
		if (entry.GetPoints() > it->GetPoints()) {
			m_orderedEntries.insert(it, entry);
			return;
		}
	}

	m_orderedEntries.push_back(entry);
}

ChallengeLeaderboardEntry ChallengeLeaderboard::GetEntryInPlace(int index) const {
	return m_orderedEntries[index];
}

std::vector<ChallengeLeaderboardEntry> ChallengeLeaderboard::GetAllEntries() const {
	return m_orderedEntries;
}

int ChallengeLeaderboard::Size() const {
	return m_orderedEntries.size();
}
