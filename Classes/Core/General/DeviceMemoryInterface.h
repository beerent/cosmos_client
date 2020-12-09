#pragma once

#include <string>

class DeviceMemoryInterface {
public:
    //username
    void StoreUsername(const std::string& username);
    std::string ReadUsername();
    
    void StoreRememberUsername(bool remember);
    bool ReadRememberUsername();
};
