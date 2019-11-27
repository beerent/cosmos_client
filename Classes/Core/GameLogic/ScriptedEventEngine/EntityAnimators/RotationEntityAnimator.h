#pragma once
#include "core/GameLogic/ScriptedEventEngine/EntityAnimators/IEntityAnimator.h"
#include "core/StringManager/StringManager.h"
#include "glm/glm.hpp"
#include "core/LevelFactory/LevelFactory.h"
#include "core/Entity/Entities/Entity.h"

class RotationEntityAnimator : public IEntityAnimator {
public:

	RotationEntityAnimator(Entity* targetEntity, float degreesPerSecond, glm::vec3 rotationAxis) :
		m_rotationDegreesPerSecond(degreesPerSecond), m_targetEntity(targetEntity), m_rotationAxis(rotationAxis),
		m_playing(false) {
	}

	void Play() override {
		m_playing = true;
	}

	void Stop() override {
		m_playing = false;
	}

	void Pump(float delta) override{
		if (m_playing == false) {
			return;
		}

		glm::quat rotation = glm::angleAxis<float>(m_rotationDegreesPerSecond * delta, m_rotationAxis.x, m_rotationAxis.y, m_rotationAxis.z);
		rotation = m_targetEntity->getRotation() * rotation;
		m_targetEntity->SetRotation(rotation);
	}

private:
	Entity* m_targetEntity;
	float m_rotationDegreesPerSecond;
	float m_playing;
	glm::vec3 m_rotationAxis;
};