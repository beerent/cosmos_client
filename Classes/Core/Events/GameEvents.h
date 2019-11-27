//
//  GameEvents.h
//  Projectios2
//
//  Created by Kamil Konecko on 6/15/15.
//
//

#ifndef Projectios2_GameEvents_h
#define Projectios2_GameEvents_h
#include "Core/Events/EventDispatcher.h"
#include "../../glm/glm.hpp"
#include "Core/StringManager/StringManager.h"
#include "Core/Input/InputDataTypes.h"

namespace GameEvents
{
    enum GameEventsEnum
    {
        CALCULATE_IMPULSE = 0,
        TIME_EVENT,
        PROPULTION_ENGINE_POWER_EVENT,
        GAME_DELTA_TIME_EVENT,
        FIRE_EVENT,
		MOVE_FORWARD_START,
		MOVE_FORWARD_STOP,
		MOVE_BACKWARDS_START,
		MOVE_BACKWARDS_STOP,
		TURN_LEFT_START,
		TURN_LEFT_STOP,
		TURN_RIGHT_START,
		TURN_RIGHT_STOP,
		REQUEST_CHARACTER_INPUT,
        GAME_EVENT_COUNT
    };

    class GameDeltaTimeEvent : public Event
    {
    public:
        GameDeltaTimeEvent(float deltaTime):mDeltaTime(deltaTime), Event(GAME_DELTA_TIME_EVENT){}
        
        float getDeltaTime() const {return mDeltaTime;}
        
    private:
        float mDeltaTime;
    };

    
    class TimeEvent : public Event
    {
    public:
        TimeEvent(STRING_ID name, int firedCount):mEventName(name), mFiredCount(firedCount), Event(TIME_EVENT){}
        
        STRING_ID getEventName()  const {return mEventName;}
        int       getFiredCount() const {return mFiredCount;}
        
    private:
        STRING_ID mEventName;
        int       mFiredCount;
    };
    
    class PropultionEnginePowerEvent: public Event
    {
    public:
        PropultionEnginePowerEvent(STRING_ID groupSubID, float power, float velocity = 0, bool useVelocity = false ):
                                mGroupSubID(groupSubID), mPower(power), mDesiredVelocity(velocity), mUseVelocity(useVelocity),
                                Event(PROPULTION_ENGINE_POWER_EVENT){}
        STRING_ID getGroupSubID()const {return mGroupSubID;}
        float     getPower()const {return mPower;}
        float     getVelocity()const {return mDesiredVelocity;}
        bool      useVelocity()const {return mUseVelocity;}
    
    
    private:
        STRING_ID mGroupSubID;
        float     mPower;
        float     mDesiredVelocity;
        bool      mUseVelocity;
    };
    
    class FireEvent: public Event
    {
    public:
        FireEvent(STRING_ID groupSubID, glm::vec3 targetPos = glm::vec3(0,0,0), bool targeted = false, STRING_ID type = StringManager::UNDEFINED): mGroupSubID(groupSubID), mTargetPos(targetPos), mTargeted(targeted),
        mProjectileType(type), Event(FIRE_EVENT){}
        STRING_ID getGroupSubID()const {return mGroupSubID;}
        STRING_ID getProjectileType()const {return mProjectileType;}
        const glm::vec3& getTargetPos()const {return mTargetPos;}
        bool isTargeted()const {return mTargeted;}
        
    private:
        STRING_ID mGroupSubID;
        STRING_ID mProjectileType;
        glm::vec3 mTargetPos;
        bool      mTargeted;
    };
    
    
    class CalculateImpulseEvent : public Event
    {
    public:
        struct ImpulseAggregator
        {
            glm::vec3 mForce;
            glm::vec3 mRelativePoint;
        };
        
        CalculateImpulseEvent(ImpulseAggregator* aggregator):
                                mAggregator(aggregator),
                                Event(CALCULATE_IMPULSE){}
        
        ImpulseAggregator* GetImpulseAggregator(){return mAggregator;}
        
        ImpulseAggregator* mAggregator;
    };

	class RequestCharacterInputEvent : public Event
	{
	public:
		RequestCharacterInputEvent(CharacterMovementInput& characterMovementInput) :
			m_characterMovementInput(&characterMovementInput),
			Event(REQUEST_CHARACTER_INPUT) {}
	
		void SetCharacterInput(const CharacterMovementInput& input) const {
			*m_characterMovementInput = input;
		}

	private:
		mutable CharacterMovementInput* m_characterMovementInput;
	};
}
#endif
