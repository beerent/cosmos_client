#pragma once

#include <Core/Keyboard/IKeyboardActivator.h>
#include <Core/Keyboard/IKeyboardListener.h>

class KeyboardManager {
public:
	KeyboardManager() : m_listenerSet(false), m_activatorSet(false), m_keyboardIsActive(false) {}

    void RegisterKeyboardListener(IKeyboardListener* keyboardListener) {
        m_keyboardListener = keyboardListener;
		m_listenerSet = true;
    }
    
    void UnregisterKeyboardListener() {
        m_keyboardListener = NULL;
		m_listenerSet = false;
    }
    
    void RegisterKeyboardActivator(IKeyboardActivator* keyboardActivator) {
        m_keyboardActivator = keyboardActivator;
		m_activatorSet = true;
    }
    
    void UnregisterKeyboardActivator(IKeyboardActivator* keyboardActivator) {
        m_keyboardActivator = NULL;
		m_activatorSet = false;
    }
    
    void OnCharacterPressed(char c) {
        if (m_listenerSet) {
            m_keyboardListener->OnCharacterPressed(c);
        }
    }
    
    void OnDeletePressed() {
        if (m_listenerSet) {
            m_keyboardListener->OnDeletePressed();
        }
    }

	void OnEnterPressed() {
		if (m_listenerSet) {
			m_keyboardListener->OnEnterPressed();
		}
	}
    
    void ActivateKeyboard() {
		if (m_activatorSet) {
			m_keyboardActivator->ActivateKeyboard();
            m_keyboardIsActive = true;
		}
    }
    
    void DeactivateKeyboard() {
		if (m_activatorSet) {
			m_keyboardActivator->DeactivateKeyboard();
            m_keyboardIsActive = false;
		}
    }
    
    bool KeyboardIsActive() const {
        return m_keyboardIsActive;
    }
    
private:
    IKeyboardListener* m_keyboardListener;
    IKeyboardActivator* m_keyboardActivator;

	bool m_listenerSet;
	bool m_activatorSet;
    bool m_keyboardIsActive;
};
