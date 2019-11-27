#pragma once

#include "../../../Classes/Core/User/IUserMemory.h"

class WindowsUserMemory : public IUserMemory {
public:
	virtual void StoreUsername(const std::string& username);
	virtual void StorePassword(const std::string& password);

	virtual std::string RetrieveUsername();
	virtual std::string RetrievePassword();
};