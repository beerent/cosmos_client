#pragma once

#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/StateMachine.h"

#include <map>

class GameStateMachine : public StateMachineDepricated {
public:

    GameStateMachine();
    
    static GameStateMachine* GetInstance();
    
private:
    static GameStateMachine* mInstance;
    BaseStateDepricated *CreateState(STRING_ID stateName);
};
