#pragma once
#include "core/GameLogic/ScriptedEventEngine/EntityAnimators/IEntityAnimator.h"
#include "core/GameLogic/ScriptedEventEngine/EntityAnimators/BasicMovementEntityAnimator.h"
#include "core/GameLogic/ScriptedEventEngine/EntityAnimators/RotationEntityAnimator.h"
#include <vector>

class EntityAnimatorFactory {
public:
	~EntityAnimatorFactory() {
		for (auto animator : m_entityAnimators) {
			delete animator;
		}
	}

	BasicMovementEntityAnimator* createBasicMovementEntityAnimator(Entity* targetEntity, glm::vec3 moveToPosition, float animationLength) {
		BasicMovementEntityAnimator* animator = new BasicMovementEntityAnimator(targetEntity, moveToPosition, animationLength);
		m_entityAnimators.push_back(animator);
		return animator;
	}

	BasicMovementEntityAnimator* createBasicMovementEntityAnimator(Entity* targetEntity, STRING_ID moveToEntityName, float animationLength) {
		Entity* entity = LevelFactory::getActiveLevel()->getEntityByName(moveToEntityName);	
		BasicMovementEntityAnimator* animator = new BasicMovementEntityAnimator(targetEntity, entity->getPosition(), animationLength);
		m_entityAnimators.push_back(animator);
		return animator;
	}

	EasyInOutMovementEntityAnimator* createEaseInOutMovementEntityAnimator(Entity* targetEntity, glm::vec3 moveToPosition, float acceleration) {
		EasyInOutMovementEntityAnimator* animator = new EasyInOutMovementEntityAnimator(targetEntity, moveToPosition, acceleration);
		m_entityAnimators.push_back(animator);
		return animator;
	}

	RotationEntityAnimator* createRotationEntityAnimator(Entity* targetEntity, float rotationDegreesPerSecond, glm::vec3 rotationAxis) {
		RotationEntityAnimator* animator = new RotationEntityAnimator(targetEntity, rotationDegreesPerSecond, rotationAxis);
		m_entityAnimators.push_back(animator);
		return animator;
	}

	OrbitEntityAnimator* createOrbitEntityAnimator(Entity* targetEntity, float rotationDegreesPerSecond, glm::vec3 rotationPoint, glm::vec3 rotationAxis) {
		OrbitEntityAnimator* animator = new OrbitEntityAnimator(targetEntity, rotationDegreesPerSecond, rotationPoint, rotationAxis);
		m_entityAnimators.push_back(animator);
		return animator;
	}

private:
	std::vector<IEntityAnimator*> m_entityAnimators;
};