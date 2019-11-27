//
//  EventDispatcher.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/2/14.
//
//

#ifndef Projectios1_EventDispatcher_h
#define Projectios1_EventDispatcher_h

#include "Core/StringManager/StringManager.h"
#include <map>
#include <list>
#include "IEngine.h"

class Event
{
public:
    Event(int eventType):mEventType(eventType){};
    
    unsigned int GetEventType() const {return mEventType;}

private:
    unsigned int mEventType;
};

class IEventHandler
{
public:
    virtual void OnHandleEvent(const Event& event) = 0;
};

class IEventDispatcher
{
public:
    virtual void dispatchEvent(const Event& event) const;
    virtual void registerForEventType(int eventType, IEventHandler* handler);
    virtual void unRegisterForEventType(int eventType, IEventHandler* handler);
};

class EventDispatcher
{
public:
    EventDispatcher(){};
    ~EventDispatcher(){};
    
    
    void dispatchEvent(const Event& event) const;
    void registerForEventType(int eventType, IEventHandler* handler);
    void unRegisterForEventType(int eventType, IEventHandler* handler);
    
private:
    
    typedef std::list<IEventHandler*>           ListOfIEventHandlers;
    typedef std::map<int, ListOfIEventHandlers> MapOfListOfIEventHandlers;
    
    MapOfListOfIEventHandlers mMapOfHandlers;
};

#endif
