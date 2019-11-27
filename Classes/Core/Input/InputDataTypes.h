#ifndef INPUT_DATA_TYPES_H
#define INPUT_DATA_TYPES_H

class CharacterMovementInput {
public:
	float m_moveForward;
	float m_turnRight;
	float m_strafeRight;
	float m_jump;

	CharacterMovementInput() : m_jump(0.0), m_moveForward(0.0), m_strafeRight(0.0), m_turnRight(0.0) {}
};

#endif