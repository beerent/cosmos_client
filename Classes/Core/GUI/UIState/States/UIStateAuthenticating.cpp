#include "Core/GUI/UIState/States/UIStateAuthenticating.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"
#include "Core/GameState/GameStateMachine.h"
#include "Core/GameState/States/GamePlayState.h"

CONST_STRING_DEF(UIStateAuthenticating, UI_STATE_AUTHENTICATING)

void UIStateAuthenticating::OnEnterState() {
	UIComponent *root = IEngine::getEngine()->getUIRoot();
	const float LABEL_HEIGHT = 90.0;
	const float LABEL_WIDTH = 585.0;

	UIComponentFactory* factory = UIComponentFactory::getInstance();

	mHeader = factory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_CENTER, "Logging In...");
	root->addChild(mHeader);
}

void UIStateAuthenticating::OnExitState() {
	mHeader->release();
	delete mHeader;
	BaseStateDepricated::OnExitState();
}

