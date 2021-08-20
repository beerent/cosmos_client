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

void ChallengeLeaderboardLoader::NotifyLoadLeaderboardListeners(ChallengeLeaderboardLoadResult result, double leaderboardChangeTimerMs) const {
    if (nullptr != m_leaderboardListener) {
        m_leaderboardListener->OnLeaderboardLoaded(result, leaderboardChangeTimerMs);
    }
}

void ChallengeLeaderboardLoader::RestReceived(const std::string& rest) {
	json11::Json json = JsonProvider::ParseString(rest);
	const ChallengeLeaderboardLoadResult leaderboardLoadResult = JsonToLoadLeaderboardResult(json);
    double leaderboardChangeTimerMs = JsonToLeaderboardChangeTimerMs(json);

	NotifyLoadLeaderboardListeners(leaderboardLoadResult, leaderboardChangeTimerMs);
}

ChallengeLeaderboardLoadResult ChallengeLeaderboardLoader::JsonToLoadLeaderboardResult(const json11::Json json) const {
	auto success = json["success"];
	if (success.bool_value() == false) {
		return ChallengeLeaderboardLoadResult();
	}

	std::vector<ChallengeLeaderboard> leaderboards = JsonToChallengeLeaderboard(json["payload"]["leaderboards"]);
	return ChallengeLeaderboardLoadResult(leaderboards);
}

double ChallengeLeaderboardLoader::JsonToLeaderboardChangeTimerMs(const json11::Json json) const {
    return std::atoi(json["payload"]["leaderboard_change_timer_ms"].string_value().c_str());
}

void ChallengeLeaderboardLoader::SendLoadLeaderboardRequest() {
	RequestBuilder requestBuilder;
	requestBuilder.SetEndpoint(GET_LEADERBOARDS);

	std::string requestString = requestBuilder.GetRequestString();
	m_loadleaderboardRequestId = m_restConnector->SendRequest(requestString, this);
}

std::vector<ChallengeLeaderboard> ChallengeLeaderboardLoader::JsonToChallengeLeaderboard(const json11::Json json) const {
    std::vector<ChallengeLeaderboard> challengeLeaderboards;
    auto leaderboardsJson = json.array_items();
    for (auto leaderboardJson : leaderboardsJson) {
        std::string title = leaderboardJson["title"].string_value();
        auto leaderboardEntries = leaderboardJson["leaderboardEntries"].array_items();

        ChallengeLeaderboard leaderboard(title);
        for (auto entry : leaderboardEntries) {
            std::string username = entry["username"].string_value();
            int id = entry["id"].int_value();
            int points = entry["points"].int_value();

            User user("null", username, "", UserAccessLevel::INVALID);
            ChallengeLeaderboardEntry leaderboardEntry(user, id, points);
            leaderboard.AddEntry(leaderboardEntry);
        }
        
        challengeLeaderboards.push_back(leaderboard);
    }
    
	return challengeLeaderboards;
}
