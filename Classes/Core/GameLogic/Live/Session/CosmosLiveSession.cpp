#include <Core/GameLogic/Live/Session/CosmosLiveSession.h>
#include <Core/GameLogic/Live/CosmosLiveStates.h>
#include <Util/Date/DateUtil.h>

CosmosLiveSession::CosmosLiveSession()
  : m_state(CosmosLiveState::INVALID), m_startTimeUTC(Util::StringToDateTime("0000-00-00T00:00:00Z")), m_round(0), m_roundSecondsRemaining(0), m_playerCount(0) {}

CosmosLiveSession::CosmosLiveSession(CosmosLiveState state, std::time_t startTimeUTC, int round, int roundSecondsRemaining, int playerCount)
  : m_state(state), m_startTimeUTC(startTimeUTC), m_round(round), m_roundSecondsRemaining(roundSecondsRemaining), m_playerCount(playerCount) {}

CosmosLiveState CosmosLiveSession::GetState() const {
    return m_state;
}

int CosmosLiveSession::GetPlayerCount() const {
    return m_playerCount;
}

std::string CosmosLiveSession::GetHash() const {
    return CosmosLiveStateToString(m_state) + std::to_string(m_startTimeUTC) + std::to_string(m_round) + std::to_string(m_roundSecondsRemaining) + std::to_string(m_playerCount);
}
