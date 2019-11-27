#pragma once

#include <Core/Keyboard/IKeyboardActivator.h>
#include <GLView.h>

class IOSKeyboardActivator : public IKeyboardActivator {
public:
    void SetView(UIView* view) {
        m_view = view;
    }
    
    virtual void ActivateKeyboard() {
        [m_view becomeFirstResponder];
    }
    
    virtual void DeactivateKeyboard() {
        [m_view resignFirstResponder];
    }

private:
    UIView* m_view;
};
