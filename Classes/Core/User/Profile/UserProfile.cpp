#include <Core/User/Profile/UserProfile.h>

UserProfile::UserProfile(const User& user, const ChallengeProfileData& challengeProfileData) : m_user(user), m_challengeProfileData(challengeProfileData) {}

ChallengeProfileData UserProfile::GetChallengeProfileData() const {
    return m_challengeProfileData;
}
