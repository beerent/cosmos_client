#pragma once

#include <Core/User/User.h>
#include <Core/User/Profile/UserProfile.h>
#include <Core/Net/IRestConnector.h>
#include <Core/Net/IRestReceiver.h>
#include <Core/Net/Json/JsonProvider.h>

class IUserProfileReceiver {
public:
    virtual void OnProfileLoaded(const UserProfile& userProfile) = 0;
};


class UserProfileLoader : public IRestReceiver {
public:
    UserProfileLoader(const User& user);
    ~UserProfileLoader();
    void LoadUserProfile();
    void RegisterProfileLoadedListener(IUserProfileReceiver* userProfileReceiver);
    void SetRestConnector(IRestConnector* connector);
    
    virtual void RestReceived(const std::string& rest);
    
private:
    User m_user;
    IUserProfileReceiver* m_userProfileReceiver;
    IRestConnector* m_restConnector;
    std::string m_profileRequestId;
    
    UserProfile JsonToUserProfile(json11::Json& json) const;
};
