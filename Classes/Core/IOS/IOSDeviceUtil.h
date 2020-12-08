#pragma once

#include <stdio.h>
#include <Core/General/IDeviceUtil.h>

class IOSDeviceUtil : public IDeviceUtil {
public:
    virtual void RateAppRequest();
};
