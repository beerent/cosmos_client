//
//  GameStateMachine.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/1/14.
//
//

#include "Core/GameState/GameStateMachine.h"
#include "Core/GameState/States/GameMainMenuState.h"

GameStateMachine* GameStateMachine::mInstance = 0;

GameStateMachine* GameStateMachine::GetInstance() {
    if(mInstance == 0) {
        mInstance = new GameStateMachine();
    }
    
    return mInstance;
};

GameStateMachine::GameStateMachine() {}

BaseStateDepricated* GameStateMachine::CreateState(STRING_ID stateName) {

    if(stateName == GameMainMenuState::MAIN_MENU_STATE) {
        return new GameMainMenuState(this);
	}
	
	else {
        Logger::Error("Requested Unknown State!");
    }
    
    return NULL;
}


