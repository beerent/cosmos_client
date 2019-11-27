//
//  TimeEventGenerator.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#ifndef __Projectios2__TimeEventGenerator__
#define __Projectios2__TimeEventGenerator__

#include <stdio.h>
#include <map>
#include "IEngine.h"
#include "Core/Events/EventDispatcher.h"

class TimeEventGenerator : IEventHandler
{
public:
	TimeEventGenerator();
	~TimeEventGenerator();

	void setupTimeEvent(STRING_ID eventName, float startTime, int repeatCount, float repeatInterval);
	void dispatchEvent(const Event& event) const;
	void registerForEventType(int eventType, IEventHandler* handler);
	void unRegisterForEventType(int eventType, IEventHandler* handler);
	void reset();

	void OnHandleEvent(const Event& event);
	float getCurrentTime() const {
		return mAcumulator;
	}
    
private:
    
    struct TimeEventData
    {
        STRING_ID       mEventName;
        float           mStartTime;
        int             mRepeatCount;
        float           mRepeatInterval;
        int             mFiredCount;
    };
    
	using TimeEvent = std::pair<STRING_ID, float>;
    typedef std::map<TimeEvent, TimeEventData> StrIDtoTimeEventDataMap;
    StrIDtoTimeEventDataMap mActiveEvents;
	StrIDtoTimeEventDataMap mPendingActiveEvents;

    float mAcumulator;
    EventDispatcher mDispatcher;
    
    bool mInitialized;
	bool mResetPending;

};

#endif /* defined(__Projectios2__TimeEventGenerator__) */
