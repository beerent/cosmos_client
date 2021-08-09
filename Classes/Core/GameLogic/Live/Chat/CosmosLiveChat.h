#pragma once

#include<string>

class CosmosLiveChat {
public:
    CosmosLiveChat(const std::string& user, const std::string& message, int secondsAgo);
    
    std::string GetUser() const;
    std::string GetMessage() const;
    int GetSecondsAgo() const;
    bool IsValid() const;
    
private:
    std::string m_user;
    std::string m_message;
    int m_secondsAgo;
};
