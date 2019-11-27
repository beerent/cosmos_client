#include "core/GameLogic/AI/AIBehavior/ScriptedAIBehavior.h"
#include "core/Debug/Logger/Logger.h"
#include <sstream>

CONST_STRING_DEF(ScriptedAIBehavior, SCRIPT_NAME)

ScriptedAIBehavior::ScriptedAIBehavior(const NPCPropertiesProvider& npcProperties, ILevelView& levelView, const PropertyCollection& properties) : AIBehavior(npcProperties, levelView) {
	STRING_ID scriptName;
	properties.getValueForProperty(SCRIPT_NAME, scriptName);

	m_luaState = IEngine::getEngine()->getLuaScriptRepository().getScript(scriptName);
	Lua::CppInterface::getInstance().registerHandler(*this, *m_luaState);
	setup();
}

void ScriptedAIBehavior::onLuaCallBack(lua_State* luaState) {
	Logger::Log("onLuaCallBack ");
	std::string functionName = luaL_checkstring(luaState, 1);
	if (functionName == "GetNPCProperty") {
		LuaGetNPCProperty(luaState);
	}
}

void ScriptedAIBehavior::LuaGetNPCProperty(lua_State* luaState) {
	std::string npcPropertyName = luaL_checkstring(luaState, 2);
	STRING_ID instanceNameID = GetNPCProperty(STRING_TO_ID(npcPropertyName));
	lua_pushstring(luaState, ID_TO_CSTR(instanceNameID));
}

ScriptedAIBehavior::~ScriptedAIBehavior() {
	lua_close(m_luaState);
}

void ScriptedAIBehavior::StartBehavior() {
}

void ScriptedAIBehavior::OnTick() {
	lua_getglobal(m_luaState, "onTick");
	if (lua_pcall(m_luaState, 0, 0, 0) != LUA_OK) {
		Lua::printError(m_luaState);
	}
}

void ScriptedAIBehavior::setup() {
	lua_getglobal(m_luaState, "setup");
	if (lua_pcall(m_luaState, 0, 0, 0) != LUA_OK) {
		Lua::printError(m_luaState);
	}
}
