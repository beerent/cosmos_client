#include "KYCPlanetSizeSweepScript.h"
#include "Core/Events/GameEvents.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/Entity/EntityFactory.h"
#include "Core/GameLogic/EntityControler/EntityControler.h"
#include "IEngine.h"
#include "Core/Entity/Entities/Camera.h"
#include "Core/GUI/Components/UILabel.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/GameState/GameStateMachine.h"
#include <map>

CONST_STRING_DEF(KYCPlanetSizeSweepScript, INITIAL_CAMERA_POSITION)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_MARS)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_VENUS)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_EARTH)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_NEPTUNE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_URANUS)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_SATURN)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_JUPITER)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_SUN)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_TRANISTION_POINT)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_MERCURY)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, MOVE_PLANETS_TO_SWAP_POSITION)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, MOVE_PLANETS_TO_SWAP_POSITION_2)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, MOVE_PLANETS_TO_SWAP_POSITION_FINAL)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, MOVE_PLANETS_TO_RELAIVE_DISTANCE_POSITION)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_MARS_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_VENUS_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_EARTH_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_NEPTUNE_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_URANUS_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_SATURN_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_JUPITER_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_MERCURY_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_SUN_SWEEP_BY_SIZE)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_SUN_ORBIT_POSITIZON)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, MOVE_PLANETS_TO_TRANSITION_BY_RELATIVE_POSITION)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_RELATIVE_DISTANCE_SWEEP_START)
CONST_STRING_DEF(KYCPlanetSizeSweepScript, INTERPOLATE_TO_RELATIVE_DISTANCE_SWEEP_END)


CONST_STRING_DEF(KYCPlanetSizeSweepScript, ORBIT_URANUS)

CONST_STRING_DEF(KYCPlanetSizeSweepScript, RESET_SCRIPT)


const float INTERPOLATION_FRAMES = 500.0f;

namespace
{
	Entity* GetEntity(STRING_ID name)
	{
		return LevelFactory::getInstance()->getActiveLevel()->getEntityByName(name);
	}

	EntityControler* GetEntityControler(STRING_ID name)
	{
		return GetEntity(name)->getControler();
	}

	float calculateOffset(float previousPlanetSize, float planetSize, float planetOffsetAdjustment) {
		
		float planetSizeOffset = (previousPlanetSize * 0.5f + planetSize * 0.5f);
		float gap = 1.5f;

		if (planetSizeOffset > 70.0f) {
			gap = 12.0f;
		}
		else if (planetSizeOffset > 2.5f) {
			gap = 4.0f;
		}
			
		return planetSizeOffset + gap * planetOffsetAdjustment;
	}

