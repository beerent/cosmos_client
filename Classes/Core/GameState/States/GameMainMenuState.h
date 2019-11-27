#pragma once

#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/UIState/UIStateMachine.h"

class GameMainMenuState : public BaseStateDepricated, public IUIStateMachineEventHandler
{
public:
    
    GameMainMenuState(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener){};
    CONST_STRING_DEC(MAIN_MENU_STATE)
	CONST_STRING_DEC(LEVEL_NAME)

    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual STRING_ID GetStateID(){return MAIN_MENU_STATE;}    
};
