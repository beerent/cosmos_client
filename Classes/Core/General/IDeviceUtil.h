#pragma once

#include <string>

class IDeviceUtil {
public:
    virtual void RateAppRequest() = 0;
    virtual std::string GetBuildVersion() = 0;
    virtual void WriteToDeviceStorage(const std::string& key, const std::string& value) = 0;
    virtual std::string ReadFromDeviceStorage(const std::string& key) = 0;
    virtual void OpenWebsite(const std::string& url) = 0;
    virtual std::string GetDeviceUid() = 0;
};
