//
//  TimeEventGenerator.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#include "Core/Events/TimeEventGenerator.h"
#include "IEngine.h"
#include <list>
#include "Core/Events/EventDispatcher.h"
#include "Core/Events/GameEvents.h"
#include "Core/Debug/Logger/Logger.h"

TimeEventGenerator::TimeEventGenerator() : mAcumulator(0), mInitialized(false), mResetPending(true)
{

}

TimeEventGenerator::~TimeEventGenerator()
{
    reset();
    if(mInitialized)
    {
        IEngine* engine = IEngine::getEngine();
        engine->unregisterForDeltaTimeEvents(this);
    }
}

void TimeEventGenerator::setupTimeEvent(STRING_ID eventName, float startTime, int repeatCount, float repeatInterval)
{
    TimeEventData& event    = mPendingActiveEvents[TimeEvent(eventName,startTime)];
    
    event.mEventName        = eventName;
    event.mStartTime        = startTime;
    event.mRepeatCount      = repeatCount;
    event.mRepeatInterval   = repeatInterval;
    event.mFiredCount       = 0;
    
}

void TimeEventGenerator::reset()
{
    if(mInitialized == false)
    {
        IEngine* engine = IEngine::getEngine();
        engine->registerForDeltaTimeEvents(this);
        mInitialized = true;
    }

	mResetPending = true;
    mAcumulator = 0;
	mPendingActiveEvents.clear();
}

void TimeEventGenerator::OnHandleEvent(const Event& event)
{
    if(event.GetEventType() != GameEvents::GAME_DELTA_TIME_EVENT)
    {
        Logger::Error("__FUNCTION__ Unexpected event!");
        return;
    }

	if (mResetPending) {
		mResetPending = false;
		mActiveEvents = mPendingActiveEvents;
	}
    
    const GameEvents::GameDeltaTimeEvent& timeEvent = static_cast<const GameEvents::GameDeltaTimeEvent& >(event);
    
    mAcumulator += timeEvent.getDeltaTime();

    StrIDtoTimeEventDataMap::iterator it = mActiveEvents.begin();
    
    typedef std::list<TimeEvent> StrIDList;
    StrIDList deleteList;
    
    while(it != mActiveEvents.end())
    {
        TimeEventData& event = it->second;
        
        if(event.mStartTime < mAcumulator)
        {
            GameEvents::TimeEvent timeEvent(event.mEventName, event.mFiredCount);
            mDispatcher.dispatchEvent(timeEvent);
            event.mFiredCount++;
            
            if(event.mRepeatCount > 0)
            {
                --event.mRepeatCount;
                event.mStartTime += event.mRepeatInterval;
            }
            else
            {
                deleteList.push_back(it->first);
            }
        }
        
		if (mResetPending) {
			return;
		}

        ++it;
    }
    
    StrIDList::iterator delIt = deleteList.begin();
    while(delIt != deleteList.end())
    {
        StrIDtoTimeEventDataMap::iterator removeIt = mActiveEvents.find(*delIt);		
        mActiveEvents.erase(removeIt);
        ++delIt;
    }
}


void TimeEventGenerator::registerForEventType(int eventType, IEventHandler* handler)
{
    mDispatcher.registerForEventType(eventType, handler);
}

void TimeEventGenerator::unRegisterForEventType(int eventType, IEventHandler* handler)
{
    mDispatcher.unRegisterForEventType(eventType, handler);
}

