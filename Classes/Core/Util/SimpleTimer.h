#pragma once

#include <set>
#include <map>
#include <IEngine.h>
#include <chrono>

namespace Timer {

enum class TimerType {
    INVALID = -1,
    CURSOR_BLINK_550_MS,
    MESSAGE_SCROLL_TIMER,
    CHALLENGE_QUESTION_TIMER_100_MS,
    CHALLENGE_QUESTION_TIMER_1000_MS,
    CHALLENGE_POST_ANSWER_TIMER,
    COSMOS_LIVE_REFRESH_TIMER
};



class SimpleTimerListener {
public:
    virtual void OnTimerEvent(TimerType type) = 0;
};

class SimpleTimer {
public:
    
    SimpleTimer(SimpleTimerListener* listener);
    ~SimpleTimer();
    
    void RegisterTimer(TimerType type);
    void DeregisterTimer(TimerType type);
    
    void ResetTimer(TimerType type);
    
    static std::chrono::steady_clock::time_point GetCurrentTime();
    
private:
    
    IEngine::onDeltaTimeEventCallBack m_onDeltaTimeCallBack;
    std::map<TimerType, std::chrono::steady_clock::time_point> m_registeredTimers;
    SimpleTimerListener* m_timerListener;
    bool m_timersAltered;
    
    void OnTimerIteration(float duration);
};

}
