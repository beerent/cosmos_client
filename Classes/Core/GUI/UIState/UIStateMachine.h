#pragma once

#include "Core/StringManager/StringManager.h"
#include "Core/GUI/Widgets/MainMenuWidget.h"
#include "Core/GUI/Components/UITouchButton.h"
#include "IEngine.h"

#include "Core/StringManager/StringManager.h"
#include <map>
#include "Core/Generic/StateMachine/StateMachine.h"


class IUIStateMachineEventHandler {
public:
	virtual void OnUIEnterState(BaseStateDepricated& state) {};
	virtual void OnUIExitState(BaseStateDepricated& state) {};
};

class UIStateMachine : public StateMachineDepricated {
public:

	UIStateMachine();
	~UIStateMachine() {
		int x = 54;
	}

	static UIStateMachine* GetInstance();

	void SetEventHandler(IUIStateMachineEventHandler& eventHandler) {
		mUIEventHandler = &eventHandler;
	}

	void UnsetEventHandler(IUIStateMachineEventHandler& eventHandler) {
		mUIEventHandler = NULL;
	}

	void OnStateCompletedTransition(BaseStateDepricated* state);
    
private:
    static UIStateMachine* mInstance;
    BaseStateDepricated *CreateState(STRING_ID stateName);
	IUIStateMachineEventHandler* mUIEventHandler;
};