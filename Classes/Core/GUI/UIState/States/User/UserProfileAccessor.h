#pragma once

#include <Core/GUI/Components/UITouchButton.h>
#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"

namespace {
	void DisplayUserProfileFrame(UIComponentFactory* uiComponentFactory, UIComponent* parentComponent) {
		auto frame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
		frame->setAnchor(UIComponent::ANCHOR_CENTER);
		frame->setWidth(600);
		frame->setHeight(460);

		parentComponent->addChild(m_leaderboard);
	}
}

namespace UserProfileAccessor {

	void DisplayUserProfile(UITouchButton::ButtonState state) {
		UIComponentFactory* uiComponentFactory = UIComponentFactory::getInstanjce();
		UIComponent* parentComponent = IEngine::getEngine()->getUIRoot();

		DisplayUserProfileFrame();
	}


}