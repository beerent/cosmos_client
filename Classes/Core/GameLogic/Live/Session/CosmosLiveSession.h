#pragma once

#import <Core/GameLogic/Live/CosmosLiveStates.h>
#import <Core/GameLogic/Live/Chat/CosmosLiveChat.h>
#include <ctime>

#import <string>
#import <vector>

class CosmosLiveSession {
public:
    CosmosLiveSession();
    CosmosLiveSession(CosmosLiveState state, std::time_t startTimeUTC, double secondsToStart, int round, int roundSecondsRemaining, int playerCount);
    CosmosLiveSession(CosmosLiveState state, std::time_t startTimeUTC, double secondsToStart, int round, int roundSecondsRemaining, int playerCount, std::vector<CosmosLiveChat> chats);
    
    CosmosLiveState GetState() const;
    int GetPlayerCount() const;
    std::vector<CosmosLiveChat> GetChats() const;
    double GetSecondsToStart() const;
    
    std::string GetHash() const;
    
private:
    CosmosLiveState m_state;
    std::time_t m_startTimeUTC;
    double m_secondsToStart;
    int m_round;
    int m_roundSecondsRemaining;
    int m_playerCount;
    std::vector<CosmosLiveChat> m_chats;
};
