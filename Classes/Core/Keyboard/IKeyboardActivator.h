#pragma once

class IKeyboardActivator {
public:
    virtual void ActivateKeyboard() = 0;
    virtual void DeactivateKeyboard() = 0;
};
