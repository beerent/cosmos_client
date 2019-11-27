//
//  MainMenuLevelData.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/16/13.
//
//

#ifndef Projectios1_MainMenuLevelData_h
#define Projectios1_MainMenuLevelData_h

#include "Core/LevelFactory/LevelData/LevelData.h"
#include "Core/Entity/Entities/Entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "IEngine.h"
#include "Core/Entity/Entities/Camera.h"
#include "Core/Physics/IPhysicsManager.h"
#include "Core/Entity/EntityFactory.h"

class MainMenuLevelData : public LevelData
{
public:
    virtual void initLevel(Level &level)
    {
        //KKonecko: actualy Implement real level loading here.
        
        IEngine *iEngine = IEngine::getEngine();
        IPhysicsManager* physicsManger = iEngine->getPhysicsManager();
        physicsManger->initialize(glm::vec3(0,0,0), false);

		LightingManager* lighting = iEngine->getLightingManager();
		lighting->setInfinitLightSourceVector(glm::vec3(-1.0, 1.0, -1.0));
		lighting->setAmbientColor(glm::vec3(0.05, 0.00, 0.00));
		lighting->setDiffuseColor(glm::vec3(0.70, 0.80, 0.70));
		lighting->setSpecularColor(glm::vec3(0.60, 0.60, 0.6));

        Camera* camera = iEngine->getActiveCamera();
        camera->SetPosition(glm::vec3(-23.0,10.0,27.0));
       
        glm::quat camRotation = glm::angleAxis<float>(10.0,1.0,0.0,0.0);
        
        camera->SetRotation(camRotation);
       
        //KKonecko: actualy Implement real level loading here.
        EntityFactory * entityFactory = EntityFactory::getInstance();
        Entity * entity;

		entity = entityFactory->createEntity(StringManager::getIDForString("spaceSphereArchetype"));
		glm::quat rotation = glm::angleAxis<float>(90.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(1.0f);
		entity->SetPosition(glm::vec3(0.0, 0.0, 0.0));
		level.addEntity(entity);

		entity = entityFactory->createEntity(StringManager::getIDForString("spaceSphereArchetype"));
		rotation = glm::angleAxis<float>(-90.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(1.0f);
		entity->SetPosition(glm::vec3(0.0, 0.0, 0.0));
		level.addEntity(entity);     
  

    }
};

#endif
