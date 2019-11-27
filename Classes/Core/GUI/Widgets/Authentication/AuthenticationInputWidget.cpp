#include <Core/GUI/Widgets/Authentication/AuthenticationInputWidget.h>

const float LABEL_HEIGHT = 90.0;
const float LABEL_SPACING = 0.0 + LABEL_HEIGHT;
//const float LABEL_FONT_SIXE = 35.0;
const float LABEL_WIDTH = 585.0;

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

void AuthenticationInputWidget::init(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) {
	m_uiComponentFactory = uiComponentFactory;
	m_parentComponent = parentComponent;
	m_failureCount = 0;
}

void AuthenticationInputWidget::Release() {
	ClearLoginFields();
	ClearAuthenticatingFields();

	delete m_username;
	delete m_password;
	delete m_submitButton;
	delete m_authenticating;

	if (HasFailureOccured()) {
		delete m_failure;
	}
}

void AuthenticationInputWidget::ShowLoginFields() {
	AddUsername();
	AddPassword();
	AddSubmitButton();
}

void AuthenticationInputWidget::ClearLoginFields() {
	m_username->release();
	m_password->release();
	m_submitButton->release();
}

void AuthenticationInputWidget::ShowAuthenticatingFields() {
	AddAuthenticating();
}

void AuthenticationInputWidget::ClearAuthenticatingFields() {
	m_authenticating->release();
}

void AuthenticationInputWidget::ShowFailureFields() {
	AddFailure();
}

void AuthenticationInputWidget::ClearFailureFields() {
	if (HasFailureOccured()) {
		m_failure->release();
	}
}

void AuthenticationInputWidget::RegisterForMenuItemSelectedEvent(onButtonStateChangedCallBack callBack) {
	m_buttonPressListeners.push_back(callBack);
}

void AuthenticationInputWidget::UnregisterForMenuItemSelectedEvent(onButtonStateChangedCallBack callBack) {
	m_buttonPressListeners.remove(callBack);
}

void AuthenticationInputWidget::AddFailure() {
	m_failure = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "login failed, try again.");
    m_failure->setDropShadowColor(dropShadowColor);
	m_parentComponent->addChild(m_failure);
}

void AuthenticationInputWidget::AddUsername() {
	m_username = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "enter username...");
    m_username->setDropShadowColor(dropShadowColor);
	m_username->setY(30);
	m_parentComponent->addChild(m_username);
}

void AuthenticationInputWidget::AddPassword() {
	m_password = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "enter password...");
    m_password->setDropShadowColor(dropShadowColor);
	m_password->setY(60);
	m_parentComponent->addChild(m_password);
}

void AuthenticationInputWidget::AddSubmitButton() {
	UITouchButton::onButtonStateChangedCallBack submitOnClick;

	m_submitButton = UIComponentFactory::getInstance()->createUILabel("KYCQuestionButtonArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "Login");
    m_submitButton->setDropShadowColor(dropShadowColor);
	m_submitButton->setY(120);
	submitOnClick.bind(this, &AuthenticationInputWidget::OnSubmitCredentials);
	m_submitButton->registerForButtonEvent(UITouchButton::DEPRESSED, submitOnClick);
	m_parentComponent->addChild(m_submitButton);
}

void AuthenticationInputWidget::AddAuthenticating() {
	m_authenticating = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "authenticating...");
    m_authenticating->setDropShadowColor(dropShadowColor);
	m_authenticating->setY(30);
	m_parentComponent->addChild(m_authenticating);
}

void AuthenticationInputWidget::UpdateUsername(const std::string& username) {
	m_username->release();
	m_username = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, username);
    m_username->setDropShadowColor(dropShadowColor);
	m_username->setY(30);
	m_parentComponent->addChild(m_username);
}

void AuthenticationInputWidget::UpdatePassword(const std::string& password) {
	m_password->release();
	m_password = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, password);
    m_password->setDropShadowColor(dropShadowColor);
	m_password->setY(60);
	m_parentComponent->addChild(m_password);
}

void AuthenticationInputWidget::IncrementFailureCount() {
	m_failureCount++;
}
bool AuthenticationInputWidget::HasFailureOccured() const {
	return m_failureCount > 0;
}

void AuthenticationInputWidget::OnSubmitCredentials(UITouchButton::ButtonState state)  {
    std::list<onButtonStateChangedCallBack>::iterator it = m_buttonPressListeners.begin();
	while (it != m_buttonPressListeners.end()) {
		(*it)(SUBMIT_AUTH_REQUEST);
		it++;
	}
}
