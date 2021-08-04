#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>

class IPopupCloser {
public:
    virtual void ClosePopup() = 0;
};

class PopupWidget {
public:
    PopupWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

    void Init(IPopupCloser* popupCloser,  const std::string& title, const std::list<std::string> lines);
    void Release();
    void SetVisible(bool visible);
    
private:
    void AddLoadingMessage();
    void AddWindow();
    void AddFrame();
    void AddTitle();
    void AddCloseButton();
    void OnClosePopup(UITouchButton::ButtonState state);
    
    IPopupCloser* m_popupCloser;
    
    UIComponentFactory* m_uiComponentFactory;
    UIComponent* m_parentComponent;
    UIComponent* m_window;
    UIComponent* m_frame;
    
    std::string m_titleString;
    std::list<std::string> m_lines;

    UILabel* m_title;
    UILabel* m_closeButton;
};
