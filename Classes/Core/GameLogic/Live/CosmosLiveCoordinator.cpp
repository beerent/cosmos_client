#include <Core/GameLogic/Live/CosmosLiveCoordinator.h>
#include <Core/Net/RequestBuilder.h>
#include <IEngine.h>

namespace requests {
    const std::string LIVE = "live";
}

CosmosLiveCoordinator::CosmosLiveCoordinator() : m_currentState(CosmosLiveState::INVALID), m_timer(this) {
    m_restConnector = IEngine::getEngine()->GetRestConnector();
}

CosmosLiveCoordinator::~CosmosLiveCoordinator() {
    DeregisterTimers();
}

void CosmosLiveCoordinator::Start() {
    RegisterTimers();
}

void CosmosLiveCoordinator::RegisterTimers() {
    m_timer.RegisterTimer(Timer::TimerType::COSMOS_LIVE_REFRESH_TIMER);
}

void CosmosLiveCoordinator::DeregisterTimers() {
    m_timer.DeregisterTimer(Timer::TimerType::COSMOS_LIVE_REFRESH_TIMER);
}

void CosmosLiveCoordinator::RestReceived(const std::string& rest) {
    CloseCosmosLiveUpdateRequest();
}

void CosmosLiveCoordinator::OnTimerEvent(Timer::TimerType type) {
    switch (type) {
        case Timer::TimerType::COSMOS_LIVE_REFRESH_TIMER:
            GetCosmosLiveUpdate();
            break;
        
        default:
            break;
    }
}

void CosmosLiveCoordinator::GetCosmosLiveUpdate() {
    if (CosmosLiveUpdateRequestIsPending()) {
        CancelCosmosLiveUpdateRequest();
    }
    
    RequestBuilder requestBuilder;
    requestBuilder.AddUser(IEngine::getEngine()->GetUserProvider()->GetUser());
    requestBuilder.SetEndpoint(requests::LIVE);

    std::string requestString = requestBuilder.GetRequestString();
    m_liveRequestId = m_restConnector->SendRequest(requestString, this);
}

bool CosmosLiveCoordinator::CosmosLiveUpdateRequestIsPending() const {
    return !m_liveRequestId.empty();
}

void CosmosLiveCoordinator::CancelCosmosLiveUpdateRequest() {
    m_restConnector->CloseRequest(m_liveRequestId);
    CloseCosmosLiveUpdateRequest();
}

void CosmosLiveCoordinator::CloseCosmosLiveUpdateRequest() {
    m_liveRequestId.clear();
}
