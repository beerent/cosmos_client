#pragma once

#include "../../../Classes/Core/Net/IRestConnector.h"
#include "../../../Classes/Core/Net/IRestReceiver.h"

#include <string>

class WindowsRestConnector : public IRestConnector {

public:
	virtual std::string SendRequest(const std::string& requestUrl, IRestReceiver* restReceiver);

};