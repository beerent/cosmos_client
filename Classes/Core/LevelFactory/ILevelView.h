#pragma once
#include "Core/StringManager/StringManager.h"
#include "Core/Entity/Entities/Entity.h"

class ILevelView {
public:
	virtual Entity* getEntityByName(STRING_ID name, bool errorOnNotFound = true) = 0;
	virtual const Entity* getEntityByName(STRING_ID name, bool errorOnNotFound = true) const = 0;
};
