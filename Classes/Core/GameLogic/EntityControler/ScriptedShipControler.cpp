//
//  ScriptedShipControler.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#include "ScriptedShipControler.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/Events/GameEvents.h"

void ScriptedShipControler::move(float speed, float velocity, bool useVelocity, STRING_ID type)
{
    GameEvents::PropultionEnginePowerEvent event(type, speed, velocity, useVelocity);
    mEntity->dispatchEvent(event);
}

void ScriptedShipControler::fire(glm::vec3 targetPos, bool targeted, STRING_ID type, STRING_ID position)
{
    {
        GameEvents::FireEvent event(position, targetPos, targeted, type);
        mEntity->dispatchEvent(event);
    }
}