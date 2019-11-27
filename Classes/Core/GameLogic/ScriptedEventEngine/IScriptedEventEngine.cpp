#include "Core/GameLogic/ScriptedEventEngine/IScriptedEventEngine.h"
#include "KYCPlanetSizeSweepScript.h"
#include "Core/Debug/Logger/Logger.h"

CONST_STRING_DEF(IScriptedEventEngine, KYC_PLANET_SIZE_SWEEP_SCRIPT)

IScriptedEventEngine* IScriptedEventEngine::Factory(STRING_ID name){
	if (name == KYC_PLANET_SIZE_SWEEP_SCRIPT) {
		return new KYCPlanetSizeSweepScript();
	}

    sprintf(Logger::GetTempCharBuffer(), "__FUNCTION__ Unknown event script name %s",
            StringManager::getStringFromID(name)->c_str());
    Logger::Error(Logger::GetTempCharBuffer());
    
    return NULL;
}
