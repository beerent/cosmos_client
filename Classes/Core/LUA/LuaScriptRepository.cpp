#include "LuaScriptRepository.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Core/Debug/Logger/Logger.h"

namespace Lua {

	static int l_my_print(lua_State* L) {
		int nargs = lua_gettop(L);

		for (int i = 1; i <= nargs; i++) {
			if (lua_isstring(L, i)) {
				Logger::Log(std::string("LuaPrint ") + lua_tostring(L, i));
			}
			else {
				/* Do something with non-strings if you like */
			}
		}

		return 0;
	}

	static const struct luaL_Reg printlib[] = {
		{ "print", l_my_print },
		{ NULL, NULL } /* end of array */
	};

	void printError(lua_State* state) {
		// The error message is on top of the stack.
		// Fetch it, print it and then pop it off the stack.
		const char* message = lua_tostring(state, -1);
		Logger::Log(message);
		lua_pop(state, 1);
	}

	lua_State* ScriptRepository::getScript(STRING_ID name) {
		std::string path = getScriptPath(name);
		lua_State* luaState = luaL_newstate();
		luaL_openlibs(luaState);

		int loadResult = luaL_loadfile(luaState, path.c_str());
		if (loadResult != LUA_OK) {
			printError(luaState);
		}

		lua_pcall(luaState, 0, LUA_MULTRET, 0);
		if (loadResult != LUA_OK) {
			printError(luaState);
		}

		lua_getglobal(luaState, "_G");
		luaL_setfuncs(luaState, printlib, 0);
		lua_pop(luaState, 1);

		return luaState;
	}

	void ScriptRepository::init(IResourceLoader *resourceLoader) {
		m_resourceLoader = resourceLoader;
	}

	std::string ScriptRepository::getScriptPath(STRING_ID name) {
		std::string nameLowerStr = *StringManager::getStringFromID(name);
		std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
		return m_resourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() + ".lua";
	}
}
