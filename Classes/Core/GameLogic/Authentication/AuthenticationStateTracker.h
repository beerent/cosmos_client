#pragma once

enum class AuthenticationState {
	INVALID,
	INPUT,
	AUTHENTICATING,
	FAILURE,
	SUCCESS
};

class AuthenticationTracker {

public:
	AuthenticationTracker();
	void SetCurrentState(AuthenticationState state);
	AuthenticationState GetCurrentState() const;

private:
	AuthenticationState m_currentState;

};