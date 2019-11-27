//
//  BaseState.h
//  Projectios2
//
//  Created by Kamil Konecko on 1/24/15.
//
//

#ifndef __Projectios2__BaseState__
#define __Projectios2__BaseState__

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Debug/Logger/Logger.h"

class BaseStateDepricated;

class IStateChanageListenerDepricated
{
public:
    virtual void OnStateCompletedTransition( BaseStateDepricated* state) = 0;
    virtual void SetState(STRING_ID stateName) = 0;
};

class BaseStateDepricated
{
public:
    BaseStateDepricated(IStateChanageListenerDepricated* stateChangeListener);
    virtual ~BaseStateDepricated(){};
    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual STRING_ID GetStateID() = 0;
    
protected:
    
    void ChangeState(STRING_ID stateId);
    
private:
    IStateChanageListenerDepricated* m_stateChangeListener;
    
};

/*
template <class T> class BaseState;

template <class T> class IStateChanageListener
{
public:
	virtual void OnStateCompletedTransition(const  BaseState<T>* state) = 0;
	virtual void SetState(STRING_ID stateName) = 0;
};

template <class T> class BaseState
{
public:
	BaseState(IStateChanageListener<T>* stateChangeListener);
	virtual ~BaseState() {};
	virtual void OnEnterState();
	virtual void OnExitState();

	virtual STRING_ID GetStateID() = 0;

protected:

	void ChangeState(STRING_ID stateId);

private:
	IStateChanageListener<T>* m_stateChangeListener;

};
*/

#endif /* defined(__Projectios2__BaseState__) */
