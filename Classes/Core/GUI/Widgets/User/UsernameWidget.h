#pragma once

//#include <Core/OpenSource/FastDelegate.h>
#include <list>

#include <Core/GUI/Components/UITouchButton.h>
#include <Core/GUI/Components/UILabel.h>
#include <Core/User/UIUsernameLabel.h>
#include <Core/GUI/Widgets/User/IUserProfileDisplayListener.h>
#include <Core/User/User.h>

class UIComponentFactory;
class UIComponent;

class UsernameWidget : IUserProfileDisplayListener {

public:
	UsernameWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
	void Init();
	void Release();

private:
	void AddUsername();

	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UILabel* m_username;
	UIUsernameLabel* m_usernameLabel;
};