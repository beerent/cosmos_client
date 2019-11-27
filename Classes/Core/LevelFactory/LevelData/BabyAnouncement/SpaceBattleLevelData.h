//
//  GameTestLevel.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/16/13.
//
//

#ifndef Projectios1_SpaceBattleDataLevel_h
#define Projectios1_SpaceBattleDataLevel_h
#include "Core/LevelFactory/LevelData/LevelData.h"
#include "Core/Entity/Entities/Entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "IEngine.h"
#include "Core/Entity/Entities/Camera.h"
#include "Core/Debug/Debug3DMathVisualizer.h"
#include "Core/Rendering/LightingManager/LightingManager.h"
#include "Core/Physics/IPhysicsManager.h"
#include "Core/GameLogic/ScriptedEventEngine/IScriptedEventEngine.h"
#include "Core/GameLogic/EntityControler/EntityControler.h"
#include "Core/Entity/EntityFactory.h"

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

class SpaceBattleLevelData : public LevelData
{
public:
    virtual void initLevel(Level &level)
    {
        IEngine *iEngine = IEngine::getEngine();
        LightingManager* lighting = iEngine->getLightingManager();
        IPhysicsManager* physicsManger = iEngine->getPhysicsManager();
        physicsManger->initialize(glm::vec3(0,0,0), false);
        /*
        */
        lighting->setInfinitLightSourceVector(glm::vec3(-1.0, 1.0, -1.0));
        lighting->setAmbientColor(glm::vec3(0.08, 0.08, 0.45));
        lighting->setDiffuseColor(glm::vec3(0.55, 0.55, 0.45));
        lighting->setSpecularColor(glm::vec3(0.75, 0.7, 0.6));
        
        Camera* camera = iEngine->getActiveCamera();
        camera->SetPosition(glm::vec3(0.0,0.0, 200.0));
        glm::quat camRotation = glm::angleAxis<float>(25,1.0,0.0,0.0);
        camera->SetRotation(camRotation);
        
        //KKonecko: actualy Implement real level loading here.
        EntityFactory * entityFactory = EntityFactory::getInstance();
        Entity * entity;
        
        /*
        entity = entityFactory->createEntity(StringManager::getIDForString("waterArchetype"));
        entity->SetPosition(glm::vec3(0.0,0.0,0.0));
        entity->setScale(500.0f);
        level.addEntity(entity);
        */
        

        //entity = entityFactory->createEntity(StringManager::getIDForString("skyBoxArchetype"));
        entity = entityFactory->createEntity(StringManager::getIDForString("spaceSphereArchetype"));
        glm::quat rotation = glm::angleAxis<float>(90.0,1.0,0.0,0.0);
        entity->SetRotation(rotation);
        entity->setScale(1.0f);
        entity->SetPosition(glm::vec3(0.0,0.0,0.0));
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("spaceSphereArchetype"));
        rotation = glm::angleAxis<float>(-90.0,1.0,0.0,0.0);
        entity->SetRotation(rotation);
        entity->setScale(1.0f);
        entity->SetPosition(glm::vec3(0.0,0.0,0.0));
        level.addEntity(entity);
        
   
        entity = entityFactory->createEntity(StringManager::getIDForString("tarsusArchetype"));
        entity->SetPosition(glm::vec3(4.0,-1.0,210.0));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        entity->setVelocity(glm::vec3(0.0,0.0,-5.0));
        
        EntityControler* controler = EntityControler::Factory(EntityControler::SCRIPT_SHIP_CONTROLLER);
        entity->setControler(controler);
        level.addEntity(entity, StringManager::getIDForString("Tarsus_A"));
        
  
        entity = entityFactory->createEntity(StringManager::getIDForString("tarsusArchetype"));
        entity->SetPosition(glm::vec3(2.0,-4.0,212.0));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        entity->setVelocity(glm::vec3(0.0,0.0,-5.0));
        
        controler = EntityControler::Factory(EntityControler::SCRIPT_SHIP_CONTROLLER);
        entity->setControler(controler);
        level.addEntity(entity, StringManager::getIDForString("Tarsus_B"));
        

        
        entity = entityFactory->createEntity(StringManager::getIDForString("tarsusArchetype"));
        entity->SetPosition(glm::vec3(-5.0,-6.0,220.0));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        entity->setVelocity(glm::vec3(0.0,0.0,-5.0));
        
