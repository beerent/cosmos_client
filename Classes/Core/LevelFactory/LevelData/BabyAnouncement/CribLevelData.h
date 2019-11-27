//
//  CribLevelData.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/29/15.
//
//

#ifndef __Projectios2__CribLevelData__
#define __Projectios2__CribLevelData__

#include <stdio.h>

#include "Core/LevelFactory/LevelData/LevelData.h"
#include "Core/Entity/Entities/Entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "IEngine.h"
#include "Core/Entity/Entities/Camera.h"
#include "Core/Rendering/LightingManager/LightingManager.h"
#include "Core/Entity/EntityFactory.h"
#include "Core/GameLogic/ScriptedEventEngine/IScriptedEventEngine.h"
#include "Core/GameLogic/EntityControler/EntityControler.h"
#include "Core/Physics/IPhysicsManager.h"

// Coordinate system
// The in game coordinate system does not match the coordinate system in blender.

// In game - Right-Handed Cartesian Coordinates
// up/down                     y axis +/-  Green
// right/left                  x axis +/-  Red
// back/forward                z axis -/+  Blue

// In blender - Right-Handed Z up coordinate system common to most CAD software
// up/down                     z axis +/-  Blue
// right/left                  x axis +/-  Red
// back/forward                y axis +/-  Green

class CribLevelData : public LevelData
{
public:
    virtual void initLevel(Level &level)
    {
        IEngine *iEngine = IEngine::getEngine();
        LightingManager* lighting = iEngine->getLightingManager();
        IPhysicsManager* physicsManger = iEngine->getPhysicsManager();
        physicsManger->initialize(glm::vec3(0,0,0), false);
        
    
        lighting->setInfinitLightSourceVector(glm::vec3(-1.0, 1.0, -1.0));
        lighting->setAmbientColor(glm::vec3(0.6, 0.6, 0.5));
        lighting->setDiffuseColor(glm::vec3(0.6, 0.6, 0.8));
        lighting->setSpecularColor(glm::vec3(0.9, 0.9, 0.7));
        
        Camera* camera = iEngine->getActiveCamera();
        camera->SetPosition(glm::vec3(0.0,14.5,6.0));
        //camera->SetPosition(glm::vec3(5.0,5.0,24.0));
        //camera->lookAt(glm::vec3(1, 14, 1));
        
        
        
        //KKonecko: actualy Implement real level loading here.
        EntityFactory * entityFactory = EntityFactory::getInstance();
        Entity * entity;
        
        
        /*
         entity = entityFactory->createEntity(StringManager::getIDForString("waterArchetype"));
         entity->SetPosition(glm::vec3(0.0,0.0,0.0));
         entity->setScale(500.0f);
         level.addEntity(entity);
         */
    
    
        entity = entityFactory->createEntity(StringManager::getIDForString("errorCube"));
        EntityControler* controler = EntityControler::Factory(EntityControler::SCRIPT_SHIP_CONTROLLER);
        entity->setControler(controler);
        entity->setScale(glm::vec3(0.001, 0.001, 0.001));
        entity->SetPosition(glm::vec3(8.0,5.0,24.0));
        level.addEntity(entity, STRING_TO_ID("NODE_1"));
        entity = entityFactory->createEntity(StringManager::getIDForString("errorCube"));
        controler = EntityControler::Factory(EntityControler::SCRIPT_SHIP_CONTROLLER);
        entity->setControler(controler);
        entity->SetPosition(glm::vec3(-0.5,14.5,3.0));
        entity->setScale(glm::vec3(0.001, 0.001, 0.001));
        level.addEntity(entity, STRING_TO_ID("NODE_2"));
        
        camera->lookAt(glm::vec3(-0.5,14.5,3.0));
        entity = entityFactory->createEntity(StringManager::getIDForString("mobileGroupArchetype"));
        entity->SetPosition(glm::vec3(0, 20, 0));
        level.addEntity(entity);
        
        
        
        level.setScriptEngine(IScriptedEventEngine::CRIB_SCRIPT);
    }
    
};


#endif /* defined(__Projectios2__CribLevelData__) */
