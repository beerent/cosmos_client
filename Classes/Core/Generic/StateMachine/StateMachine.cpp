#include "StateMachine.h"
#include "BaseState.h"
#include "IEngine.h"

CONST_STRING_DEF(StateMachineDepricated, BASE_STATE)

StateMachineDepricated::StateMachineDepricated():mActiveState(NULL), mEnteringState(NULL){
    mTargetState = StringManager::UNDEFINED;
}

void StateMachineDepricated::SetState(StringManager::StringID stateName) {
    m_onDeltaTimeCallBack.bind(this, &StateMachineDepricated::OnDeltaTimeUpdate);
    IEngine::getEngine()->registerForDeltaTimeEvents(m_onDeltaTimeCallBack);
    mTargetState = stateName;
}

void StateMachineDepricated::OnStateCompletedTransition(BaseStateDepricated* state) {
    switch (mStateAction) {
        case STATE_ENTERING:
            mStateAction = STATE_IDLE;
            mStateParameters.clear();
            break;
        case STATE_EXITING:
            mStateAction = STATE_ENTERING;
            DestroyActiveState();
            mActiveState = mEnteringState;
            mEnteringState = NULL;
            mActiveState->OnEnterState();
            break;
        default:
            Logger::Error("Unexpected StateCompleted Transition");
            break;
    }
}

void StateMachineDepricated::DestroyActiveState() {
    delete (mActiveState);
}

StateMachineDepricated::~StateMachineDepricated() {
    if(mActiveState != NULL) {
        DestroyActiveState();
    }
}

void StateMachineDepricated::PushStateParameters(STRING_ID parameterID, STRING_ID parameterValue) {
    mStateParameters[parameterID] = parameterValue;
}

void StateMachineDepricated::OnDeltaTimeUpdate(float deltaTime) {
    if(mTargetState != StringManager::UNDEFINED) {
        DoTransition();
    }
}

void StateMachineDepricated::DoTransition() {
    mEnteringState = CreateState(mTargetState);
    mTargetState = StringManager::UNDEFINED;

    if(mActiveState != NULL) {
        mStateAction = STATE_EXITING;
        mActiveState->OnExitState();
    } else if(mActiveState == NULL) {
        mStateAction = STATE_ENTERING;
        mActiveState = mEnteringState;
        mActiveState->OnEnterState();
    }
    
    IEngine::getEngine()->unregisterForDeltaTimeEvents(m_onDeltaTimeCallBack);
}