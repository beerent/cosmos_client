//
//  EventDispatcher.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/2/14.
//
//

#include "Core/Events/EventDispatcher.h"


void EventDispatcher::dispatchEvent(const Event &event) const
{
    const MapOfListOfIEventHandlers::const_iterator it = mMapOfHandlers.find(event.GetEventType());

    if(it != mMapOfHandlers.end())
    {
        const ListOfIEventHandlers& eventHandlerList = it->second;
        ListOfIEventHandlers::const_iterator          listIt = eventHandlerList.begin();
        const ListOfIEventHandlers::const_iterator listItEnd = eventHandlerList.end();
        
        while (listIt != listItEnd)
        {
            (*listIt)->OnHandleEvent(event);
            ++listIt;
        }
    }
}

void EventDispatcher::registerForEventType(int eventType, IEventHandler* handler)
{
    //No check for duplicates! We should at least add an assert here for debug builds!
    mMapOfHandlers[eventType].push_back(handler);
}

void EventDispatcher::unRegisterForEventType(int eventType, IEventHandler* handler)
{
    const MapOfListOfIEventHandlers::iterator it = mMapOfHandlers.find(eventType);
    
    if(it != mMapOfHandlers.end())
    {
        ListOfIEventHandlers& eventHandlerList = it->second;
        eventHandlerList.remove(handler);
    }
}
