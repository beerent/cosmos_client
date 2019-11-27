//
//  EntityControler.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#ifndef __Projectios2__EntityControler__
#define __Projectios2__EntityControler__

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "../../../glm/glm.hpp"
#include "Core/Input/InputManager.h"

class Entity;

class EntityControler
{
public:
    
    EntityControler() :mEntity(NULL) {}
    virtual ~EntityControler(){};
    
    CONST_STRING_DEC(SCRIPT_SHIP_CONTROLLER);
	CONST_STRING_DEC(CHARACTER_CONTROLLER);
	CONST_STRING_DEC(AI_CHARACTER_CONTROLLER);

    static EntityControler* Factory(STRING_ID name);
    
    void setSlaveEntity(Entity* slave){mEntity = slave;};
    
    virtual void move(float speed){ move(speed, 0, false, STRING_TO_ID("000"));};
    virtual void move(float speed, float velocity, bool useVelocity){move(speed, velocity, useVelocity, STRING_TO_ID("000"));};
    virtual void move(float speed, float velocity, bool useVelocity, STRING_ID type){};
    virtual void fire(){ fire(glm::vec3(0,0,0), false, StringManager::UNDEFINED, STRING_TO_ID("000"));};
    virtual void fire(glm::vec3 targetPos, bool targeted, STRING_ID type, STRING_ID position){};

	virtual void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data) {};

protected:
    Entity* mEntity;
    
};

#endif /* defined(__Projectios2__EntityControler__) */
