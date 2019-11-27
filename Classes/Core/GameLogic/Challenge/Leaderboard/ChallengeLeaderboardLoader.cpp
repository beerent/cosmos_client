#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>
#include <Core/Net/RequestBuilder.h>
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardEntry.h>
#include <Core/User/User.h>

ChallengeLeaderboardLoader::ChallengeLeaderboardLoader() {}

ChallengeLeaderboardLoader::~ChallengeLeaderboardLoader() {
	m_restConnector->CloseRequest(m_loadleaderboardRequestId);
}

void ChallengeLeaderboardLoader::SetRestConnector(IRestConnector* connector) {
	m_restConnector = connector;
}

void ChallengeLeaderboardLoader::RegisterLoadLeaderboardListener(LoadLeaderboardListener listener) {
	m_loadLeaderboardListeners.push_back(listener);
}

void ChallengeLeaderboardLoader::UnregisterLoadLeaderboardListener(LoadLeaderboardListener listener) {
	m_loadLeaderboardListeners.remove(listener);
}

void ChallengeLeaderboardLoader::NotifyLoadLeaderboardListeners(ChallengeLeaderboardLoadResult result) const {
	for (auto listener : m_loadLeaderboardListeners) {
		(listener)(result);
	}
}

void ChallengeLeaderboardLoader::RestReceived(const std::string& rest) {
	json11::Json json = JsonProvider::ParseString(rest);
	const ChallengeLeaderboardLoadResult leaderboardLoadResult = JsonToLoadLeaderboardResult(json);

	NotifyLoadLeaderboardListeners(leaderboardLoadResult);
}

ChallengeLeaderboardLoadResult ChallengeLeaderboardLoader::JsonToLoadLeaderboardResult(const json11::Json json) const {
	auto success = json["success"];
	if (success.bool_value() == false) {
		return ChallengeLeaderboardLoadResult();
	}

	ChallengeLeaderboard leaderboard = JsonToChallengeLeaderboard(json["payload"]["leaderboardEntries"]);
	return ChallengeLeaderboardLoadResult(leaderboard);
}

void ChallengeLeaderboardLoader::SendLoadLeaderboardRequest() {
	RequestBuilder requestBuilder;
	requestBuilder.SetEndpoint(GET_CHALLENGE_LEADERBOARD);

	std::string requestString = requestBuilder.GetRequestString();
	m_loadleaderboardRequestId = m_restConnector->SendRequest(requestString, this);
}

ChallengeLeaderboard ChallengeLeaderboardLoader::JsonToChallengeLeaderboard(const json11::Json json) const {
	auto leaderboardEntries = json.array_items();

	ChallengeLeaderboard leaderboard;
	for (auto entry : leaderboardEntries) {
		std::string username = entry["username"].string_value();
		int id = entry["id"].int_value();
		int points = entry["points"].int_value();

		User user(username, "");
		ChallengeLeaderboardEntry leaderboardEntry(user, id, points);
		leaderboard.AddEntry(leaderboardEntry);
	}

	return leaderboard;
}
