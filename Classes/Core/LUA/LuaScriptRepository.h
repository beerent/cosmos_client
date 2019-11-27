#pragma once
#include "Core/StringManager/StringManager.h"
#include <map>
#include "Interface.hpp"
#include "lua.hpp"

namespace Lua {
	void printError(lua_State* state);

	class ScriptRepository {
	public:
		void init(IResourceLoader *resourceLoader);
		lua_State* getScript(STRING_ID name);

	private:
		std::string getScriptPath(STRING_ID name);
		IResourceLoader* m_resourceLoader;
	};
}
