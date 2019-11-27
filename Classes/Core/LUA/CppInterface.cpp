#include "CppInterface.h"
#include "Core/Debug/Logger/Logger.h"

namespace Lua {
	CppInterface* CppInterface::m_instance = nullptr;

	CppInterface& CppInterface::getInstance() {
		if (nullptr == m_instance) {
			m_instance = new CppInterface();
		}

		return *m_instance;
	}

	static int LuaCallBack(lua_State *luaState) {
		CppInterface::getInstance().handleLuaCallback(luaState);
		return 1;
	}

	void CppInterface::handleLuaCallback(lua_State* luaState) {
		LuaStateToEventHandlerMap::iterator it = m_handlersRegisterdToLuaState.find(luaState);
		if (it != m_handlersRegisterdToLuaState.end()) {
			it->second->onLuaCallBack(luaState);
		}
		else {
			Logger::Log("Handler not registered for call back ");
		}
	}

	void CppInterface::registerHandler(ILuaEventHandler& handler, lua_State& luaState) {
		lua_getglobal(&luaState, "_G");
		//luaL_setfuncs(&luaState, printlib, 0);
		lua_pushcfunction(&luaState, LuaCallBack);
		lua_setglobal(&luaState, "LuaCallBack");
		lua_pop(&luaState, 1);

		m_handlersRegisterdToLuaState[&luaState] = &handler;
	}
}
