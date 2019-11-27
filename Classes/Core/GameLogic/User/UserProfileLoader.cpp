#include <Core/GameLogic/User/UserProfileLoader.h>
#include <Core/Net/RequestBuilder.h>

UserProfileLoader::UserProfileLoader(const User& user) : m_user(user) {}

UserProfileLoader::~UserProfileLoader() {
    m_restConnector->CloseRequest(m_profileRequestId);
}

void UserProfileLoader::LoadUserProfile() {
    RequestBuilder requestBuilder;
    requestBuilder.SetEndpoint("getUserProfile");
    requestBuilder.AddParameter("username", m_user.GetUsername());
    
    std::string requestString = requestBuilder.GetRequestString();
    m_profileRequestId = m_restConnector->SendRequest(requestString, this);
}

void UserProfileLoader::RestReceived(const std::string& rest) {
    json11::Json json = JsonProvider::ParseString(rest);
    UserProfile profile = JsonToUserProfile(json);
    m_userProfileReceiver->OnProfileLoaded(profile);
}

UserProfile UserProfileLoader::JsonToUserProfile(json11::Json& json) const {
    json11::Json challengeJsonData = json["payload"]["challengeData"];
    
    int challengeCount = challengeJsonData["challengeCount"].int_value();
    int challengeHighScore = challengeJsonData["challengeHighScore"].int_value();
    int challengeLeaderboardPosition = challengeJsonData["challengeLeaderboardPosition"].int_value();
    ChallengeProfileData challengeProfileData(challengeCount, challengeHighScore, challengeLeaderboardPosition);
    
    UserProfile profile(m_user, challengeProfileData);
    return profile;
}

void UserProfileLoader::RegisterProfileLoadedListener(IUserProfileReceiver* userProfileReceiver) {
    m_userProfileReceiver = userProfileReceiver;
}

void UserProfileLoader::SetRestConnector(IRestConnector* connector) {
    m_restConnector = connector;
}
