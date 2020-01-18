#pragma once

#include <Core/Net/IRestConnector.h>
#include <Core/Net/Json/JsonProvider.h>
#include <Core/User/User.h>
#include <Core/OpenSource/FastDelegate.h>
#include <Core/GameLogic/Challenge/Leaderboard/ChallengeLeaderboardLoadResult.h>
#include <list>

const std::string GET_CHALLENGE_LEADERBOARD = "getChallengeLeaderboard";

class ILeaderboardListener {
public:
    virtual void OnLeaderboardLoaded(const ChallengeLeaderboardLoadResult& result) = 0;
};

class ChallengeLeaderboardLoader : public IRestReceiver {
public:

	ChallengeLeaderboardLoader();
	~ChallengeLeaderboardLoader();
	void SetRestConnector(IRestConnector* connector);
	void SendLoadLeaderboardRequest();

	void RegisterLoadLeaderboardListener(ILeaderboardListener* listener);
	void UnregisterLoadLeaderboardListener();

	virtual void RestReceived(const std::string& rest);

private:
	IRestConnector* m_restConnector;
	std::string m_loadleaderboardRequestId;
    
    ILeaderboardListener* m_leaderboardListener;
    
	ChallengeLeaderboardLoadResult JsonToLoadLeaderboardResult(const json11::Json json) const;
	ChallengeLeaderboard JsonToChallengeLeaderboard(const json11::Json json) const;
	void NotifyLoadLeaderboardListeners(ChallengeLeaderboardLoadResult result) const;
};
