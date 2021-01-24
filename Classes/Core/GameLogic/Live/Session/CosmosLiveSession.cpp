#include <Core/GameLogic/Live/Session/CosmosLiveSession.h>
#include <Core/GameLogic/Live/CosmosLiveStates.h>
#include <Util/Date/DateUtil.h>

CosmosLiveSession::CosmosLiveSession()
  : m_state(CosmosLiveState::INVALID), m_startTimeUTC(Util::StringToDateTime("0000-00-00T00:00:00Z")), m_round(0), m_roundSecondsRemaining(0), m_playerCount(0) {}

CosmosLiveSession::CosmosLiveSession(CosmosLiveState state, std::time_t startTimeUTC, int round, int roundSecondsRemaining, int playerCount)
  : m_state(state), m_startTimeUTC(startTimeUTC), m_round(round), m_roundSecondsRemaining(roundSecondsRemaining), m_playerCount(playerCount) {}

CosmosLiveSession::CosmosLiveSession(CosmosLiveState state, std::time_t startTimeUTC, int round, int roundSecondsRemaining, int playerCount, std::vector<CosmosLiveChat> chats)
  : m_state(state), m_startTimeUTC(startTimeUTC), m_round(round), m_roundSecondsRemaining(roundSecondsRemaining), m_playerCount(playerCount), m_chats(chats) {}

CosmosLiveState CosmosLiveSession::GetState() const {
    return m_state;
}

int CosmosLiveSession::GetPlayerCount() const {
    return m_playerCount;
}

std::vector<CosmosLiveChat> CosmosLiveSession::GetChats() const {
    return m_chats;
}

std::string CosmosLiveSession::GetHash() const {
    std::string hash = CosmosLiveStateToString(m_state);
    hash += std::to_string(m_startTimeUTC);
    hash += std::to_string(m_round);
    hash += std::to_string(m_roundSecondsRemaining);
    hash += std::to_string(m_playerCount);
    for (const auto& chat : m_chats) {
        hash += (chat.GetUser() + chat.GetMessage() + chat.GetAdded());
    }
    
    return hash;
}
