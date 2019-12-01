#include <Core/GUI/Components/UIComponentFactory.h>
#include <Core/GUI/Components/UIComponent.h>
#include <Core/Util/SimpleTimer.h>
#include <Core/OpenSource/FastDelegate.h>


namespace Timer {

std::chrono::steady_clock::time_point GetCurrentTime() {
    return std::chrono::steady_clock::now();
}

std::map<TimerType, double> TIMER_MAP = {
    { TimerType::CURSOR_BLINK_500_MS, 550 }
};

SimpleTimer::SimpleTimer(SimpleTimerListener* listener) : m_timerListener(listener) {
    m_onDeltaTimeCallBack.bind(this, &SimpleTimer::OnTimerIteration);
    IEngine::getEngine()->registerForDeltaTimeEvents(m_onDeltaTimeCallBack);
}

SimpleTimer::~SimpleTimer() {
IEngine::getEngine()->unregisterForDeltaTimeEvents(m_onDeltaTimeCallBack);
}

void SimpleTimer::RegisterTimer(TimerType type) {
    m_registeredTimers[type] = GetCurrentTime();
}

void SimpleTimer::DeregisterTimer(TimerType type) {
    auto timerIt = m_registeredTimers.find(type);
    if (timerIt != m_registeredTimers.end()) {
        m_registeredTimers.erase(timerIt);
    }
}

void SimpleTimer::ResetTimer(TimerType type) {
    RegisterTimer(type);
}

void SimpleTimer::OnTimerIteration(float duration) {
    for (auto it = m_registeredTimers.begin(); it != m_registeredTimers.end(); it++) {
        const TimerType type = it->first;
        const std::chrono::steady_clock::time_point typeTime = it->second;
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(GetCurrentTime() - typeTime).count();
        if (elapsedTime > TIMER_MAP[type]) {
            m_timerListener->OnTimerEvent(type);
            m_registeredTimers[type] = GetCurrentTime();
        }
    }
}
}
