#pragma once

class IKeyboardListener {
public:
    virtual void OnDeletePressed() = 0;
    virtual void OnCharacterPressed(char c) = 0;
	virtual void OnEnterPressed() = 0;
};
