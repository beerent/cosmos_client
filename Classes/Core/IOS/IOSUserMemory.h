#pragma once

#include <Core/User/IUserMemory.h>
#include "Keychain/PDKeychainBindings.h"
#include <string>

class IOSUserMemory : public IUserMemory {
public:
    IOSUserMemory();
    virtual void StoreUsername(const std::string& username);
    virtual void StorePassword(const std::string& password);
    
    virtual std::string RetrieveUsername();
    virtual std::string RetrievePassword();

private:
    PDKeychainBindings* m_bindings;
};

