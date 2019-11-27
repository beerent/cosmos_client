//
//  BaseState.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 1/24/15.
//
//

#include "BaseState.h"

BaseStateDepricated::BaseStateDepricated(IStateChanageListenerDepricated* stateChangeListener)
{
    m_stateChangeListener = stateChangeListener;
}

void BaseStateDepricated::OnEnterState()
{
    m_stateChangeListener->OnStateCompletedTransition(this);
}

void BaseStateDepricated::OnExitState()
{
    m_stateChangeListener->OnStateCompletedTransition(this);
}

void BaseStateDepricated::ChangeState(STRING_ID stateId)
{
    m_stateChangeListener->SetState(stateId);
}
