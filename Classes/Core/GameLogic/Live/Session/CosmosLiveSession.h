#pragma once

#import <Core/GameLogic/Live/CosmosLiveStates.h>

#include <ctime>

#import <string>
#import <list>

struct CosmosLiveSession {
    CosmosLiveSession(CosmosLiveState state, std::time_t startTimeUTC, int round, int roundSecondsRemaining, int playerCount) : state(state), startTimeUTC(startTimeUTC), round(round), roundSecondsRemaining(roundSecondsRemaining), playerCount(playerCount) {}
    CosmosLiveState state;
    std::time_t startTimeUTC;
    int round;
    int roundSecondsRemaining;
    int playerCount;
};
