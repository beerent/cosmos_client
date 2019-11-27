#pragma once

#include <Core/Net/IRestConnector.h>
#include <Core/Net/Json/JsonProvider.h>
#include <Core/User/User.h>
#include <Core/OpenSource/FastDelegate.h>
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoadResult.h>
#include <list>

const std::string GET_CHALLENGE_LEADERBOARD = "getChallengeLeaderboard";

class ChallengeLeaderboardLoader : public IRestReceiver {
public:
	typedef fastdelegate::FastDelegate1<ChallengeLeaderboardLoadResult> LoadLeaderboardListener;

	ChallengeLeaderboardLoader();
	~ChallengeLeaderboardLoader();
	void SetRestConnector(IRestConnector* connector);
	void SendLoadLeaderboardRequest();

	void RegisterLoadLeaderboardListener(LoadLeaderboardListener listener);
	void UnregisterLoadLeaderboardListener(LoadLeaderboardListener listener);

	virtual void RestReceived(const std::string& rest);

private:
	IRestConnector* m_restConnector;
	std::string m_loadleaderboardRequestId;

	std::list<LoadLeaderboardListener> m_loadLeaderboardListeners;

	ChallengeLeaderboardLoadResult JsonToLoadLeaderboardResult(const json11::Json json) const;
	ChallengeLeaderboard JsonToChallengeLeaderboard(const json11::Json json) const;
	void NotifyLoadLeaderboardListeners(ChallengeLeaderboardLoadResult result) const;
};
