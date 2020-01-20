#include "Core/GUI/Widgets/MainMenuWidget.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/Debug/DebugLogic.h"

#include <Core/User/UserProvider.h>

float labelYPosition = 100.0;
float labelXPosition = 5.0f;
const float LABEL_HEIGHT = 90.0;
const float LABEL_SPACING = 0.0 + LABEL_HEIGHT;
const float LABEL_WIDTH = 585.0;

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

const std::string USERNAME = "username: ";

MainMenuWidget::MainMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) : m_menu(nullptr), m_usernameInputBox(nullptr), m_loginButton(nullptr), m_usernameRefreshButton(nullptr), m_usernameRefreshListener(nullptr), m_username(nullptr)/*, m_registerButton(nullptr)*/ {
	m_uiComponentFactory = uiComponentFactory;
	m_parentComponent = parentComponent;
   
}
void MainMenuWidget::init() {
	m_menu = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_menu->setAnchor(UIComponent::ANCHOR_CENTER);
    m_menu->setWidth(600);
    m_menu->setHeight(460);
    
	m_parentComponent->addChild(m_menu);

	UITouchButton::onButtonStateChangedCallBack callBack;

	UILabel* label = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "Know Your Cosmos");
    label->setDropShadowColor(dropShadowColor);
    label->setY(12.0);   
    m_menu->addChild(label);
    

    /* Blank Label */
    label = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "");
    label->setDropShadowColor(dropShadowColor);
    label->setX(labelXPosition);
    label->setY(labelYPosition + LABEL_SPACING );
    m_menu->addChild(label);
    
    label = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_LEFT, "Challenge Mode");
    label->setDropShadowColor(dropShadowColor);
    label->setX(labelXPosition);
    label->setY(labelYPosition + LABEL_SPACING * 2);
	callBack.bind(this, &MainMenuWidget::OnLoadChallengeMenu);
	label->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    m_menu->addChild(label);
    
    AddUsernameRefreshButton();
    
    /* REMOVED LOGIN FEATURE FOR INITIAL RELEASE */
    //UserProvider* userProvider = IEngine::getEngine()->GetUserProvider();
    //if (!userProvider->IsLoggedIn()) {
    //    SetGuestUsernameDisplay("");
    //    AddLoginButton(m_parentComponent);
    //}
}

void MainMenuWidget::AddLoginButton(UIComponent *parentComponent) {
	UITouchButton::onButtonStateChangedCallBack callBack;

	m_loginButton = UIComponentFactory::getInstance()->createUILabel("KYCQuestionButtonArchetype", LABEL_WIDTH / 2 - 10, LABEL_HEIGHT, UIComponent::ANCHOR_BOTTOM_CENTER, "Login");
    m_loginButton->setDropShadowColor(dropShadowColor);
	callBack.bind(this, &MainMenuWidget::onLogin);
	m_loginButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

	parentComponent->addChild(m_loginButton);
}

void MainMenuWidget::AddUsernameRefreshButton() {
    UITouchButton::onButtonStateChangedCallBack callBack;
    
    m_usernameRefreshButton = UIComponentFactory::getInstance()->createUILabel("KYCHeaderLabelArchetype", 80, 60, UIComponent::ANCHOR_CENTER, "<*>");
    m_usernameRefreshButton->setDropShadowColor(dropShadowColor);
    callBack.bind(this, &MainMenuWidget::OnRefreshUsername);
    m_usernameRefreshButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

    m_usernameRefreshButton->setY(-50);
    m_usernameRefreshButton->setX(-240);
    
    m_parentComponent->addChild(m_usernameRefreshButton);
    
    SetUsernameText();
}

void MainMenuWidget::OnRefreshUsername(UITouchButton::ButtonState state) {
    if (nullptr != m_usernameRefreshListener) {
        m_usernameRefreshListener->OnUsernameRefresh();
    }
}
    

/*void MainMenuWidget::AddRegisterButton(UIComponent *parentComponent) {
	//UITouchButton::onButtonStateChangedCallBack callBack;

	m_registerButton = UIComponentFactory::getInstance()->createUILabel("KYCQuestionButtonArchetype", LABEL_WIDTH / 2 - 10, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_LEFT, "Register");
	m_registerButton->setDropShadowColor(dropShadowColor);
	m_registerButton->setX(740);
	m_registerButton->setY(650);
	//parentComponent->addChild(m_registerButton);
	//callBack.bind(this, &MainMenuWidget::onLogin);
	//m_registerButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
}*/

void MainMenuWidget::OnLoadChallengeMenu(UITouchButton::ButtonState state) {
    std::list<onMenuItemSelectedCallBack>::iterator it = m_onMenuItemSelectedListeners.begin();
    while(it != m_onMenuItemSelectedListeners.end()) {
        (*it)(LOAD_CHALLENGE_MENU_LEVEL);
        it++;
    }
}

void MainMenuWidget::onLogin(UITouchButton::ButtonState state) {
	std::list<onMenuItemSelectedCallBack>::iterator it = m_onMenuItemSelectedListeners.begin();
	while (it != m_onMenuItemSelectedListeners.end()) {
		(*it)(LOAD_LOGIN_LEVEL);
		it++;
	}
}

void MainMenuWidget::registerForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack) {
    m_onMenuItemSelectedListeners.push_back(callBack);
}

void MainMenuWidget::unregisterForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack) {
    m_onMenuItemSelectedListeners.remove(callBack);
}

void MainMenuWidget::release() {
    m_menu->release();
    delete(m_menu);


 if (m_loginButton != nullptr) {
		m_loginButton->release();
		delete(m_loginButton);
	}
/*
	if (m_registerButton != nullptr) {
		m_registerButton->release();
		delete(m_registerButton);
	}
 */
    
    m_usernameRefreshButton->release();
    delete m_usernameRefreshButton;
}

void MainMenuWidget::SetUsernameText() {
    m_username = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 1, 10, UIComponent::ANCHOR_CENTER, USERNAME);
    m_username->setDropShadowColor(dropShadowColor);
    m_username->setX(-100);
    m_username->setY(-50);
    m_menu->addChild(m_username);
}

void MainMenuWidget::SetGuestUsernameDisplay(const std::string& displayUsername) {
    if (m_usernameInputBox != nullptr) {
        m_usernameInputBox->release();
    }
    
    std::string s;
    for (int i = 0; i < displayUsername.size(); i++) {
        s += " ";
    }
    
    s += displayUsername;
        
    /* Non Logged In Option */
    m_usernameInputBox = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 450, 80, UIComponent::ANCHOR_CENTER, s);
    m_usernameInputBox->setDropShadowColor(dropShadowColor);
    m_usernameInputBox->setY(-50);
    m_menu->addChild(m_usernameInputBox);
}

void MainMenuWidget::RegisterUsernameFocusCallback(UILabel::onButtonStateChangedCallBack callback) {
    m_usernameInputBox->registerForButtonEvent(UITouchButton::DEPRESSED, callback);
}
