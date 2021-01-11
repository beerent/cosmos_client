//
//  UIStateMachine.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/1/14.
//
//

#include "Core/GUI/UIState/UIStateMachine.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GUI/Widgets/CameraControlWidget.h"
#include "IEngine.h"
#include "Core/GameState/GameStateMachine.h"
#include "Core/Debug/DebugLogic.h"
#include "Core/GUI/UIState/States/UIStateDebugControls.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"

#include "Core/GUI/UIState/States/Authentication/UIStateAuthInput.h"

#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h"
#include "Core/GUI/UIState/States/Live/UIStateCosmosLiveLobby.h"

UIStateMachine* UIStateMachine::mInstance = 0;

UIStateMachine* UIStateMachine::GetInstance() {
    if(mInstance == 0) {
        mInstance = new UIStateMachine();
    }
    
    return mInstance;
};

UIStateMachine::UIStateMachine() : mUIEventHandler(nullptr) {}

void UIStateMachine::OnStateCompletedTransition(BaseStateDepricated* state) {
	if (mUIEventHandler == nullptr) {
		return;
	}

	switch (mStateAction) {
	case STATE_ENTERING:
		mUIEventHandler->OnUIEnterState(*state);
		break;
	case STATE_EXITING:
		mUIEventHandler->OnUIExitState(*state);
		break;
	default:
		break;
	}

	StateMachineDepricated::OnStateCompletedTransition(state);
}

BaseStateDepricated* UIStateMachine::CreateState(STRING_ID stateName) {
	if (stateName == UIStateDebugControls::UI_STATE_DEBUG_CONTROLS) {
		return new UIStateDebugControls(this);
	}

	else if (stateName == UIStateMainMenu::UI_STATE_MAINMENU) {
		return new UIStateMainMenu(this);
	}

	else if (stateName == UIStateAuthInput::UI_STATE_AUTH_INPUT) {
		return new UIStateAuthInput(this);
	}

	else if (stateName == UIStateChallengeMainMenu::UI_STATE_CHALLENGE_MAIN_MENU) {
		return new UIStateChallengeMainMenu(this);
	}

	else if (stateName == UIStateChallengeMode::UI_STATE_CHALLENGE_MODE) {
		return new UIStateChallengeMode(this);
	}
    
    else if (stateName == UIStateCosmosLiveLobby::UI_STATE_COSMOS_LIVE_LOBBY) {
        return new UIStateCosmosLiveLobby(this);
    }

	else {
		Logger::Error("Requested Unknown State!");
	}

	return NULL;
}
