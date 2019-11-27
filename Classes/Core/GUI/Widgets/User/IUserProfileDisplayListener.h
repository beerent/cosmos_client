#pragma once

#include <Core/User/User.h>
#include <Core/GUI/Widgets/User/UserProfileWidget.h>
#include <Core/GameLogic/User/UserProfileLoader.h>
#include <IEngine.h>

class IUserProfileDisplayListener : public IProfileCloser, IUserProfileReceiver {
public:
	IUserProfileDisplayListener(UIComponentFactory* uiComponentFactory, UIComponent* parentComponent) {
		m_userProfileWidget = new UserProfileWidget(uiComponentFactory, parentComponent);
	}

	void DisplayUserProfile(const User& user) {
        m_userProfileLoader = new UserProfileLoader(user);
        m_userProfileLoader->RegisterProfileLoadedListener(this);
        m_userProfileLoader->SetRestConnector(IEngine::getEngine()->GetRestConnector());
        
		m_userProfileWidget->Init(user, this);
        m_userProfileWidget->DisplayLoading();
        m_userProfileLoader->LoadUserProfile();

		UIComponent* component = m_userProfileWidget->GetWindow();
		IEngine::getEngine()->DisplayUIBlockingComponent(component);
	}

	virtual void CloseUserProfile() {
		IEngine::getEngine()->CompleteUIBlockingComponent();
	}
    
    virtual void OnProfileLoaded(const UserProfile& userProfile) {
        m_userProfileWidget->TakeDownLoading();
        m_userProfileWidget->DisplayUserProfile(userProfile);
    }

private:
    UserProfileLoader* m_userProfileLoader;
	UserProfileWidget* m_userProfileWidget;
};
