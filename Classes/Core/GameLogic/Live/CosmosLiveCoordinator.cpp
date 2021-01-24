#include <Core/GameLogic/Live/CosmosLiveCoordinator.h>

#include <Core/Util/Date/DateUtil.h>

#include <Core/Net/RequestBuilder.h>
#include <IEngine.h>

namespace requests {
    const std::string LIVE = "live";
}

CosmosLiveCoordinator::CosmosLiveCoordinator() : m_timer(this), m_cosmosLiveSessionUpdateListener(nullptr) {
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

    json11::Json json = JsonProvider::ParseString(rest);
    CosmosLiveSession session = RestToCosmosLiveSession(json);
    if (ShouldUpdateSession(session)) {
        UpdateSession(session);
    }
}

void CosmosLiveCoordinator::UpdateSession(const CosmosLiveSession& session) {
    m_currentLiveSession = session;
    
    if (m_cosmosLiveSessionUpdateListener != nullptr) {
        m_cosmosLiveSessionUpdateListener->OnCosmosLiveSessionUpdated(session);
    }
}

void CosmosLiveCoordinator::RegisterCosmosLiveSessionUpdateListener(ICosmosLiveSessionUpdateListener* listener) {
    m_cosmosLiveSessionUpdateListener = listener;
}

void CosmosLiveCoordinator::DeregisterCosmosLiveSessionUpdateListener() {
    m_cosmosLiveSessionUpdateListener = nullptr;
}
    
bool CosmosLiveCoordinator::ShouldUpdateSession(const CosmosLiveSession& session) const {
    return !SessionsAreEqual(m_currentLiveSession, session);
}

bool CosmosLiveCoordinator::SessionsAreEqual(const CosmosLiveSession& sessionA, const CosmosLiveSession& sessionB) const {
    const std::string sessionAHash = sessionA.GetHash();
    const std::string sessionBHash = sessionB.GetHash();
    
    return sessionAHash == sessionBHash;
}

CosmosLiveSession CosmosLiveCoordinator::RestToCosmosLiveSession(const json11::Json& json) const {
    const json11::Json sessionJson = json["payload"]["cosmos_live_session"];
    std::string state = sessionJson["state"].string_value();
    std::string startDate = sessionJson["start"].string_value();
    int round = sessionJson["round"].int_value();
    int roundSecondsRemaining = sessionJson["round_seconds_remaining"].int_value();
    int playerCount = sessionJson["player_count"].int_value();
    
    const json11::Json chatJson = json["payload"]["chat"];
    std::vector<CosmosLiveChat> cosmosLiveChats = GetChatsFromJson(chatJson);
    
    CosmosLiveSession cosmosLiveSession(StringToCosmosLiveState(state), Util::StringToDateTime(startDate), round, roundSecondsRemaining, playerCount);
    return cosmosLiveSession;
}

std::vector<CosmosLiveChat> CosmosLiveCoordinator::GetChatsFromJson(const json11::Json& json) const {
    std::vector<CosmosLiveChat> cosmosLiveChats;
    
    auto chatsJson = json.array_items();
    for (const auto& chatJson : chatsJson) {
        std::string user = chatJson["user"].string_value();
        std::string message = chatJson["message"].string_value();
        std::string added = chatJson["added"].string_value();
        
        cosmosLiveChats.push_back(CosmosLiveChat(user, message, added));
    }
    
    return cosmosLiveChats;
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
