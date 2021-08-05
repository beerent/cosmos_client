#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>

class IPopupCloser {
public:
    virtual void ClosePopup(const std::string& key) = 0;
};

class PopupWidget {
public:
    PopupWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

    void Init(IPopupCloser* popupCloser, const std::string& key, const std::string& title, const std::vector<std::string> lines);
    void Release();
    void SetVisible(bool visible);
    
private:
    void AddLines();
    void AddWindow();
    void AddFrame();
    void AddTitle();
    void AddCloseButton();
    void OnClosePopup(UITouchButton::ButtonState state);
    
    bool ContainsColor(const std::string& text) const;
    glm::vec3 GetColor(const std::string& text) const;
    std::string RemoveColor(const std::string& text) const;
    
    IPopupCloser* m_popupCloser;
    
    UIComponentFactory* m_uiComponentFactory;
    UIComponent* m_parentComponent;
    UIComponent* m_window;
    UIComponent* m_frame;
    
    std::string m_key;
    std::string m_titleString;
    std::vector<std::string> m_lines;

    UILabel* m_title;

    UILabel* m_line0;
    UILabel* m_line1;
    UILabel* m_line2;
    UILabel* m_line3;
    UILabel* m_line4;
    UILabel* m_line5;
    UILabel* m_line6;
    UILabel* m_line7;
    UILabel* m_line8;
    UILabel* m_line9;
    
    UILabel* m_closeButton;
};
