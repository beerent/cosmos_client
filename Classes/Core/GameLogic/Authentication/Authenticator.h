#pragma once

#include <Core/Net/IRestConnector.h>
#include <Core/Net/IRestReceiver.h>
#include <Core/Net/Json/JsonProvider.h>
#include <Core/User/User.h>
#include <Core/OpenSource/FastDelegate.h>

#include <list>

enum AuthenticationResult {
    FAILURE,
    SUCCESS
};

class IAuthenticationResultListener {
public:
    virtual void OnAuthenticationResultReceived(AuthenticationResult result) = 0;
};

class Authenticator : public IRestReceiver {
public:
	typedef fastdelegate::FastDelegate1<AuthenticationResult> AuthenticationResultListener;

	Authenticator();
	~Authenticator();
	void SetRestConnector(IRestConnector* connector);
	void SetUser(const User& user);
	User GetUser() const;
    
    void SendGuestAuthenticationRequest();
    void SendAuthenticationRequest();

	void RegisterAuthenticationResultListener(IAuthenticationResultListener* listener);
	void UnregisterAuthenticationResultListener();

	virtual void RestReceived(const std::string& rest);

private:
	IRestConnector* m_restConnector;
	User m_user;

	std::string m_authenticationRequestId;

	IAuthenticationResultListener* m_authenticationResultListener;

	bool JsonToAuthenticationResult(const json11::Json json) const;
	void NotifyAuthenticationResultListeners(AuthenticationResult result) const;

	void HandleAuthenticationSuccess();
	void HandleAuthenticationFailure();
};
