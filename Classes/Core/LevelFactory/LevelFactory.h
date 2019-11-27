//
//  LevelFactory.h
//  Projectios1
//
//  Created by Kamil Konecko on 1/3/13.
//
//

#ifndef Projectios1_LevelFactory_h
#define Projectios1_LevelFactory_h
#include "Core/LevelFactory/Level.h"
#include "Core/StringManager/StringManager.h"
#include "Interface.hpp"

class LevelFactory
{
public:
    static const StringManager::StringID MAIN_MENU_LEVEL_ID;
	static const StringManager::StringID GAME_TEST_LEVEL_ID;
	static const StringManager::StringID LOGIN_LEVEL_ID;
    static const StringManager::StringID ART_TEST_LEVEL_ID;

	CONST_STRING_DEC(KYC_PLANET_SIZE_LEVEL_DATA_ID)

private:
    static LevelFactory * m_instance;
    IResourceLoader *m_resourceLoader;
    Level *m_activeLevel;

public:
    void init(IResourceLoader *resourceLoader);
    bool LevelIsSet();
	void clearCurrentLevel();
    Level* LoadLevel(StringManager::StringID name);
    static LevelFactory * getInstance(){ return m_instance;};
    static Level* getActiveLevel();
};

#endif
