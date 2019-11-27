#include "Core/LevelFactory/LevelFactory.h"
#include "Core/LevelFactory/LevelData/LevelData.h"
#include "Core/LevelFactory/LevelData/GameTestLevelData.h"
#include "Core/LevelFactory/LevelData/MainMenuLevelData.h"
#include "Core/LevelFactory/LevelData/LoginLevelData.h"
#include "Core/LevelFactory/LevelData/ArtTestLevelData.h"
#include "Core/GameLogic/ProjectileManager.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/Debug/Debug3DMathVisualizer.h"
#include "Core/LevelFactory/LevelData/KYCPlanetSizeSweepLevelData.h"

LevelFactory* LevelFactory::m_instance = NULL;

const StringManager::StringID LevelFactory::MAIN_MENU_LEVEL_ID = StringManager::getIDForString("MainMenuLevel");
const StringManager::StringID LevelFactory::GAME_TEST_LEVEL_ID = StringManager::getIDForString("GameTestLevel");
const StringManager::StringID LevelFactory::LOGIN_LEVEL_ID = StringManager::getIDForString("LoginLevel");
const StringManager::StringID LevelFactory::ART_TEST_LEVEL_ID = StringManager::getIDForString("ArtTestLevel");
const StringManager::StringID LevelFactory::KYC_PLANET_SIZE_LEVEL_DATA_ID = StringManager::getIDForString("KYCPlanetSizeLevel");

void LevelFactory::init(IResourceLoader *resourceLoader) {
    m_resourceLoader = resourceLoader;
    m_instance = this;
    m_activeLevel = NULL;
}

bool LevelFactory::LevelIsSet() {
    return m_activeLevel != NULL;
}

void LevelFactory::clearCurrentLevel() {
	if (m_activeLevel == NULL) {
		return;
	}

	Debug3DMathVisualizer::instance().reset();
	ProjectileManager::getInstance()->Reset();
	m_activeLevel->release();
	delete m_activeLevel;
	m_activeLevel = nullptr;
}

Level* LevelFactory::LoadLevel(StringManager::StringID name) {
	clearCurrentLevel();

    Level * level = new Level();
 
    Debug3DMathVisualizer::instance().init(level);
    
	if (name == MAIN_MENU_LEVEL_ID) {
		MainMenuLevelData levelData;
		levelData.initLevel(*level);

	} else if (name == LOGIN_LEVEL_ID) {
		LoginLevelData levelData;
		levelData.initLevel(*level);

    } else if(name == GAME_TEST_LEVEL_ID) {
        GameTestLevelData levelData;
        levelData.initLevel(*level);

    } else if(name == ART_TEST_LEVEL_ID) {
        ArtTestLevelData levelData;
        levelData.initLevel(*level);

    } else if (name == KYC_PLANET_SIZE_LEVEL_DATA_ID) {
		KYCPlanetSizeSweepLevelData levelData;
		levelData.initLevel(*level);

	} else {
        Logger::Error("Level Not Found");
    }
    
    level->mLevelName = name;
    level->init();

    m_activeLevel = level;
    return level;
}

Level* LevelFactory::getActiveLevel() {
    if(m_instance == NULL) {
        Logger::Error("LevelFactory is not initialized!");
    }
    
    return m_instance->m_activeLevel;
};