        controler = EntityControler::Factory(EntityControler::SCRIPT_SHIP_CONTROLLER);
        entity->setControler(controler);
        level.addEntity(entity, StringManager::getIDForString("Tarsus_C"));
        
        
          /*
        entity = entityFactory->createEntity(StringManager::getIDForString("tarsusArchetype"));
        entity->SetPosition(glm::vec3(40.0,0.0,20.0));
        rotation = glm::angleAxis<float>(40.0,0.0,1.0,0.0);
        controler = EntityControler::Factory(EntityControler::SCRIPT_SHIP_CONTROLLER);
        entity->setControler(controler);
        level.addEntity(entity, StringManager::getIDForString("Tarsus_B"));

        entity = entityFactory->createEntity(StringManager::getIDForString("tarsusArchetype"));
        entity->SetPosition(glm::vec3(40.0,-25.0,0.0));
        rotation = glm::angleAxis<float>(40.0,0.0,1.0,0.0);
        controler = EntityControler::Factory(EntityControler::SCRIPT_SHIP_CONTROLLER);
        entity->setControler(controler);
        level.addEntity(entity, StringManager::getIDForString("Tarsus_C"));

        entity = entityFactory->createEntity(StringManager::getIDForString("EnterpriseArchetype"));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        controler = EntityControler::Factory(EntityControler::SCRIPT_SHIP_CONTROLLER);
        entity->setControler(controler);
        level.addEntity(entity, StringManager::getIDForString("Enterprise"));
        
        
   
         
         
        entity = entityFactory->createEntity(StringManager::getIDForString("XCRXtarsusArchetype"));
        entity->SetPosition(glm::vec3(10.0,0.0,-20.0));
        rotation = glm::angleAxis<float>(25.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);

        entity = entityFactory->createEntity(StringManager::getIDForString("catDudeArchetype"));
        entity->SetPosition(glm::vec3(0.0,0.0,0.0));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("barrelArcheType"));
        entity->SetPosition(glm::vec3(0.0,0.0,-1.0));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        
        
        
        entity = entityFactory->createEntity(StringManager::getIDForString("elAmarilloArchetype"));
        entity->SetPosition(glm::vec3(0.0,0.0,15.0));
        rotation = glm::angleAxis<float>(230.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        entity = entityFactory->createEntity(StringManager::getIDForString("firstIslandArchetype"));
        entity->SetPosition(glm::vec3(50.0,0.0,0.0));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
  */
        
/*
        
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                entity = entityFactory->createEntity(StringManager::getIDForString("firstIslandArchetype"));
                entity->SetPosition(glm::vec3(50.0 + (i * 70),0.0 ,0.0 + (j * 70)));
                rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
                entity->SetRotation(rotation);
                level.addEntity(entity);
            }
        }
        
 
        entity = entityFactory->createEntity(StringManager::getIDForString("amarilloShipArchetype"));
        entity->SetPosition(glm::vec3(30.0,3.25,25.0));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
 */
        
        /*
         entity = entityFactory->createEntity(StringManager::getIDForString("shipArchetype"));
         entity->SetPosition(glm::vec3(20.0,0.0,-60.0));
         rotation = glm::angleAxis<float>(25.0,0.0,1.0,0.0);
         entity->SetRotation(rotation);
         level.addEntity(entity);
         
         entity = entityFactory->createEntity(StringManager::getIDForString("shipArchetype"));
         entity->SetPosition(glm::vec3(-15.0,0.0,-35.0));
         rotation = glm::angleAxis<float>(95.0,0.0,1.0,0.0);
         entity->SetRotation(rotation);
         level.addEntity(entity);
         */
              //visual->SetPosition(glm::vec3(1.0, 0.0, 0.0));
        //rotation = glm::angleAxis<float>(90.0,0.0,1.0,0.0);
        //visual->SetRotation(rotation);
        
        //testObjectID = visualizer.createVisual(StringManager::getIDForString("BlueArrow"), StringManager::getIDForString("debugArrowBlueArchetype"));
        //visual = visualizer.getVisual(testObjectID);
        //visual->SetPosition(glm::vec3(0.1, 0.0, 0.0));
        
        /*
        testObjectID = visualizer.createVisual(StringManager::getIDForString("GreenArrow"), StringManager::getIDForString("debugArrowGreenArchetype"));
        visual = visualizer.getVisual(testObjectID);
        rotation = glm::angleAxis<float>(45.0,0.0,1.0,0.0);
        visual->SetRotation(rotation);
        visual->SetPosition(glm::vec3(0.0, 0.0, 1.0));
        */
        
    
        //Debug3DMathVisualizer::rotateVisualAlongLine(visual, glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,2.0,0.1));
        //visual->SetPosition(glm::vec3(0.0, 1.0, 0.0));
  
        
        
        //Debug3DMathVisualizer::rotateVisualAlongLine(visual, glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 2.0));
        //visual->SetPosition(glm::vec3(0.0, 0.0, 2.0));

        
        //rotation = glm::angleAxis<float>(45.0,0.0,1.0,0.0);
        //visual->SetRotation(rotation);
        //visual->SetPosition(glm::vec3(0.0, 0.0, 1.0));

        
        //preload
        entity = entityFactory->createEntity(StringManager::getIDForString("dalekGroupArchetype"));
        entity->SetPosition(glm::vec3(0, 100000, 0));
        entity = entityFactory->createEntity(StringManager::getIDForString("enterpriseGroupArchetype"));
        entity->SetPosition(glm::vec3(0, 100000, 0));
        entity = entityFactory->createEntity(StringManager::getIDForString("starDestroyerGroupArchetype"));
        entity->SetPosition(glm::vec3(0, 100000, 0));
        entity = entityFactory->createEntity(StringManager::getIDForString("laserArchetype"));
        entity->SetPosition(glm::vec3(0, 100000, 0));
        entity = entityFactory->createEntity(StringManager::getIDForString("largeOrangePhaserArchetype"));
        entity->SetPosition(glm::vec3(0, 100000, 0));
        entity = entityFactory->createEntity(StringManager::getIDForString("techionCannonArchetype"));
        entity->SetPosition(glm::vec3(0, 100000, 0));
        entity = entityFactory->createEntity(StringManager::getIDForString("greenLaserArchetype"));
        entity->SetPosition(glm::vec3(0, 100000, 0));
        entity = entityFactory->createEntity(StringManager::getIDForString("tardisGroupArchetype"));
        entity->SetPosition(glm::vec3(0, 100000, 0));
        level.setScriptEngine(IScriptedEventEngine::SPACE_BATTLE_SCRIPT);
    }

};


#endif
