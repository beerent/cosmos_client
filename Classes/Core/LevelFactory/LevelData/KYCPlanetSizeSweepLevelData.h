#pragma once

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
#include "Core/Debug/Debug3DMathVisualizer.h"

class KYCPlanetSizeSweepLevelData : public LevelData
{
public:
	virtual void initLevel(Level &level)
	{
		IEngine *iEngine = IEngine::getEngine();
		IPhysicsManager* physicsManger = iEngine->getPhysicsManager();
		physicsManger->initialize(glm::vec3(0, 0, 0), false);

		LightingManager* lighting = iEngine->getLightingManager();
		lighting->setInfinitLightSourceVector(glm::vec3(-0.5, 0.0, -1.0));
		lighting->setAmbientColor(glm::vec3(0.05, 0.00, 0.00));
		lighting->setDiffuseColor(glm::vec3(0.70, 0.80, 0.70));
		lighting->setSpecularColor(glm::vec3(0.60, 0.60, 0.6));

		Camera* camera = iEngine->getActiveCamera();
		camera->SetPosition(glm::vec3(-23.0, 10.0, 27.0));

		glm::quat camRotation = glm::angleAxis<float>(10.0, 1.0, 0.0, 0.0);

		camera->SetRotation(camRotation);
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
		float scale = 4.00f;

		scale = 135.67f;		
		entity = entityFactory->createEntity(StringManager::getIDForString("kycSunArchetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);
		level.addEntity(entity, STRING_TO_ID("SUN"));

		scale = 11.20f;	
		entity = entityFactory->createEntity(StringManager::getIDForString("kycJupiterArchetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);
		level.addEntity(entity, STRING_TO_ID("JUPITER"));

		scale = 9.45f;
		entity = entityFactory->createEntity(StringManager::getIDForString("kycPlanetTest2Archetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);		
		level.addEntity(entity, STRING_TO_ID("SATURN"));

		scale = 4.00f;
		entity = entityFactory->createEntity(StringManager::getIDForString("kycUranusArchetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);	
		level.addEntity(entity, STRING_TO_ID("URANUS"));

		scale = 3.88f;
		entity = entityFactory->createEntity(StringManager::getIDForString("kycNeptuneArchetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);
		level.addEntity(entity, STRING_TO_ID("NEPTUNE"));
		
		scale = 1.0f;	
		entity = entityFactory->createEntity(StringManager::getIDForString("kycEarthArchetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);
		level.addEntity(entity, STRING_TO_ID("EARTH"));

		scale = 0.95f;
		entity = entityFactory->createEntity(StringManager::getIDForString("kycVenusArchetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);
		level.addEntity(entity, STRING_TO_ID("VENUS"));

		scale = 0.53f;
		entity = entityFactory->createEntity(StringManager::getIDForString("kycMarsArchetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);
		level.addEntity(entity, STRING_TO_ID("MARS"));

		scale = 0.38f;	
		entity = entityFactory->createEntity(StringManager::getIDForString("kycMercuryArchetype"));
		rotation = glm::angleAxis<float>(0.0, 1.0, 0.0, 0.0);
		entity->SetRotation(rotation);
		entity->setScale(scale);
		level.addEntity(entity, STRING_TO_ID("MERCURY"));

		level.setScriptEngine(IScriptedEventEngine::KYC_PLANET_SIZE_SWEEP_SCRIPT);
	}
};
