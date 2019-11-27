#pragma once
#include <map>
#include "Core/StringManager/StringManager.h"

class NPCProperties {
public:
	bool hasProperty(STRING_ID id) const {
		return mPropertyCollection.find(id) != mPropertyCollection.end();
	}

	STRING_ID getProperty(STRING_ID id) const {
		return mPropertyCollection.find(id)->second;
	}

	void addProperty(STRING_ID id, STRING_ID value) {
		mPropertyCollection[id] = value;
	}

private:
	using PropertyCollection = std::map<STRING_ID, STRING_ID>;
	PropertyCollection mPropertyCollection;
};

class NPCPropertiesProvider {
public:
	virtual const NPCProperties& GetNPCProperties() const = 0;
};
