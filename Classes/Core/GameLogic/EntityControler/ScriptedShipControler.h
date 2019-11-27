//
//  ScriptedShipControler.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#ifndef __Projectios2__ScriptedShipControler__
#define __Projectios2__ScriptedShipControler__

#include <stdio.h>
#include "Core/GameLogic/EntityControler/EntityControler.h"

class ScriptedShipControler : public EntityControler
{
public:
    void move(float speed, float velocity, bool useVelocity, STRING_ID type);
    void fire(glm::vec3 targetPos, bool targeted, STRING_ID type, STRING_ID position);
};

#endif /* defined(__Projectios2__ScriptedShipControler__) */
