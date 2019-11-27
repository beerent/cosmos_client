//
//  StateMachine.h
//  Projectios2
//
//  Created by Kamil Konecko on 1/24/15.
//
//

#ifndef __Projectios2__StateMachine__
#define __Projectios2__StateMachine__

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include <map>
#include "BaseState.h"
#include "IEngine.h"

class StateMachine : public IStateChanageListenerDepricated
{
public:

	CONST_STRING_DEC(BASE_STATE)

	enum STATE_ACTION
	{
		STATE_IDLE = 0,
		STATE_EXITING,
		STATE_ENTERING
	};

	typedef std::map<STRING_ID, STRING_ID> StringIDtoStringIDMap;

	StateMachine();
	~StateMachine();

	void PushStateParameters(STRING_ID parameterID, STRING_ID parameterValue);
	void SetState(STRING_ID stateName);
	void OnStateCompletedTransition(BaseStateDepricated* state);

	const StringIDtoStringIDMap* GetStateParameters()const { return &mStateParameters; };

private:

	StringIDtoStringIDMap mStateParameters;

	BaseStateDepricated *mActiveState;
	BaseStateDepricated *mEnteringState;
	STRING_ID mTargetState;

	virtual BaseStateDepricated *CreateState(STRING_ID stateName) = 0;
	void DestroyActiveState();

	STATE_ACTION mStateAction;

	IEngine::onDeltaTimeEventCallBack m_onDeltaTimeCallBack;
	void OnDeltaTimeUpdate(float deltaTime);
	void DoTransition();
};

class StateMachineDepricated : public IStateChanageListenerDepricated
{
public:
    
    CONST_STRING_DEC(BASE_STATE)
    
    enum STATE_ACTION
    {
        STATE_IDLE = 0,
        STATE_EXITING,
        STATE_ENTERING
    };
    
    typedef std::map<STRING_ID, STRING_ID> StringIDtoStringIDMap;
    
    StateMachineDepricated();
    ~StateMachineDepricated();
    
    void PushStateParameters(STRING_ID parameterID, STRING_ID parameterValue);
    void SetState(STRING_ID stateName);
    virtual void OnStateCompletedTransition(BaseStateDepricated* state);

    const StringIDtoStringIDMap* GetStateParameters()const { return &mStateParameters;};

protected:
	STATE_ACTION mStateAction;

private:
    
    StringIDtoStringIDMap mStateParameters;
    
    BaseStateDepricated *mActiveState;
    BaseStateDepricated *mEnteringState;
    STRING_ID mTargetState;
    
    virtual BaseStateDepricated *CreateState(STRING_ID stateName) = 0;
    void DestroyActiveState();
    

    IEngine::onDeltaTimeEventCallBack m_onDeltaTimeCallBack;
    void OnDeltaTimeUpdate(float deltaTime);
    void DoTransition();
};
#endif /* defined(__Projectios2__StateMachine__) */
