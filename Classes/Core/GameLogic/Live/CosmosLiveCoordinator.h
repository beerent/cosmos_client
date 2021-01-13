#pragma once

#include <Core/GameLogic/Live/CosmosLiveStates.h>
#include <Core/GameLogic/Live/Session/CosmosLiveSession.h>

#include <Core/Net/Json/JsonProvider.h>
#include <Core/Net/IRestConnector.h>
#include <Core/Net/IRestReceiver.h>

#include <Core/Util/SimpleTimer.h>

/*
 * Responsibilities:
 *  continuously get updates for a user
 *  maintain state of user
 *  report on state changes
 *
 
 
 
 * while true:
*   - get state
*   - if state is different than previously recorded state, report change with associated data
*/

class CosmosLiveCoordinator : public IRestReceiver, Timer::SimpleTimerListener {
public:
    CosmosLiveCoordinator();
    ~CosmosLiveCoordinator();
    void Start();
    
    virtual void OnTimerEvent(Timer::TimerType type);
    virtual void RestReceived(const std::string& rest);

private:
    CosmosLiveState m_currentState;
    
    IRestConnector* m_restConnector;
    std::string m_liveRequestId;
    
    Timer::SimpleTimer m_timer;
    
    void GetCosmosLiveUpdate();
    bool CosmosLiveUpdateRequestIsPending() const;
    void CancelCosmosLiveUpdateRequest();
    void CloseCosmosLiveUpdateRequest();
    
    CosmosLiveSession RestToCosmosLiveSession(const json11::Json& json) const;
    
    void RegisterTimers();
    void DeregisterTimers();
};
