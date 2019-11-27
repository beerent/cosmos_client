#include "Core/GUI/UIState/States/Authentication/UIStateAuthAuthenticating.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"
#include "Core/GameState/GameStateMachine.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"

#include <Core/User/UserProvider.h>

CONST_STRING_DEF(UIStateAuthAuthenticating, UI_STATE_AUTH_AUTHENTICATING)

void UIStateAuthAuthenticating::OnEnterState() {
	DisplayUI();
	BaseStateDepricated::OnEnterState();
	SubmitAuthenticationRequest();
}

void UIStateAuthAuthenticating::OnExitState() {
	mHeader->release();
	delete mHeader;
	BaseStateDepricated::OnExitState();
}

void UIStateAuthAuthenticating::DisplayUI() {
	UIComponent *root = IEngine::getEngine()->getUIRoot();
	const float LABEL_HEIGHT = 90.0;
	const float LABEL_WIDTH = 585.0;

	UIComponentFactory* factory = UIComponentFactory::getInstance();

	mHeader = factory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_CENTER, "Authenticating...");
	root->addChild(mHeader);
}

void UIStateAuthAuthenticating::SubmitAuthenticationRequest() {
	Authenticator::AuthenticationResultListener callback;
	callback.bind(this, &UIStateAuthAuthenticating::OnAuthenticationResult);

	m_authenticator.SetRestConnector(IEngine::getEngine()->GetRestConnector());
    m_authenticator.SetUser(IEngine::getEngine()->GetUserProvider()->GetUser());
	m_authenticator.RegisterAuthenticationResultListener(callback);
	m_authenticator.SendAuthenticationRequest();
}

void UIStateAuthAuthenticating::OnAuthenticationResult(Authenticator::AuthenticationResult result) {
	STRING_ID levelID = StringManager::UNDEFINED;

	switch (result) {
        case Authenticator::AuthenticationResult::SUCCESS:
            IEngine::getEngine()->GetUserProvider()->SetAuthenticated(true);
            IEngine::getEngine()->GetUserMemory()->StoreUsername(IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername());
            IEngine::getEngine()->GetUserMemory()->StorePassword(IEngine::getEngine()->GetUserProvider()->GetUser().GetPasswordHash());

            levelID = StringManager::getIDForString("MainMenuLevel");
            ChangeState(UIStateMainMenu::UI_STATE_MAINMENU);
            break;
        default:
            break;
	}
}
