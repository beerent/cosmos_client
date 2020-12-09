#include <Core/GUI/Widgets/User/Username/UsernameEditWidget.h>
#include <Core/GUI/Components/UIComponentFactory.h>
#include <Core/Util/UsernameGenerator.h>
#include <Core/General/DeviceMemoryInterface.h>
#include <IEngine.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const glm::vec3 GREEN_TEXT_COLOR(0.0f / 255.0f, 255.0f / 255.0f, 128.0f / 255.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

UsernameEditWidget::UsernameEditWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
  m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_profileWindow(nullptr), m_profileFrame(nullptr), m_usernameSelectionLabel(nullptr), m_usernameLabel(nullptr), m_rememberUsernameLabel(nullptr), m_rememberUsernameCheckLabel(nullptr), m_closeButton(nullptr), m_editButton(nullptr), m_randomButton(nullptr), m_editUsernameCloser(nullptr), m_cursorOn(false), m_editingUsername(false), m_timer(this) {}

void UsernameEditWidget::Init() {
    m_keyboardManager = IEngine::getEngine()->GetKeyboardManager();
    m_keyboardManager->RegisterKeyboardListener(this);
    
    m_closeKeyboardCallback.bind(this, &UsernameEditWidget::OnBackgroundPressed);
    InputManager::getInstance()->registerForInputEvents(m_closeKeyboardCallback);
    
    m_username = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    
	AddProfileWindow();
    AddProfileFrame();
    AddUsernameSelectionLabel();
    AddUsernameLabel();
    AddRememberUsernameLabel();
    AddRememberUsernameCheckLabel();
    AddEditButton();
    AddRandomButton();
    AddCloseButton();
}

void UsernameEditWidget::AddProfileWindow() {
	m_profileWindow = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
	m_profileWindow->setWidth(m_parentComponent->getWidth());
	m_profileWindow->setHeight(m_parentComponent->getHeight());
    m_parentComponent->addChild(m_profileWindow);
}

void UsernameEditWidget::AddProfileFrame() {
	m_profileFrame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
	m_profileFrame->setAnchor(UIComponent::ANCHOR_CENTER);
	m_profileFrame->setWidth(900);
	m_profileFrame->setHeight(460);

	m_profileWindow->addChild(m_profileFrame);
}

void UsernameEditWidget::AddUsernameSelectionLabel() {
    m_usernameSelectionLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "Username Selection");
    m_usernameSelectionLabel->setDropShadowColor(dropShadowColor);
    m_usernameSelectionLabel->setY(6);

	m_profileFrame->addChild(m_usernameSelectionLabel);
}

void UsernameEditWidget::AddUsernameLabel() {
    m_usernameLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 1, 1, UIComponent::ANCHOR_CENTER, m_username);
    m_usernameLabel->setY(-80);
    m_usernameLabel->setDropShadowColor(dropShadowColor);

    m_profileFrame->addChild(m_usernameLabel);
}

void UsernameEditWidget::AddRememberUsernameLabel() {
    std::string rememberUsernameString = "[ ] remember";
    m_rememberUsernameLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 250, 50, UIComponent::ANCHOR_CENTER, rememberUsernameString);
    m_rememberUsernameLabel->setY(-30);
    m_rememberUsernameLabel->setDropShadowColor(dropShadowColor);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &UsernameEditWidget::OnRememberUsernamePressed);
    m_rememberUsernameLabel->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

    m_profileFrame->addChild(m_rememberUsernameLabel);
}

void UsernameEditWidget::AddRememberUsernameCheckLabel() {
    std::string checkString = " ";
    if (DeviceMemoryInterface().ReadRememberUsername()) {
        checkString = "X";
    }

    m_rememberUsernameCheckLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 1, 1, UIComponent::ANCHOR_CENTER, checkString);
    m_rememberUsernameCheckLabel->setColor(GREEN_TEXT_COLOR);
    m_rememberUsernameCheckLabel->setY(-30);
    m_rememberUsernameCheckLabel->setX(-66);
    m_rememberUsernameCheckLabel->setDropShadowColor(dropShadowColor);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &UsernameEditWidget::OnRememberUsernamePressed);
    m_rememberUsernameCheckLabel->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

    m_profileFrame->addChild(m_rememberUsernameCheckLabel);
}

void UsernameEditWidget::AddEditButton() {
    m_editButton = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", 255.0, 90.0, UIComponent::ANCHOR_BOTTOM_LEFT, "Edit");
    m_editButton->setY(90);
    m_editButton->setX(25);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &UsernameEditWidget::OnEditPressed);
    m_editButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_profileFrame->addChild(m_editButton);
}

