//
//  Level.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 1/3/13.
//
//

#include "Core/LevelFactory/Level.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/GUI/Components/UILabel.h"
#include "Core/GUI/Components/UIComponent.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/StringManager/StringManager.h"
#include "IEngine.h"


const StringManager::StringID Level::CLASS_ID_LEVEL = StringManager::getIDForString("CLASS_ID_LEVEL");

Level::Level():DynamicObject(CLASS_ID_LEVEL), mScriptEngine(NULL), mGameMaster(nullptr)
{
    UIComponentFactory* uiFactory = UIComponentFactory::getInstance();
    mOverlayLabel = static_cast<UILabel*>(uiFactory->createUIComponent(StringManager::getIDForString("UILabelArchetype")));
    

    
    IEngine::getEngine()->getUIRoot()->addChild(mOverlayLabel);
    
}

Level::~Level() {
	if (mGameMaster != nullptr) {
		mGameMaster->stop();
	}
}

void Level::addEntity(Entity *entity)
{
    char buffer[256];
    sprintf(buffer, "AutoGenName_%d", entity->getObjectId());
    STRING_ID name = StringManager::getIDForString(buffer);
    m_entities[name] = entity;
}

void Level::addNPCAI(NPCAI* npcAI) {
	m_npcAICollection.push_back(npcAI);
}

void Level::addEntity(Entity *entity, STRING_ID name)
{
    m_entities[name] = entity;
}

void Level::release()
{
    std::map<STRING_ID, Entity*>::reverse_iterator it = m_entities.rbegin();
    while(it != m_entities.rend())
    {
        it->second->release();
        delete(it->second);
        it++;
    }
    
    m_entities.clear();
    DynamicObject::release();
    if(mScriptEngine)
    {
        mScriptEngine->stop();
        delete (mScriptEngine);
    }
    
    mOverlayLabel->release();
    IEngine::getEngine()->getUIRoot()->removeChild(mOverlayLabel->getID());
    delete mOverlayLabel;

	if (mGameMaster != nullptr) {
		mGameMaster->stop();
	}

	for (auto it = m_npcAICollection.begin(); it != m_npcAICollection.end(); ++it) {
		delete (*it);
	}

	m_npcAICollection.clear();
}

Entity* Level::getEntityByName(STRING_ID name, bool errorOnNotFound)
{
    StrIDToEntityPtrMap::iterator it = m_entities.find(name);
    if(it != m_entities.end())
    {
        return it->second;
    }
    
    if(errorOnNotFound)
    {
        Logger::Error("__FUNCTION__ Could not find requested entity in the current level.");
    }
    return NULL;
}

const Entity* Level::getEntityByName(STRING_ID name, bool errorOnNotFound) const {
	StrIDToEntityPtrMap::const_iterator it = m_entities.find(name);
	if (it != m_entities.end())
	{
		return it->second;
	}

	if (errorOnNotFound)
	{
		Logger::Error("__FUNCTION__ Could not find requested entity in the current level.");
	}
	return NULL;
}

void Level::setScriptEngine(StringManager::StringID name)
{
    mScriptEngine = IScriptedEventEngine::Factory(name);
    mScriptEngine->start();
}

void Level::setGameMaster(IGameMaster* master) {
	mGameMaster = master;
}

void Level::init() {
	if (mGameMaster != nullptr) {
		mGameMaster->start();
	}

	DynamicObject::init();
}
