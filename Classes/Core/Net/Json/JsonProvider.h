#pragma once

#include "json11.hpp"
#include <string>

namespace JsonProvider {
	static json11::Json ParseString(const std::string& jsonToParse) {
		std::string error;

		json11::Json json = json11::Json::parse(jsonToParse, error);
		return json;
	}
}