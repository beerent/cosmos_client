#pragma once

#include <string>

class IDeviceUtil {
public:
    virtual void RateAppRequest() = 0;
    virtual std::string GetBuildVersion() = 0;
};
