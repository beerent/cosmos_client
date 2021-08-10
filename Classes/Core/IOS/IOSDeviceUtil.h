#pragma once

#include <stdio.h>
#include <Core/General/IDeviceUtil.h>
#include "Keychain/PDKeychainBindings.h"
#include <string>

class IOSDeviceUtil : public IDeviceUtil {
public:
    IOSDeviceUtil();
    
    virtual void RateAppRequest();
    virtual std::string GetBuildVersion();
    virtual void WriteToDeviceStorage(const std::string& key, const std::string& value);
    virtual std::string ReadFromDeviceStorage(const std::string& key);
    virtual void OpenWebsite(const std::string& url);
    virtual std::string GetDeviceUid();
    
private:
    PDKeychainBindings* m_bindings;
};
