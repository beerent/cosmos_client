#include <Core/GameLogic/Authentication/Authenticator.h>
#include <Core/Net/RequestBuilder.h>
#include "IEngine.h"

Authenticator::Authenticator() : m_user(User("", "", "", UserAccessLevel::INVALID)), m_authenticationResultListener(nullptr) {}

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

void Authenticator::RegisterAuthenticationResultListener(IAuthenticationResultListener* listener) {
    m_authenticationResultListener = listener;
}

void Authenticator::UnregisterAuthenticationResultListener() {
    m_authenticationResultListener = nullptr;
}

void Authenticator::NotifyAuthenticationResultListeners(AuthenticationResult result) const {
    if (nullptr != m_authenticationResultListener) {
        m_authenticationResultListener->OnAuthenticationResultReceived(result);
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
}

void Authenticator::HandleAuthenticationFailure() {
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

void Authenticator::SendGuestAuthenticationRequest() {
    RequestBuilder requestBuilder;
    requestBuilder.SetEndpoint("guestAuthenticate");
    requestBuilder.AddUser(m_user);

    std::string requestString = requestBuilder.GetRequestString();
    m_authenticationRequestId = m_restConnector->SendRequest(requestString, this);
}