	std::map <STRING_ID, glm::vec3> calculatePlanetPositionBySize() {
		std::map <STRING_ID, glm::vec3> positionsByName;
		
		float planetOffsetAdjustment = 1.2f;
		float scale = 4.00f;
		float planetSize = 2 * scale;
		float previousPlanetSize = 0;
		float planetOffset = previousPlanetSize * 0.5f + planetSize * 0.5f * planetOffsetAdjustment;
		float totalPlanetOffset = planetOffset;

		scale = 135.67f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset = planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("SUN")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 11.20f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("JUPITER")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 9.45f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("SATURN")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 4.00f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;
		positionsByName[STRING_TO_ID("URANUS")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 3.88f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;
		positionsByName[STRING_TO_ID("NEPTUNE")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 1.0f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("EARTH")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 0.95f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("VENUS")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 0.53f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("MARS")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 0.38f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("MERCURY")] = glm::vec3(0.0, 0.0, totalPlanetOffset);
		return positionsByName;
	}

	std::map <STRING_ID, glm::vec3> calculatePlanetPositionByLocationFromSun() {
		std::map <STRING_ID, glm::vec3> positionsByName;

		float planetOffsetAdjustment = 1.2f;
		float scale = 4.00f;
		float planetSize = 2 * scale;
		float previousPlanetSize = 0;
		float planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		float totalPlanetOffset = planetOffset;

		scale = 135.67f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset = planetOffset;
		previousPlanetSize = planetSize;
		positionsByName[STRING_TO_ID("SUN")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 0.38f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;
		positionsByName[STRING_TO_ID("MERCURY")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 0.95f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;
		positionsByName[STRING_TO_ID("VENUS")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 1.0f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;
		positionsByName[STRING_TO_ID("EARTH")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 0.53f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;
		positionsByName[STRING_TO_ID("MARS")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 11.20f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("JUPITER")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 9.45f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("SATURN")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 4.00f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("URANUS")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		scale = 3.88f;
		planetSize = 2 * scale;
		planetOffset = calculateOffset(previousPlanetSize, planetSize, planetOffsetAdjustment);
		totalPlanetOffset += planetOffset;
		previousPlanetSize = planetSize;

		positionsByName[STRING_TO_ID("NEPTUNE")] = glm::vec3(0.0, 0.0, totalPlanetOffset);

		return positionsByName;
	}

	std::map <STRING_ID, glm::vec3> calculatePlanetPositionBySizeTransition() {
		std::map <STRING_ID, glm::vec3> positionsByName = calculatePlanetPositionBySize();

		float zMin = 100000;
		float zMax = -100000;

		for (auto& it : positionsByName) {
			glm::vec3 position = it.second;

			if (zMin > position.z) {
				zMin = position.z;
			}

			if (zMax < position.z) {
				zMax = position.z;
			}
		}

		float range = zMax - zMin;
		float midZValue = range / 6; // junk Math! 
		float offsetToSun = zMax - midZValue;

		for (auto& it : positionsByName) {
			STRING_ID name = it.first;
			glm::vec3 position = it.second;
			if (name != STRING_TO_ID("SUN")) {
				position = glm::vec3(position.x, position.z - offsetToSun, position.z);
				it.second = position;
			}
		}
		return positionsByName;
	}
	
	std::map <STRING_ID, glm::vec3> calculatePlanetPositionByPositionFromSunTransition() {
		std::map <STRING_ID, glm::vec3> positionsByName = calculatePlanetPositionBySize();
		std::map <STRING_ID, glm::vec3> positionsSortedByDistanceIndexedByName = calculatePlanetPositionByLocationFromSun();

		float zMin = 100000;
		float zMax = -100000;

		for (auto& it : positionsByName) {
			glm::vec3 position = it.second;

			if (zMin > position.z) {
				zMin = position.z;
			}

			if (zMax < position.z) {
				zMax = position.z;
			}
		}

		float range = zMax - zMin;
		float midZValue = range / 6; // junk Math! 
		float offsetToSun = zMax - midZValue;

		for (auto& it : positionsByName) {
			STRING_ID name = it.first;
			glm::vec3 position = it.second;
			glm::vec3 positionByDistance = positionsSortedByDistanceIndexedByName[name];
			if (name != STRING_TO_ID("SUN")) {
				position = glm::vec3(position.x, position.z - offsetToSun, positionByDistance.z);
				it.second = position;
			}
		}
		return positionsByName;		
	}
}

std::map <STRING_ID, glm::vec3> calculatePlanetPositionByLocationFromSunAndRelativeDistance() {
	std::map <STRING_ID, glm::vec3> positionsByName;

	float earthDistance = 15;
	float distanceFromSunOffset = 135.67f; // sun diameter

	positionsByName[STRING_TO_ID("SUN")] = glm::vec3(0.0, 0.0, earthDistance * 0.0);
	positionsByName[STRING_TO_ID("MERCURY")] = glm::vec3(0.0, 0.0, distanceFromSunOffset + earthDistance * 0.3871);
	positionsByName[STRING_TO_ID("VENUS")] = glm::vec3(0.0, 0.0, distanceFromSunOffset + earthDistance * 0.7226);
	positionsByName[STRING_TO_ID("EARTH")] = glm::vec3(0.0, 0.0, distanceFromSunOffset + earthDistance * 1.0);
	positionsByName[STRING_TO_ID("MARS")] = glm::vec3(0.0, 0.0, distanceFromSunOffset + earthDistance * 1.5226);
	positionsByName[STRING_TO_ID("JUPITER")] = glm::vec3(0.0, 0.0, distanceFromSunOffset + earthDistance * 5.2);
	positionsByName[STRING_TO_ID("SATURN")] = glm::vec3(0.0, 0.0, distanceFromSunOffset + earthDistance * 9.5344);
	positionsByName[STRING_TO_ID("URANUS")] = glm::vec3(0.0, 0.0, distanceFromSunOffset + earthDistance * 19.5344);
	positionsByName[STRING_TO_ID("NEPTUNE")] = glm::vec3(0.0, 0.0, distanceFromSunOffset + earthDistance * 30.047);

	return positionsByName;
}


KYCPlanetSizeSweepScript::KYCPlanetSizeSweepScript()
{
	mTimeEventGen.registerForEventType(GameEvents::TIME_EVENT, this);
	IEngine::getEngine()->registerForDeltaTimeEvents(this);
}

KYCPlanetSizeSweepScript::~KYCPlanetSizeSweepScript()
{
	mTimeEventGen.reset();
	mTimeEventGen.unRegisterForEventType(GameEvents::TIME_EVENT, this);
	IEngine::getEngine()->unregisterForDeltaTimeEvents(this);
}

void KYCPlanetSizeSweepScript::start()
{
	mTimeEventGen.reset();

	float offset = 0;
	float timeAtPlanet = 6.0f;

	mTimeEventGen.setupTimeEvent(INITIAL_CAMERA_POSITION, offset + 0, 0, 0);
	offset += timeAtPlanet;

	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_MARS, offset + 0.1, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_VENUS, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_EARTH, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_NEPTUNE, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_URANUS, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_SATURN, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_JUPITER, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_SUN, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_TRANISTION_POINT, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;

	mTimeEventGen.setupTimeEvent(MOVE_PLANETS_TO_SWAP_POSITION, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(MOVE_PLANETS_TO_SWAP_POSITION_2, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(MOVE_PLANETS_TO_SWAP_POSITION_FINAL, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_NEPTUNE_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_URANUS_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_SATURN_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_JUPITER_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_MARS_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_EARTH_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_VENUS_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_MERCURY_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_SUN_SWEEP_BY_SIZE, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(MOVE_PLANETS_TO_TRANSITION_BY_RELATIVE_POSITION, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(MOVE_PLANETS_TO_RELAIVE_DISTANCE_POSITION, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_RELATIVE_DISTANCE_SWEEP_START, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_RELATIVE_DISTANCE_SWEEP_END, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(ORBIT_URANUS, offset + 0, 0, 0);
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_SUN, offset, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(INTERPOLATE_TO_SUN_ORBIT_POSITIZON, offset + 0, 0, 0);
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	offset += timeAtPlanet;
	offset += timeAtPlanet;
    offset += timeAtPlanet;
    offset += timeAtPlanet;
    offset += timeAtPlanet;
    offset += timeAtPlanet;
    offset += timeAtPlanet;
    offset += timeAtPlanet;
    offset += timeAtPlanet;
    offset += timeAtPlanet;
	mTimeEventGen.setupTimeEvent(RESET_SCRIPT, offset, 0, 0);
	return;
}

void KYCPlanetSizeSweepScript::stop()
{
}

void KYCPlanetSizeSweepScript::InterpolateCameraToEntity(STRING_ID targetEntity,
	unsigned int frame) {
	float percent = frame / INTERPOLATION_FRAMES;
	IEngine* engine = IEngine::getEngine();
	Camera* camera = engine->getActiveCamera();

	if (frame == 0) {
		m_cameraPositionBeforeInterpolation = camera->getPosition();
	}

	glm::vec3 camPos = GetEntity(targetEntity)->getPosition();
	camPos.x = GetEntity(targetEntity)->getScaleVec()[0] * -5.0f;
	glm::vec3 delta = camPos - m_cameraPositionBeforeInterpolation;
	delta *= percent;
	camera->SetPosition(m_cameraPositionBeforeInterpolation + delta);
}

namespace {
	glm::vec3 GetCameraPositionRelativeToPlanet(Entity* entity) {
		glm::vec3 camPos = entity->getPosition();
		camPos.x = entity->getScaleVec()[0] * -5.0f;
		return camPos;
	}
}

void KYCPlanetSizeSweepScript::OnHandleEvent(const Event& event)
{
	IEngine* engine = IEngine::getEngine();
	Camera* camera = engine->getActiveCamera();

	switch (event.GetEventType()) {

	case GameEvents::GAME_DELTA_TIME_EVENT:
	{
		const GameEvents::GameDeltaTimeEvent& timeEvent = static_cast<const GameEvents::GameDeltaTimeEvent&>(event);
		for (auto animator : m_entityAnimators) {
			animator->Pump(timeEvent.getDeltaTime());
		}
	}
	case GameEvents::TIME_EVENT:
	{
		const GameEvents::TimeEvent& timeEvent = static_cast<const GameEvents::TimeEvent&>(event);
		STRING_ID eventName = timeEvent.getEventName();

		//sprintf(Logger::GetTempCharBuffer(), "%s Starting event %s", __func__, ID_TO_CSTR(eventName));
		//Logger::Log(Logger::SCRIPTING, Logger::GetTempCharBuffer());

		if (eventName == INITIAL_CAMERA_POSITION)
		{
			std::map<STRING_ID, glm::vec3> planetPositions = calculatePlanetPositionBySize();

			for (auto it : planetPositions) {
				STRING_ID name = it.first;
				glm::vec3 position = it.second;
				LevelFactory::getActiveLevel()->getEntityByName(name)->SetPosition(position);
			}

			//engine->getActiveCamera()->lookAtAnimated(glm::vec3(0.0,1.0,0.0), 0.1, false);
			camera->stopFollow();
			camera->stopTracking();
			glm::vec3 camPos = GetEntity(STRING_TO_ID("MERCURY"))->getPosition();
			camPos.x = GetEntity(STRING_TO_ID("MERCURY"))->getScaleVec()[0] * -5.0f;
			camera->SetPosition(camPos);
			camera->lookAt(GetEntity(STRING_TO_ID("MERCURY"))->getPosition());
			//camera->trackEntity(STRING_TO_ID("NODE_1"));

			Entity* entity = LevelFactory::getActiveLevel()->getEntityByName(STRING_TO_ID("MERCURY"));
			IEntityAnimator* animator = m_entityAnimatorFactory.createRotationEntityAnimator(entity, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			animator->Play();
			m_entityAnimators.push_back(animator);

			float planetRotationRateInDegrees = 2.0f;
			AddPlanetRotationAnimation(STRING_TO_ID("MERCURY"), planetRotationRateInDegrees);
			AddPlanetRotationAnimation(STRING_TO_ID("MARS"), planetRotationRateInDegrees);
			AddPlanetRotationAnimation(STRING_TO_ID("VENUS"), planetRotationRateInDegrees);
			AddPlanetRotationAnimation(STRING_TO_ID("EARTH"), planetRotationRateInDegrees);
			AddPlanetRotationAnimation(STRING_TO_ID("NEPTUNE"), planetRotationRateInDegrees);
			AddPlanetRotationAnimation(STRING_TO_ID("URANUS"), planetRotationRateInDegrees);
			AddPlanetRotationAnimation(STRING_TO_ID("SATURN"), planetRotationRateInDegrees);
			AddPlanetRotationAnimation(STRING_TO_ID("JUPITER"), planetRotationRateInDegrees);
			AddPlanetRotationAnimation(STRING_TO_ID("SUN"), planetRotationRateInDegrees);
		}
		else if (eventName == INTERPOLATE_TO_MARS)
		{
			AnimateToPlanet(STRING_TO_ID("MARS"), 0.7f);
		}
		else if (eventName == INTERPOLATE_TO_VENUS)
		{
			AnimateToPlanet(STRING_TO_ID("VENUS"), 0.9f);
		}
		else if (eventName == INTERPOLATE_TO_EARTH)
		{
			AnimateToPlanet(STRING_TO_ID("EARTH"), 1.2f);
		}
		else if (eventName == INTERPOLATE_TO_NEPTUNE)
		{
			AnimateToPlanet(STRING_TO_ID("NEPTUNE"), 5.0f);
		}
		else if (eventName == INTERPOLATE_TO_URANUS)
		{
			AnimateToPlanet(STRING_TO_ID("URANUS"), 6.0f);
		}
		else if (eventName == INTERPOLATE_TO_SATURN)
		{
			AnimateToPlanet(STRING_TO_ID("SATURN"), 10.0f);
		}
		else if (eventName == INTERPOLATE_TO_JUPITER)
		{
			AnimateToPlanet(STRING_TO_ID("JUPITER"), 12.0f);
		}
		else if (eventName == INTERPOLATE_TO_SUN)
		{
			AnimateToPlanet(STRING_TO_ID("SUN"), 35.0f);
		}
		else if (eventName == INTERPOLATE_TO_MERCURY)
		{
			AnimateToPlanet(STRING_TO_ID("MERCURY"), 100.0f);
		}
		else if (eventName == INTERPOLATE_TO_TRANISTION_POINT)
		{
			Entity* planet = LevelFactory::getActiveLevel()->getEntityByName(STRING_TO_ID("JUPITER"));
			glm::vec3 camPos = planet->getPosition();
			camPos.x = planet->getScaleVec()[0] * -11.0f;
			IEntityAnimator* animator = m_entityAnimatorFactory.createEaseInOutMovementEntityAnimator(IEngine::getEngine()->getActiveCamera(), camPos, 25.0f);
			animator->Play();
			m_entityAnimators.push_back(animator);
		}
		else if (eventName == MOVE_PLANETS_TO_SWAP_POSITION)
		{
			std::map<STRING_ID, glm::vec3> planetPositions = calculatePlanetPositionBySizeTransition();

			for (auto it : planetPositions) {
				STRING_ID name = it.first;
				glm::vec3 position = it.second;
				Entity* planet = LevelFactory::getActiveLevel()->getEntityByName(name);			
				IEntityAnimator* animator = m_entityAnimatorFactory.createEaseInOutMovementEntityAnimator(planet, position, 8.0f);
				animator->Play();
				m_entityAnimators.push_back(animator);
			}
		}
		else if (eventName == MOVE_PLANETS_TO_SWAP_POSITION_2)
		{
			std::map<STRING_ID, glm::vec3> planetPositions = calculatePlanetPositionByPositionFromSunTransition();

			for (auto it : planetPositions) {
				STRING_ID name = it.first;
				glm::vec3 position = it.second;
				Entity* planet = LevelFactory::getActiveLevel()->getEntityByName(name);
				IEntityAnimator* animator = m_entityAnimatorFactory.createEaseInOutMovementEntityAnimator(planet, position, 8.0f);
				animator->Play();
				m_entityAnimators.push_back(animator);
			}
		}
		else if (eventName == MOVE_PLANETS_TO_SWAP_POSITION_FINAL)
		{
			std::map<STRING_ID, glm::vec3> planetPositions = calculatePlanetPositionByLocationFromSun();

			for (auto it : planetPositions) {
				STRING_ID name = it.first;
				glm::vec3 position = it.second;
				Entity* planet = LevelFactory::getActiveLevel()->getEntityByName(name);
				IEntityAnimator* animator = m_entityAnimatorFactory.createEaseInOutMovementEntityAnimator(planet, position, 8.0f);
				animator->Play();
				m_entityAnimators.push_back(animator);
			}	

		}
		else if (eventName == INTERPOLATE_TO_MARS_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("MARS"), 15.0f);
		}
		else if (eventName == INTERPOLATE_TO_VENUS_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("VENUS"), 2.0f);
		}
		else if (eventName == INTERPOLATE_TO_EARTH_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("EARTH"), 2.0f);
		}
		else if (eventName == INTERPOLATE_TO_NEPTUNE_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("NEPTUNE"), 20.0f);
		}
		else if (eventName == INTERPOLATE_TO_URANUS_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("URANUS"), 6.0f);
		}
		else if (eventName == INTERPOLATE_TO_SATURN_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("SATURN"), 10.0f);
		}
		else if (eventName == INTERPOLATE_TO_JUPITER_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("JUPITER"), 12.0f);
		}
		else if (eventName == INTERPOLATE_TO_MERCURY_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("MERCURY"), 2.0f);
		}
		else if (eventName == INTERPOLATE_TO_SUN_SWEEP_BY_SIZE)
		{
			AnimateToPlanet(STRING_TO_ID("SUN"), 35.0f);
		}
		else if (eventName == MOVE_PLANETS_TO_RELAIVE_DISTANCE_POSITION)
		{
			std::map<STRING_ID, glm::vec3> planetPositions = calculatePlanetPositionByLocationFromSunAndRelativeDistance();

			for (auto it : planetPositions) {
				STRING_ID name = it.first;
				glm::vec3 position = it.second;
				Entity* planet = LevelFactory::getActiveLevel()->getEntityByName(name);
				IEntityAnimator* animator = m_entityAnimatorFactory.createEaseInOutMovementEntityAnimator(planet, position, 8.0f);
				animator->Play();
				m_entityAnimators.push_back(animator);
			}

		}
		else if (eventName == INTERPOLATE_TO_RELATIVE_DISTANCE_SWEEP_START)
		{
			AnimateToPlanet(STRING_TO_ID("MERCURY"), 20.0f, glm::vec3(0.0, 0.0, 3.0));
			camera->stopFollow();
			camera->stopTracking();
			camera->trackEntity(STRING_TO_ID("SUN"), glm::vec3(0.0, 1.0, 0.0), 0.7f);
		}
		else if (eventName == INTERPOLATE_TO_RELATIVE_DISTANCE_SWEEP_END)
		{
			AnimateToPlanet(STRING_TO_ID("NEPTUNE"), 2.0f, glm::vec3(-100.0, 0.0, 200.0));
		}
		else if (eventName == MOVE_PLANETS_TO_TRANSITION_BY_RELATIVE_POSITION)
		{
			AnimateToPlanet(STRING_TO_ID("SATURN"), 35.0f, glm::vec3(-300.0, 0.0, 0.0));
		}
		else if (eventName == ORBIT_URANUS)
		{
			AddPlanetOrbitAnimation(STRING_TO_ID("MERCURY"), 9.0f);
			AddPlanetOrbitAnimation(STRING_TO_ID("MARS"), 14.0f);
			AddPlanetOrbitAnimation(STRING_TO_ID("VENUS"), 13.0f);
			AddPlanetOrbitAnimation(STRING_TO_ID("EARTH"), 21.0f);
			AddPlanetOrbitAnimation(STRING_TO_ID("NEPTUNE"), 17.0f);
			AddPlanetOrbitAnimation(STRING_TO_ID("URANUS"), 22.0f);
			AddPlanetOrbitAnimation(STRING_TO_ID("SATURN"), 25.0f);
			AddPlanetOrbitAnimation(STRING_TO_ID("JUPITER"), 10.0f);
		}
		else if (eventName == INTERPOLATE_TO_SUN_ORBIT_POSITIZON) {
			Entity* entity = LevelFactory::getActiveLevel()->getEntityByName(STRING_TO_ID("SUN"));
			IEntityAnimator* animator = m_entityAnimatorFactory.createEaseInOutMovementEntityAnimator(IEngine::getEngine()->getActiveCamera(), GetCameraPositionRelativeToPlanet(entity) + glm::vec3(0.0, 150.f, 0.0f) , 20.0f);
			animator->Play();
			m_entityAnimators.push_back(animator);
			IEngine::getEngine()->getActiveCamera()->trackEntity(STRING_TO_ID("SUN"), glm::vec3(0.0, 1.0, 0.0), 10.0f);
		}
		else if (eventName == RESET_SCRIPT)
		{
			start();
		}
	}
	break;

	default:
		Logger::Error("__FUNCITON__ Unexpected Evnet Type");
		break;
	}
}

void KYCPlanetSizeSweepScript::AnimateToPlanet(STRING_ID targetEntity, float timeAtPlanet, const glm::vec3& offset) {
	Entity* entity = LevelFactory::getActiveLevel()->getEntityByName(targetEntity);
	IEntityAnimator* animator = m_entityAnimatorFactory.createEaseInOutMovementEntityAnimator(IEngine::getEngine()->getActiveCamera(), GetCameraPositionRelativeToPlanet(entity) + offset, timeAtPlanet);
	animator->Play();
	m_entityAnimators.push_back(animator);
}

void KYCPlanetSizeSweepScript::AnimateToPlanet(STRING_ID targetEntity, float timeAtPlanet) {
	AnimateToPlanet(targetEntity, timeAtPlanet, glm::vec3(0.0, 0.0, 0.0));
}

void KYCPlanetSizeSweepScript::AddPlanetRotationAnimation(STRING_ID targetEntity, float degreesPerSecond) {
	Entity* entity = LevelFactory::getActiveLevel()->getEntityByName(targetEntity);
	IEntityAnimator* animator = m_entityAnimatorFactory.createRotationEntityAnimator(entity, degreesPerSecond, glm::vec3(0.0f, 1.0f, 0.0f));
	animator->Play();
	m_entityAnimators.push_back(animator);
}

void KYCPlanetSizeSweepScript::AddPlanetOrbitAnimation(STRING_ID targetEntity, float degreesPerSecond) {
	
	Entity* sunEntity = LevelFactory::getActiveLevel()->getEntityByName(STRING_TO_ID("SUN"));
	Entity* entity = LevelFactory::getActiveLevel()->getEntityByName(targetEntity);
	IEntityAnimator* animator = m_entityAnimatorFactory.createOrbitEntityAnimator(entity, degreesPerSecond, sunEntity->getPosition(), glm::vec3(1.0f, 0.0f, 0.0f));
	animator->Play();
	m_entityAnimators.push_back(animator);
}

void KYCPlanetSizeSweepScript::AnimateToPlanet(STRING_ID targetEntity) {
	AnimateToPlanet(targetEntity, 3.0f);
}
