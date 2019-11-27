#pragma once

#include <stdio.h>
#include <Core/Net/IRestConnector.h>
#include <Core/Net/IRestReceiver.h>

class IOSRestConnector : public IRestConnector {
public:
    virtual std::string SendRequest(const std::string& request, IRestReceiver* restReceiver);
};
