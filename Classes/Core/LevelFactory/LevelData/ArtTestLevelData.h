//
//  ArtTestLevelData.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/17/13.
//
//

#ifndef Projectios1_ArtTestLevelData_h
#define Projectios1_ArtTestLevelData_h
#include "Core/LevelFactory/LevelData/LevelData.h"
#include "Core/Entity/Entities/Entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "IEngine.h"
#include "Core/Entity/Entities/Camera.h"
#include "Core/Debug/Debug3DMathVisualizer.h"
#include "Core/Physics/IPhysicsManager.h"
#include "Core/Rendering/LightingManager/LightingManager.h"

class ArtTestLevelData : public LevelData
{
public:
    virtual void initLevel(Level &level)
    {
        IEngine *iEngine = IEngine::getEngine();
        Camera* camera = iEngine->getActiveCamera();
        camera->SetPosition(glm::vec3(-23.0,10.0,27.0));
        glm::quat camRotation = glm::angleAxis<float>(0,0.0,0.0,1.0);
        camRotation[0] = -0.163411;
        camRotation[1] = -0.466;
        camRotation[2] = -0.09953;
        camRotation[3] = 0.8653;
        
        camera->SetRotation(camRotation);
        
        
        LightingManager* lighting = iEngine->getLightingManager();
        IPhysicsManager* physicsManger = iEngine->getPhysicsManager();
        physicsManger->initialize(glm::vec3(0,0,0), false);
        
        lighting->setInfinitLightSourceVector(glm::vec3(-1.0, 1.0, -1.0));
        lighting->setAmbientColor(glm::vec3(0.08, 0.08, 0.45));
        lighting->setDiffuseColor(glm::vec3(0.55, 0.55, 0.45));
        lighting->setSpecularColor(glm::vec3(0.75, 0.7, 0.6));
        

        
        //KKonecko: actualy Implement real level loading here.
        EntityFactory * entityFactory = EntityFactory::getInstance();
        Entity * entity;
        
        entity = entityFactory->createEntity(StringManager::getIDForString("moonArchetype"));
        entity->SetPosition(glm::vec3(0.0,0.0,0.0));
        entity->setScale(6000.0f);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("starSphereArchetype"));
        glm::quat rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        entity->setScale(70.0f);
        entity->SetPosition(glm::vec3(0.0,0.0,0.0));
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("spiredBuildingArchetype"));
        entity->SetPosition(glm::vec3(50.0,14.0,-150.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("spiredBuildingArchetype"));
        entity->SetPosition(glm::vec3(200.0,14.0,-100.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("squatBuildingArchetype"));
        entity->SetPosition(glm::vec3(250.0,10.0,-175.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("spiredBuildingArchetype"));
        entity->SetPosition(glm::vec3(200.0,14.0,-200.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("tallBuildingArchetype"));
        entity->SetPosition(glm::vec3(110.0,51.0,-40.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("lowBuildingArchetype"));
        entity->SetPosition(glm::vec3(150.0,3.5,-70.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("cubeBuildingArchetype"));
        entity->SetPosition(glm::vec3(100.0,14.0,-20.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("treeArchetype"));
        entity->SetPosition(glm::vec3(70.0,1.7,-25.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("catDudeArchetype"));
        entity->SetPosition(glm::vec3(75.0,0.3,-20.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("fountainArchetype"));
        entity->SetPosition(glm::vec3(72.5,0.1,-23.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("landingPadArchetype"));
        entity->SetPosition(glm::vec3(25.0,0.2,-20.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("tarsusArchetype"));
        entity->SetPosition(glm::vec3(25.0,10.0,-20.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("testThrusterArchetype"));
        entity->SetPosition(glm::vec3(5.5,10.0,-20.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("testThrusterFlameArchetype"));
        entity->SetPosition(glm::vec3(5.0,10.0,-20.0));
        rotation = glm::angleAxis<float>(-45.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        
        entity = entityFactory->createEntity(StringManager::getIDForString("testCityArchetype"));
        entity->SetPosition(glm::vec3(-150.0,0.2,-50.0));
        rotation = glm::angleAxis<float>(0.0,0.0,1.0,0.0);
        entity->SetRotation(rotation);
        level.addEntity(entity);
        // In game - Right-Handed Cartesian Coordinates
        // up/down                     y axis +/-  Green
        // right/left                  x axis +/-  Red
        // back/forward                z axis -/+  Blue
        
        // In blender - Right-Handed Z up coordinate system common to most CAD software
        // up/down                     z axis +/-  Blue
        // right/left                  x axis +/-  Red
        // back/forward                y axis +/-  Green
        
        Debug3DMathVisualizer& visualizer = Debug3DMathVisualizer::instance();
        int testObjectID = visualizer.createVisual(StringManager::getIDForString("RedArrow"), StringManager::getIDForString("debugArrowRedArchetype"));
        Debug3DVisual* visual = visualizer.getVisual(testObjectID);
        visual->SetPosition(glm::vec3(1.0, 0.0, 0.0));
        rotation = glm::angleAxis<float>(90.0,0.0,1.0,0.0);
        visual->SetRotation(rotation);
        
        testObjectID = visualizer.createVisual(StringManager::getIDForString("BlueArrow"), StringManager::getIDForString("debugArrowBlueArchetype"));
        visual = visualizer.getVisual(testObjectID);
        visual->SetPosition(glm::vec3(0.0, 0.0, 1.0));
        
        
        testObjectID = visualizer.createVisual(StringManager::getIDForString("GreenArrow"), StringManager::getIDForString("debugArrowGreenArchetype"));
        visual = visualizer.getVisual(testObjectID);
        rotation = glm::angleAxis<float>(-90.0,1.0,0.0,0.0);
        visual->SetRotation(rotation);
        visual->SetPosition(glm::vec3(0.0, 1.0, 0.0));
        
        
        testObjectID = visualizer.createVisual(StringManager::getIDForString("Point"), StringManager::getIDForString("debugPointArchetype"));
        visual = visualizer.getVisual(testObjectID);
        
        visual->SetPosition(glm::vec3(0.0, 0.0, 0.0));
    }
};


#endif
