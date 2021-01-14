#pragma once

#include <string>

enum class CosmosLiveState {
    INVALID,
    CLOSED,
    PRE_GAME_LOBBY,
    IN_GAME,
    POST_GAME_LOBBY
};

CosmosLiveState StringToCosmosLiveState(const std::string& string);
std::string CosmosLiveStateToString(const CosmosLiveState& state);
