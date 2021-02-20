#pragma once

#include<string>

class CosmosLiveChat {
public:
    CosmosLiveChat(const std::string& user, const std::string& message, const std::string& added);
    
    std::string GetUser() const;
    std::string GetMessage() const;
    std::string GetAdded() const;
    bool IsValid() const;
    
private:
    std::string m_user;
    std::string m_message;
    std::string m_added;
};
