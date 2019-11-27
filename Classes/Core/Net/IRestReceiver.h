#pragma once

#include <string>

class IRestReceiver {
public:
	virtual void RestReceived(const std::string& rest) = 0;
};
