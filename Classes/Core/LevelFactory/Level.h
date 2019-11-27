//
//  Level.h
//  Projectios1
//
//  Created by Kamil Konecko on 1/3/13.
//
//

#ifndef Projectios1_Level_h
#define Projectios1_Level_h
#include "Core/Entity/Entities/Entity.h"
#include <vector>
#include <map>
#include "Core/Memory/DynamicObject.h"
#include "Core/StringManager/StringManager.h"
#include "Core/GameLogic/ScriptedEventEngine/IScriptedEventEngine.h"
#include "Core/GameLogic/GameMaster/IGameMaster.h"
#include "Core/Input/InputManager.h" 
#include "core/LevelFactory/ILevelView.h"
#include "core/GameLogic/AI/NPCAI/NPCAI.h"

class UILabel;

class Level : public DynamicObject, public ILevelView
{
public:
    typedef std::map<STRING_ID, Entity *> StrIDToEntityPtrMap;
    
    static const StringManager::StringID CLASS_ID_LEVEL;

    StrIDToEntityPtrMap& getEntities(){return m_entities;}
    Entity* getEntityByName(STRING_ID name, bool errorOnNotFound = true);
	const Entity* getEntityByName(STRING_ID name, bool errorOnNotFound = true) const;

    Level();
	~Level();

    STRING_ID mLevelName;

    void addEntity(Entity * entity, STRING_ID name);
    void addEntity(Entity * entity);
	void addNPCAI(NPCAI* npcAI);
    
    void setScriptEngine(STRING_ID);
	void setGameMaster(IGameMaster* master);
    
    void release();
    UILabel& getOverlayLabel(){return *mOverlayLabel;}

	void init();

	void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data) { 
		if (mGameMaster != nullptr) {
			mGameMaster->onInputEvent(event, data);
		}
	};

private:

	StrIDToEntityPtrMap m_entities;
	IScriptedEventEngine* mScriptEngine;
	UILabel* mOverlayLabel;
	IGameMaster* mGameMaster;
	std::list<NPCAI*> m_npcAICollection;

};
#endif
