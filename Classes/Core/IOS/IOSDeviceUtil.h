#pragma once

#include <stdio.h>
#include <Core/General/IDeviceUtil.h>
#include <string>

class IOSDeviceUtil : public IDeviceUtil {
public:
    virtual void RateAppRequest();
    virtual std::string GetBuildVersion();
};
