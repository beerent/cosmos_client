#include <Core/General/DeviceMemoryInterface.h>
#include <IEngine.h>

static const std::string USERNAME = "username";
static const std::string REMEMBER_USERNAME = "remember_username";
static const std::string REMEMBER_USERNAME_TRUE = "1";
static const std::string REMEMBER_USERNAME_FALSE = "0";


void DeviceMemoryInterface::StoreUsername(const std::string& username) {
    IEngine::getEngine()->GetDeviceUtil()->WriteToDeviceStorage(USERNAME, username);
}

std::string DeviceMemoryInterface::ReadUsername() {
    return IEngine::getEngine()->GetDeviceUtil()->ReadFromDeviceStorage(USERNAME);
}

void DeviceMemoryInterface::StoreRememberUsername(bool remember) {
    std::string rememberStr = REMEMBER_USERNAME_FALSE;
    if (remember) {
        rememberStr = REMEMBER_USERNAME_TRUE;
    }
    
    IEngine::getEngine()->GetDeviceUtil()->WriteToDeviceStorage(REMEMBER_USERNAME, rememberStr);
}

bool DeviceMemoryInterface::ReadRememberUsername() {
    return REMEMBER_USERNAME_TRUE == IEngine::getEngine()->GetDeviceUtil()->ReadFromDeviceStorage(REMEMBER_USERNAME);
}
