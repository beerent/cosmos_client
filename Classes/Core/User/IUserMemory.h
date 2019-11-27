#pragma once
#include <string>

class IUserMemory {
public:
    virtual void StoreUsername(const std::string& username) = 0;
    virtual void StorePassword(const std::string& password) = 0;
    
    virtual std::string RetrieveUsername() = 0;
    virtual std::string RetrievePassword() = 0;
};
