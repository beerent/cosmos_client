#include <Core/GameLogic/Authentication/Authenticator.h>
#include <Core/Net/RequestBuilder.h>
#include "IEngine.h"

Authenticator::Authenticator() : m_user(User("", "")) {}

Authenticator::~Authenticator() {
	m_restConnector->CloseRequest(m_authenticationRequestId);
}

void Authenticator::SetRestConnector(IRestConnector* connector) {
	m_restConnector = connector;
}

void Authenticator::SetUser(const User& user) {
	m_user = user;
}

User Authenticator::GetUser() const {
	return m_user;
}

void Authenticator::RegisterAuthenticationResultListener(AuthenticationResultListener listener) {
	m_authenticationResultListeners.push_back(listener);
}

void Authenticator::UnregisterAuthenticationResultListener(AuthenticationResultListener listener) {
	m_authenticationResultListeners.remove(listener);
}

void Authenticator::NotifyAuthenticationResultListeners(AuthenticationResult result) const {
	for (auto listener : m_authenticationResultListeners) {
		(listener)(result);
	}
}

void Authenticator::RestReceived(const std::string& rest) {
	json11::Json json = JsonProvider::ParseString(rest);
	const bool authenticationSuccess = JsonToAuthenticationResult(json);

	AuthenticationResult result = AuthenticationResult::FAILURE;
	if (authenticationSuccess) {
		result = AuthenticationResult::SUCCESS;
		HandleAuthenticationSuccess();
    } else {
		HandleAuthenticationFailure();
    }

	NotifyAuthenticationResultListeners(result);
}

void Authenticator::HandleAuthenticationSuccess() {
	IEngine::getEngine()->GetUserProvider()->SetUser(GetUser());
	IEngine::getEngine()->GetUserProvider()->LogIn();
}

void Authenticator::HandleAuthenticationFailure() {
	IEngine::getEngine()->GetUserProvider()->LogOut();
}

bool Authenticator::JsonToAuthenticationResult(const json11::Json json) const {
	auto response = json["success"];
	return response.bool_value();
}

void Authenticator::SendAuthenticationRequest() {
	RequestBuilder requestBuilder;
	requestBuilder.SetEndpoint("authenticate");
	requestBuilder.AddUser(m_user);

	std::string requestString = requestBuilder.GetRequestString();
	m_authenticationRequestId = m_restConnector->SendRequest(requestString, this);
}
