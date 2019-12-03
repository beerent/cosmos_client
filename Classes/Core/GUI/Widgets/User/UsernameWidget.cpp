#include <Core/GUI/Widgets/User/UsernameWidget.h>
#include <Core/User/UserProvider.h>
#include <Core/User/User.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

UsernameWidget::UsernameWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) : IUserProfileDisplayListener(uiComponentFactory, parentComponent), m_username(nullptr) {
	m_uiComponentFactory = uiComponentFactory;
	m_parentComponent = parentComponent;
}

void UsernameWidget::Init() {
	AddUsername();
}

void UsernameWidget::Release() {
	m_username->release();
	delete m_username;
}

void UsernameWidget::AddUsername() {
	UserProvider* userProvider = IEngine::getEngine()->GetUserProvider();
	std::string username = userProvider->GetUser().GetUsername();
    if (userProvider->GetUser().GetAccessLevel() == UserAccessLevel::GUEST) {
        username += " (guest)";
    }

	float width = 12.5 * username.size();

	m_username = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", width, 40, UIComponent::ANCHOR_TOP_RIGHT, username);
	m_username->setDropShadowColor(dropShadowColor);
	m_username->setX(50);

	m_usernameLabel = new UIUsernameLabel(userProvider->GetUser(), m_username);
	m_usernameLabel->RegisterUserProfileDisplayListener(this);

	m_parentComponent->addChild(m_username);
}
