#pragma once
#include "core/GameLogic/AI/AIBehavior/AIBehavior.h"
#include "core/GameLogic/EntityControler/CharacterControler.h"
#include "IEngine.h"
#include "core/LUA/LuaScriptRepository.h"
#include "lua.hpp"
extern "C" {
#include "Core/LUA/LuaPrintOutputRedirect.h"
}
#include "core/LUA/CppInterface.h"


class ScriptedAIBehavior : public AIBehavior, public ILuaEventHandler {
public:
	CONST_STRING_DEC(SCRIPT_NAME)
	ScriptedAIBehavior(const NPCPropertiesProvider& npcProperties, ILevelView& levelView, const PropertyCollection& properties);
	~ScriptedAIBehavior();
	void OnTick();
	void StartBehavior();
	void StopBehavior() {};
	void onLuaCallBack(lua_State*);

private:
	std::string m_scriptSource;
	lua_State *m_luaState;
	void setup();
	void LuaGetNPCProperty(lua_State*);

};