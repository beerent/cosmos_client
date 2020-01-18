#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoader.h>
#include <Core/Net/RequestBuilder.h>
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardEntry.h>
#include <Core/User/User.h>

ChallengeLeaderboardLoader::ChallengeLeaderboardLoader() : m_leaderboardListener(nullptr) {}

ChallengeLeaderboardLoader::~ChallengeLeaderboardLoader() {
	m_restConnector->CloseRequest(m_loadleaderboardRequestId);
}

void ChallengeLeaderboardLoader::SetRestConnector(IRestConnector* connector) {
	m_restConnector = connector;
}

void ChallengeLeaderboardLoader::RegisterLoadLeaderboardListener(ILeaderboardListener* listener) {
    m_leaderboardListener = listener;
}

void ChallengeLeaderboardLoader::UnregisterLoadLeaderboardListener() {
    m_leaderboardListener = nullptr;
}

void ChallengeLeaderboardLoader::NotifyLoadLeaderboardListeners(ChallengeLeaderboardLoadResult result) const {
    if (nullptr != m_leaderboardListener) {
        m_leaderboardListener->OnLeaderboardLoaded(result);
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

		User user(username, "", UserAccessLevel::INVALID);
		ChallengeLeaderboardEntry leaderboardEntry(user, id, points);
		leaderboard.AddEntry(leaderboardEntry);
	}

	return leaderboard;
}
