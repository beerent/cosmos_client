//
//  Settings.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/2/14.
//
//

#include "Core/Settings/Settings.h"
#include "Core/Debug/Logger/Logger.h"

CONST_STRING_DEF(Settings, SETTING_SHOW_PHYSICS_DEBUG_INFO);
CONST_STRING_DEF(Settings, SETTING_SHOW_PHYSICS_VOLUME_OBJECTS);
CONST_STRING_DEF(Settings, SETTING_SHOW_OBJECT_AXIS);
CONST_STRING_DEF(Settings, SETTING_SHOW_OBJECT_NAMES);

Settings* Settings::mInstance = NULL;

Settings* Settings::GetInstance()
{
    if(mInstance == NULL)
    {
        mInstance = new Settings();
    }
    
    return mInstance;
}

Settings::Settings()
{
    mToggleSettings[SETTING_SHOW_PHYSICS_DEBUG_INFO] = false;
    mToggleSettings[SETTING_SHOW_PHYSICS_VOLUME_OBJECTS] = false;
    mToggleSettings[SETTING_SHOW_OBJECT_AXIS] = false;
    mToggleSettings[SETTING_SHOW_OBJECT_NAMES] = false;
}

bool Settings::getToggleSetting(STRING_ID name)
{
    Settings* instance = GetInstance();
    
    STRING_IDtoBoolMap::const_iterator it = instance->mToggleSettings.find(name);
    Logger::Assert(it == instance->mToggleSettings.end(), "Invalid Setting Name");
    return (*it).second;
}

void Settings::setToggleSetting(STRING_ID name, bool value)
{
    Settings* instance = GetInstance();

    STRING_IDtoBoolMap::iterator it = instance->mToggleSettings.find(name);
    Logger::Assert(it == instance->mToggleSettings.end(), "Invalid Setting Name");
    (*it).second = value;
}
