#pragma once

#include <Core/User/User.h>
#include <string>

struct ChallengeProfileData {
    ChallengeProfileData(int count, int highScore, int leaderboardPosition) : count(count), highScore(highScore), leaderboardPosition(leaderboardPosition) {}
    
    const int count;
    const int highScore;
    const int leaderboardPosition;
    
    int GetCount() {
        return count;
    }
    
    int GetHighScore() {
        return highScore;
    }
    
    int GetLeaderboardPosition(){
        return leaderboardPosition;
    }
};

class UserProfile {
    
public:
    UserProfile(const User& user, const ChallengeProfileData& challengeProfileData);
    ChallengeProfileData GetChallengeProfileData() const;
    
private:
    User m_user;
    ChallengeProfileData m_challengeProfileData;
};
