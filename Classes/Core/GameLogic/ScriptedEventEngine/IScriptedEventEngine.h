//
//  IScriptedEventEngine.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#ifndef __Projectios2__IScriptedEventEngine__
#define __Projectios2__IScriptedEventEngine__

#include <stdio.h>
#include "Core/StringManager/StringManager.h"

class IScriptedEventEngine
{
public:
    CONST_STRING_DEC(SPACE_BATTLE_SCRIPT)
    CONST_STRING_DEC(CRIB_SCRIPT)
	CONST_STRING_DEC(KYC_PLANET_SIZE_SWEEP_SCRIPT)

    static IScriptedEventEngine* Factory(STRING_ID name);
    
    virtual void start() = 0;
    virtual void stop() = 0;
    
    virtual ~IScriptedEventEngine(){};
};

#endif /* defined(__Projectios2__IScriptedEventEngine__) */
