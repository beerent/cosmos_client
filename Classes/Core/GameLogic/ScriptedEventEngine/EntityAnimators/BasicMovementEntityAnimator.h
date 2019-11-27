#pragma once
#include "core/GameLogic/ScriptedEventEngine/EntityAnimators/IEntityAnimator.h"
#include "core/StringManager/StringManager.h"
#include "glm/glm.hpp"
#include "core/LevelFactory/LevelFactory.h"
#include "core/Entity/Entities/Entity.h"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/euler_angles.hpp"


class BasicMovementEntityAnimator : public IEntityAnimator {
public:

	BasicMovementEntityAnimator(Entity* targetEntity, glm::vec3 moveToPosition, float animationLength) : 
		m_targetEntity(targetEntity), m_moveToPosition(moveToPosition), m_animationLength(animationLength), m_currentPositionTime(0.0), 
		m_playing(false) {
	}

	void Play() override {
		if (IsAtBegining()) {	
			m_startPosition = m_targetEntity->getPosition();
		}
		m_playing = true;
	}

	void Stop() override {
		m_playing = false;
	}

	void Pump(float delta) override{
		if (m_playing == false) {
			return;
		}

		m_currentPositionTime += delta;

		glm::vec3 deltaPosition = m_moveToPosition - m_startPosition;
		float distance = glm::length(deltaPosition);
		float playbackPercentage = m_currentPositionTime / m_animationLength;				

		if (playbackPercentage >= 1.0) {
			playbackPercentage = 1.0;
			Stop();
		}
		
		float distanceAtTimeline = distance * playbackPercentage;
		glm::vec3 direction = glm::normalize(deltaPosition);	

		glm::vec3 targetPositionAtTimeline = m_startPosition;
		targetPositionAtTimeline += direction * distanceAtTimeline;
		m_targetEntity->SetPosition(targetPositionAtTimeline);
	}

private:
	Entity* m_targetEntity;
	glm::vec3 m_moveToPosition;
	glm::vec3 m_startPosition;

	float m_animationLength;
	float m_currentPositionTime;
	float m_playing;

	bool IsAtBegining() const {
		return m_currentPositionTime == 0.0;
	}
};

class EasyInOutMovementEntityAnimator : public IEntityAnimator {
public:

	EasyInOutMovementEntityAnimator(Entity* targetEntity, glm::vec3 moveToPosition, float accelerationRate) :
		m_targetEntity(targetEntity), m_moveToPosition(moveToPosition), m_accelerationRate(accelerationRate), m_currentPositionTime(0.0),
		m_playing(false), m_velocity(0.0f){
	}

	void Play() override {
		if (IsAtBegining()) {
			m_startPosition = m_targetEntity->getPosition();
			m_currentPosition = m_startPosition;
		}
		m_playing = true;
	}

	void Stop() override {
		m_playing = false;
	}

	void Pump(float delta) override {
		if (m_playing == false) {
			return;
		}

		m_currentPositionTime += delta;

		glm::vec3 deltaPosition = m_moveToPosition - m_startPosition;
		float startDistance = glm::length(deltaPosition);
		glm::vec3 currentDeltaPosition = m_currentPosition - m_moveToPosition;

		float currentDistance = glm::length(currentDeltaPosition);

		if (currentDistance > startDistance / 2.0f)
		{
			m_velocity += m_accelerationRate * delta;
		}
		else
		{
			m_velocity -= m_accelerationRate * delta;
		}	

		if (m_velocity == 0 || m_velocity < 0) {
			Stop();
			m_velocity = 0;		
			return;
		}

		glm::vec3 direction = glm::normalize(deltaPosition);
		glm::vec3 moveDistanceAtVelocity = direction * m_velocity * delta;

		float distanceAtCurrentVelocity = glm::length(moveDistanceAtVelocity);

		if (currentDistance < distanceAtCurrentVelocity) {
			Stop();
			m_velocity = 0;
			m_targetEntity->SetPosition(m_moveToPosition);
		}
		else {
			m_currentPosition += direction * m_velocity * delta;
			m_targetEntity->SetPosition(m_currentPosition);
		}
	}

private:
	Entity* m_targetEntity;
	glm::vec3 m_moveToPosition;
	glm::vec3 m_currentPosition;
	glm::vec3 m_startPosition;

	float m_accelerationRate;
	float m_currentPositionTime;
	float m_playing;
	float m_velocity;

	bool IsAtBegining() const {
		return m_currentPositionTime == 0.0;
	}
};

class OrbitEntityAnimator : public IEntityAnimator {
public:

	OrbitEntityAnimator(Entity* targetEntity, float degreesPerSecond, glm::vec3 rotationPoint, glm::vec3 rotationAxis) :
		m_rotationDegreesPerSecond(degreesPerSecond), m_targetEntity(targetEntity), m_rotationAxis(rotationAxis), 
		m_rotationPoint(rotationPoint), m_playing(false) {
	}

	void Play() override {
		m_playing = true;
	}

	void Stop() override {
		m_playing = false;
	}

	void Pump(float delta) override {
		if (m_playing == false) {
			return;
		}


		//calculate current radius
		glm::vec3 currentPossition = m_targetEntity->getPosition();
		const float radius = glm::distance(m_rotationPoint, currentPossition);
		glm::vec3 A = glm::normalize(currentPossition - m_rotationPoint);
		glm::vec3 B = glm::normalize(m_rotationAxis);
		//glm::vec3 crossProduct = glm::cross(A, B);
		//const float dotProduct = glm::dot(A, B);
		
		//const float angle = glm::acos(dotProduct);
		const float orientedAngle = glm::orientedAngle(A, B, glm::vec3(0, 1, 0));
		//const float angleDegrees = glm::degrees(angle);
		const float newAngle = m_rotationDegreesPerSecond * delta + orientedAngle;
		//calculate current rotation
		//calculate rotation delta
		//glm::quat newRotation = glm::angleAxis<float>(newAngle, crossProduct.x, crossProduct.y, crossProduct.z);
		glm::quat newRotation = glm::angleAxis<float>(newAngle, 0.0f, 1.0f, 0.0f);
		//newRotation = glm::normalize(newRotation);
		//calculate new position based on current rotation
		//adjust object position
		glm::vec3 newPosition = glm::normalize(B * newRotation);
		newPosition *= radius;
		newPosition += m_rotationPoint;
		m_targetEntity->SetPosition(newPosition);				
	}

private:
	Entity* m_targetEntity;
	float m_rotationDegreesPerSecond;
	float m_playing;
	glm::vec3 m_rotationAxis;
	glm::vec3 m_rotationPoint;
};