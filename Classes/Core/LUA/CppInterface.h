#pragma once
#include "lua.hpp"
#include <map>
#include <list>

class ILuaEventHandler {
public:
	virtual void onLuaCallBack(lua_State*) = 0;
};

namespace Lua {
	class CppInterface {
	public:
		static CppInterface& getInstance();
		void registerHandler(ILuaEventHandler& handler, lua_State& luaState);
		void handleLuaCallback(lua_State*);

	private:
		static CppInterface* m_instance;
		//using ILueEventHandlers = std::list<ILuaEventHandler*>;
		using LuaStateToEventHandlerMap = std::map<lua_State*, ILuaEventHandler*>;
		LuaStateToEventHandlerMap m_handlersRegisterdToLuaState;
	};
}
