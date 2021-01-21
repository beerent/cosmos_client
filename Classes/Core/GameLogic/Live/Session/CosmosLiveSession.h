#pragma once

#import <Core/GameLogic/Live/CosmosLiveStates.h>

#include <ctime>

#import <string>
#import <list>

class CosmosLiveSession {
public:
    CosmosLiveSession();
    CosmosLiveSession(CosmosLiveState state, std::time_t startTimeUTC, int round, int roundSecondsRemaining, int playerCount);
    
    CosmosLiveState GetState() const;
    int GetPlayerCount() const;
    
    std::string GetHash() const;
    
private:
    CosmosLiveState m_state;
    std::time_t m_startTimeUTC;
    int m_round;
    int m_roundSecondsRemaining;
    int m_playerCount;
};
