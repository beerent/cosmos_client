#include <Core/GameLogic/Live/CosmosLiveStates.h>

CosmosLiveState StringToCosmosLiveState(const std::string& string) {
    if (string == "INVALID") {
        return CosmosLiveState::INVALID;
    }
    
    if (string == "CLOSED") {
        return CosmosLiveState::CLOSED;
    }
    
    if (string == "PRE_GAME_LOBBY") {
        return CosmosLiveState::PRE_GAME_LOBBY;
    }
    
    if (string == "IN_GAME") {
        return CosmosLiveState::IN_GAME;
    }
    
    if (string == "POST_GAME_LOBBY") {
        return CosmosLiveState::POST_GAME_LOBBY;
    }
    
    return CosmosLiveState::INVALID;
}

std::string CosmosLiveStateToString(const CosmosLiveState& state) {
    switch(state) {
        case CosmosLiveState::INVALID:
            return "INVALID";
            
        case CosmosLiveState::CLOSED:
            return "CLOSED";
            
        case CosmosLiveState::PRE_GAME_LOBBY:
            return "PRE_GAME_LOBBY";
            
        case CosmosLiveState::IN_GAME:
            return "IN_GAME";
            
        case CosmosLiveState::POST_GAME_LOBBY:
            return "POST_GAME_LOBBY";
    }
    
    return "INVALID";
}
