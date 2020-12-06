#pragma once

#include <stdio.h>
#include <Core/GameLogic/Rate/IRateAppController.h>

class IOSRateAppController : public IRateAppController {
public:
    virtual void RateAppRequest();
};
