//
//  Settings.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/2/14.
//
//

#ifndef Projectios1_Settings_h
#define Projectios1_Settings_h

#include "Core/StringManager/StringManager.h"
#include <map>

class Settings
{
public:
    CONST_STRING_DEC(SETTING_SHOW_PHYSICS_DEBUG_INFO);
    CONST_STRING_DEC(SETTING_SHOW_PHYSICS_VOLUME_OBJECTS);
    CONST_STRING_DEC(SETTING_SHOW_OBJECT_AXIS);
    CONST_STRING_DEC(SETTING_SHOW_OBJECT_NAMES);
    
    static Settings* GetInstance();
    
    typedef std::map<STRING_ID, bool> STRING_IDtoBoolMap;
    
    Settings();
    
    static bool getToggleSetting(STRING_ID name);
    static void setToggleSetting(STRING_ID name, bool value);
    
private:
    static Settings* mInstance;
    STRING_IDtoBoolMap mToggleSettings;
    
    
};
#endif
