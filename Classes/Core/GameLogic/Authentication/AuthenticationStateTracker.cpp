#include <Core/GameLogic/Authentication/AuthenticationStateTracker.h>

AuthenticationTracker::AuthenticationTracker() : m_currentState(AuthenticationState::INVALID) {}

void AuthenticationTracker::SetCurrentState(AuthenticationState state) {
	m_currentState = state;
}

AuthenticationState AuthenticationTracker::GetCurrentState() const {
	return m_currentState;
}