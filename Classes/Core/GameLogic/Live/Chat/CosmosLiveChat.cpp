#include <Core/GameLogic/Live/Chat/CosmosLiveChat.h>

CosmosLiveChat::CosmosLiveChat(const std::string& user, const std::string& message, int secondsAgo) : m_user(user), m_message(message), m_secondsAgo(secondsAgo) {}

std::string CosmosLiveChat::GetUser() const {
    return m_user;
}

std::string CosmosLiveChat::GetMessage() const {
    return m_message;
}

int CosmosLiveChat::GetSecondsAgo() const {
    return m_secondsAgo;
}

bool CosmosLiveChat::IsValid() const {
    return !m_user.empty();
}