void UsernameEditWidget::AddRandomButton() {
    m_randomButton = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", 275.0, 90.0, UIComponent::ANCHOR_CENTER, "Random");
    m_randomButton->setY(90);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &UsernameEditWidget::OnRandomPressed);
    m_randomButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_profileFrame->addChild(m_randomButton);
}

void UsernameEditWidget::AddCloseButton() {
    m_closeButton = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", 255.0, 90.0, UIComponent::ANCHOR_BOTTOM_RIGHT, "Close");
    m_closeButton->setY(95);
    m_closeButton->setX(25);
    
    m_closeButtonCallback.bind(this, &UsernameEditWidget::OnExitPressed);
    m_closeButton->registerForButtonEvent(UITouchButton::DEPRESSED, m_closeButtonCallback);
	
    m_profileFrame->addChild(m_closeButton);
}

bool UsernameEditWidget::IsEditingUsername() {
    return m_editingUsername;
}

void UsernameEditWidget::DisplayCursor() {
    m_usernameLabel->setTextString(m_username + "|");
}

void UsernameEditWidget::HideCursor() {
    m_usernameLabel->setTextString(m_username);
}

void UsernameEditWidget::OnRememberUsernamePressed(UITouchButton::ButtonState state) {
    const bool remember = !DeviceMemoryInterface().ReadRememberUsername();
    DeviceMemoryInterface().StoreRememberUsername(remember);

    m_rememberUsernameCheckLabel->release();
    AddRememberUsernameCheckLabel();
}

void UsernameEditWidget::OnEditPressed(UITouchButton::ButtonState state) {
    m_keyboardManager->ActivateKeyboard();
    DisplayCursor();
    m_timer.RegisterTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
    m_editingUsername = true;
}

void UsernameEditWidget::OnRandomPressed(UITouchButton::ButtonState state) {
    m_username = UsernameGenerator().GetRandomUsername();
    
    if (IsEditingUsername()) {
        OnTimerEvent(Timer::TimerType::CURSOR_BLINK_500_MS);
    }
    else
    {
        HideCursor();
    }
}

void UsernameEditWidget::OnExitPressed(UITouchButton::ButtonState state) {
    const User currentUser = IEngine::getEngine()->GetUserProvider()->GetUser();
    const User newUser = User(m_username, currentUser.GetPasswordHash(), currentUser.GetAccessLevel());
    IEngine::getEngine()->GetUserProvider()->SetUser(newUser);
    
    m_editUsernameCloser->CloseEditUsername();
}

void UsernameEditWidget::OnBackgroundPressed(InputManager::InputEvent event, InputManager::InputEventData data) {
    if (IEngine::getEngine()->GetKeyboardManager()->KeyboardIsActive()) {
        IEngine::getEngine()->GetKeyboardManager()->OnEnterPressed();
    }
}

void UsernameEditWidget::RegisterEditUsernameCloser(IEditUsernameCloser* editUsernameCloser) {
    m_editUsernameCloser = editUsernameCloser;
}

void UsernameEditWidget::OnDeletePressed() {
    
    std::string currentUsername = m_username;
    if (currentUsername.empty() == false) {
        currentUsername.pop_back();
    }
    
    m_username = currentUsername;
    DisplayCursor();
    m_timer.ResetTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
}

void UsernameEditWidget::OnCharacterPressed(char c) {
    
    std::string currentUsername = m_username;
    
    if (currentUsername.size() < 20) {
        currentUsername += c;
        m_username = currentUsername;
    }
    
    DisplayCursor();
    m_timer.ResetTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
}

void UsernameEditWidget::OnEnterPressed() {
    m_editingUsername = false;
    HideCursor();
    m_keyboardManager->DeactivateKeyboard();
}

void UsernameEditWidget::OnTimerEvent(Timer::TimerType type) {
    switch(type) {
        case Timer::TimerType::CURSOR_BLINK_500_MS:
            if (IsEditingUsername() == false) {
                break;
            }
            
            if (m_cursorOn) {
                HideCursor();
                m_cursorOn = false;
            } else {
                m_cursorOn = true;
                DisplayCursor();
            }
            
            break;
        default:
            break;
    }
}

void UsernameEditWidget::Release() {
    m_closeButton->unregisterForButtonEvent(UITouchButton::DEPRESSED, m_closeButtonCallback);
    InputManager::getInstance()->unregisterForInputEvents(m_closeKeyboardCallback);
    m_keyboardManager->UnregisterKeyboardListener();
    
    if (m_profileWindow != nullptr) {
        m_profileWindow->release();
    }
}
