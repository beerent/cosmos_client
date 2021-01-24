#include <Core/GameLogic/Live/Chat/CosmosLiveChat.h>

CosmosLiveChat::CosmosLiveChat(const std::string& user, const std::string& message, const std::string& added) : m_user(user), m_message(message), m_added(added) {}

std::string CosmosLiveChat::GetUser() const {
    return m_user;
}

std::string CosmosLiveChat::GetMessage() const {
    return m_message;
}

std::string CosmosLiveChat::GetAdded() const {
    return m_added;
}
