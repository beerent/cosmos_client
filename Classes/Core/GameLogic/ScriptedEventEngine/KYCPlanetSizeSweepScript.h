#pragma once

#include "Core/GameLogic/ScriptedEventEngine/IScriptedEventEngine.h"
#include "Core/Events/EventDispatcher.h"
#include "Core/Events/TimeEventGenerator.h"
#include "Core/StringManager/StringManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "core/GameLogic/ScriptedEventEngine/EntityAnimators/EntityAnimatorFactory.h"
#include <vector>

class KYCPlanetSizeSweepScript : public IScriptedEventEngine, public IEventHandler
{
public:
	KYCPlanetSizeSweepScript();
	~KYCPlanetSizeSweepScript();

	void start();
	void stop();
	void OnHandleEvent(const Event& event);

private:
	CONST_STRING_DEC(INITIAL_CAMERA_POSITION)
		CONST_STRING_DEC(INTERPOLATE_TO_MARS)
		CONST_STRING_DEC(INTERPOLATE_TO_VENUS)
		CONST_STRING_DEC(INTERPOLATE_TO_EARTH)
		CONST_STRING_DEC(INTERPOLATE_TO_NEPTUNE)
		CONST_STRING_DEC(INTERPOLATE_TO_URANUS)
		CONST_STRING_DEC(INTERPOLATE_TO_SATURN)
		CONST_STRING_DEC(INTERPOLATE_TO_JUPITER)
		CONST_STRING_DEC(INTERPOLATE_TO_SUN)
		CONST_STRING_DEC(INTERPOLATE_TO_TRANISTION_POINT)
		CONST_STRING_DEC(INTERPOLATE_TO_MERCURY)
		CONST_STRING_DEC(INTERPOLATE_TO_MARS_SWEEP_BY_SIZE)
		CONST_STRING_DEC(INTERPOLATE_TO_VENUS_SWEEP_BY_SIZE)
		CONST_STRING_DEC(INTERPOLATE_TO_EARTH_SWEEP_BY_SIZE)
		CONST_STRING_DEC(INTERPOLATE_TO_NEPTUNE_SWEEP_BY_SIZE)
		CONST_STRING_DEC(INTERPOLATE_TO_URANUS_SWEEP_BY_SIZE)
		CONST_STRING_DEC(INTERPOLATE_TO_SATURN_SWEEP_BY_SIZE)
		CONST_STRING_DEC(INTERPOLATE_TO_JUPITER_SWEEP_BY_SIZE)
		CONST_STRING_DEC(INTERPOLATE_TO_MERCURY_SWEEP_BY_SIZE)
		CONST_STRING_DEC(INTERPOLATE_TO_SUN_SWEEP_BY_SIZE)
		CONST_STRING_DEC(MOVE_PLANETS_TO_SWAP_POSITION)
		CONST_STRING_DEC(MOVE_PLANETS_TO_SWAP_POSITION_2)
		CONST_STRING_DEC(MOVE_PLANETS_TO_SWAP_POSITION_FINAL)
		CONST_STRING_DEC(MOVE_PLANETS_TO_RELAIVE_DISTANCE_POSITION)
		CONST_STRING_DEC(INTERPOLATE_TO_SUN_ORBIT_POSITIZON)
		CONST_STRING_DEC(INTERPOLATE_TO_RELATIVE_DISTANCE_SWEEP_START)
		CONST_STRING_DEC(INTERPOLATE_TO_RELATIVE_DISTANCE_SWEEP_END)
		CONST_STRING_DEC(MOVE_PLANETS_TO_TRANSITION_BY_RELATIVE_POSITION)

		CONST_STRING_DEC(ORBIT_URANUS)

		CONST_STRING_DEC(RESET_SCRIPT)

		EntityAnimatorFactory m_entityAnimatorFactory;
	std::vector<IEntityAnimator* > m_entityAnimators;

	TimeEventGenerator mTimeEventGen;
	glm::vec3 m_cameraPositionBeforeInterpolation;
	void InterpolateCameraToEntity(STRING_ID targetEntity,
		unsigned int frame);

	void AnimateToPlanet(STRING_ID targetEntity);
	void AnimateToPlanet(STRING_ID targetEntity, float timeAtPlanet);
	void AnimateToPlanet(STRING_ID targetEntity, float timeAtPlanet, const glm::vec3& offset);

	void AddPlanetRotationAnimation(STRING_ID targetEntity, float degreesPerSecond);
	void AddPlanetOrbitAnimation(STRING_ID targetEntity, float degreesPerSecond);
};