#pragma once

#include "Core/OpenSource/FastDelegate.h"
#include "Core/GUI/Components/UITouchButton.h"
#include "Core/GUI/Components/UILabel.h"
#include <Core/GUI/Components/UIComponentFactory.h>
#include <Core/Util/SimpleTimer.h>
#include <Core/Keyboard/IKeyboardListener.h>
#include <Core/GUI/Widgets/User/Username/UsernameEditWidget.h>
#include <list>
#include <string>

class IEditUsernameCloser {
public:
	virtual void CloseEditUsername() = 0;
};

class UsernameEditWidget : public IKeyboardListener, Timer::SimpleTimerListener {

public:
    UsernameEditWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

	void Init();
	void Release();
    
    virtual void OnDeletePressed();
    virtual void OnCharacterPressed(char c);
    virtual void OnEnterPressed();

    virtual void OnTimerEvent(Timer::TimerType type);
    
    void RegisterEditUsernameCloser(IEditUsernameCloser* editUsernameCloser);

private:
	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

    IEditUsernameCloser* m_editUsernameCloser;

	UIComponent* m_profileWindow;
	UIComponent* m_profileFrame;
	UILabel* m_usernameSelectionLabel;
    UILabel* m_usernameLabel;
    UILabel* m_rememberUsernameLabel;
    UILabel* m_rememberUsernameCheckLabel;
    UITouchButton* m_editButton;
    UITouchButton* m_randomButton;
    UITouchButton* m_closeButton;
    
    UITouchButton::onButtonStateChangedCallBack m_closeButtonCallback;
    
    std::string m_username;
    bool m_cursorOn;
    bool m_editingUsername;
    
    Timer::SimpleTimer m_timer;
    KeyboardManager* m_keyboardManager;
    UsernameEditWidget* m_usernameEditWidget;

	void AddProfileWindow();
	void AddProfileFrame();
	void AddUsernameSelectionLabel();
    void AddUsernameLabel();
    void AddRememberUsernameLabel();
    void AddRememberUsernameCheckLabel();
    void AddEditButton();
    void AddRandomButton();
	void AddCloseButton();
    
    
    bool IsEditingUsername();
    void DisplayCursor();
    void HideCursor();
    
    void OnRememberUsernamePressed(UITouchButton::ButtonState state);
    void OnEditPressed(UITouchButton::ButtonState state);
    void OnRandomPressed(UITouchButton::ButtonState state);
	void OnExitPressed(UITouchButton::ButtonState state);
};
