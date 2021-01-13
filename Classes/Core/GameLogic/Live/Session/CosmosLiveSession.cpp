#include <Core/GameLogic/Live/Session/CosmosLiveSession.h>

CosmosLiveState GetCosmosLiveState(const std::string& s) {
    if (s == "INVALID") {
        return CosmosLiveState::INVALID;
    }
    
    if (s == "CLOSED") {
        return CosmosLiveState::CLOSED;
    }
    
    if (s == "PRE_GAME_LOBBY") {
        return CosmosLiveState::PRE_GAME_LOBBY;
    }
    
    if (s == "IN_GAME") {
        return CosmosLiveState::IN_GAME;
    }
    
    if (s == "POST_GAME_LOBBY") {
        return CosmosLiveState::POST_GAME_LOBBY;
    }
    
    return CosmosLiveState::INVALID;
}

