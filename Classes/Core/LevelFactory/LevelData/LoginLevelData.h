#pragma once

#include "Core/LevelFactory/LevelData/LevelData.h"
#include "Core/Entity/Entities/Entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "IEngine.h"
#include "Core/Entity/Entities/Camera.h"
#include "Core/Debug/Debug3DMathVisualizer.h"
#include "Core/Entity/Entities/Camera.h"
#include "Core/Rendering/LightingManager/LightingManager.h"
#include "Core/Physics/IPhysicsManager.h"
#include "Core/Entity/EntityFactory.h"

class LoginLevelData : public LevelData {

public:
	virtual void initLevel(Level &level) {
		IEngine *iEngine = IEngine::getEngine();
		Camera* camera = iEngine->getActiveCamera();
		camera->SetPosition(glm::vec3(-23.0, 10.0, 27.0));
		glm::quat camRotation = glm::angleAxis<float>(0, 0.0, 0.0, 1.0);
		camRotation[0] = -0.163411;
		camRotation[1] = -0.466;
		camRotation[2] = -0.09953;
		camRotation[3] = 0.8653;

		camera->SetRotation(camRotation);


		LightingManager* lighting = iEngine->getLightingManager();
		IPhysicsManager* physicsManger = iEngine->getPhysicsManager();
		physicsManger->initialize(glm::vec3(0, 0, 0), false);

		lighting->setInfinitLightSourceVector(glm::vec3(-1.0, 1.0, -1.0));
		lighting->setAmbientColor(glm::vec3(0.05, 0.00, 0.00));
		lighting->setDiffuseColor(glm::vec3(0.70, 0.80, 0.70));
		lighting->setSpecularColor(glm::vec3(0.60, 0.60, 0.6));



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

		entity = entityFactory->createEntity(StringManager::getIDForString("XCRXtarsusArchetype"));
		entity->SetPosition(glm::vec3(10.0, 0.0, -20.0));
		rotation = glm::angleAxis<float>(25.0, 0.0, 1.0, 0.0);
		entity->SetRotation(rotation);
		level.addEntity(entity);

		//entity = entityFactory->createEntity(StringManager::getIDForString("tardisGroupArchetype"));
		entity = entityFactory->createEntity(StringManager::getIDForString("lightingTestArchetype"));
		entity->SetPosition(glm::vec3(10.0, 0.0, -15.0));
		rotation = glm::angleAxis<float>(25.0, 0.0, 1.0, 0.0);
		entity->SetRotation(rotation);
		level.addEntity(entity);

		entity = entityFactory->createEntity(StringManager::getIDForString("catDudeArchetype"));
		entity->SetPosition(glm::vec3(0.0, 0.0, 0.0));
		rotation = glm::angleAxis<float>(0.0, 0.0, 1.0, 0.0);
		entity->SetRotation(rotation);
		level.addEntity(entity);

		entity = entityFactory->createEntity(StringManager::getIDForString("barrelArcheType"));
		entity->SetPosition(glm::vec3(0.0, 0.0, -1.0));
		rotation = glm::angleAxis<float>(0.0, 0.0, 1.0, 0.0);
		entity->SetRotation(rotation);
		level.addEntity(entity);


		/*

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
		Debug3DMathVisualizer& visualizer = Debug3DMathVisualizer::instance();
		int testObjectID = visualizer.createVisual(StringManager::getIDForString("RedArrow"), StringManager::getIDForString("debugArrowRedArchetype"));
		Debug3DVisual* visual = visualizer.getVisual(testObjectID);
		visual->SetPosition(glm::vec3(1.0, 0.0, 0.0));
		rotation = glm::angleAxis<float>(90.0, 0.0, 1.0, 0.0);
		visual->SetRotation(rotation);

		testObjectID = visualizer.createVisual(StringManager::getIDForString("BlueArrow"), StringManager::getIDForString("debugArrowBlueArchetype"));
		visual = visualizer.getVisual(testObjectID);
		visual->SetPosition(glm::vec3(0.0, 0.0, 1.0));


		testObjectID = visualizer.createVisual(StringManager::getIDForString("GreenArrow"), StringManager::getIDForString("debugArrowGreenArchetype"));
		visual = visualizer.getVisual(testObjectID);
		rotation = glm::angleAxis<float>(-90.0, 1.0, 0.0, 0.0);
		visual->SetRotation(rotation);
		visual->SetPosition(glm::vec3(0.0, 1.0, 0.0));


		testObjectID = visualizer.createVisual(StringManager::getIDForString("Point"), StringManager::getIDForString("debugPointArchetype"));
		visual = visualizer.getVisual(testObjectID);

		visual->SetPosition(glm::vec3(0.0, 0.0, 0.0));
	}
};