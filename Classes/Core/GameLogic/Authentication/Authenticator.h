#pragma once

#include <Core/Net/IRestConnector.h>
#include <Core/Net/IRestReceiver.h>
#include <Core/Net/Json/JsonProvider.h>
#include <Core/User/User.h>
#include <Core/OpenSource/FastDelegate.h>

#include <list>

class Authenticator : public IRestReceiver {
public:
	enum AuthenticationResult {
		FAILURE,
		SUCCESS
	};
	typedef fastdelegate::FastDelegate1<AuthenticationResult> AuthenticationResultListener;

	Authenticator();
	~Authenticator();
	void SetRestConnector(IRestConnector* connector);
	void SetUser(const User& user);
	User GetUser() const;
	void SendAuthenticationRequest();

	void RegisterAuthenticationResultListener(AuthenticationResultListener listener);
	void UnregisterAuthenticationResultListener(AuthenticationResultListener listener);

	virtual void RestReceived(const std::string& rest);

private:
	IRestConnector* m_restConnector;
	User m_user;

	std::string m_authenticationRequestId;

	std::list<AuthenticationResultListener> m_authenticationResultListeners;

	bool JsonToAuthenticationResult(const json11::Json json) const;
	void NotifyAuthenticationResultListeners(AuthenticationResult result) const;

	void HandleAuthenticationSuccess();
	void HandleAuthenticationFailure();
};