#include <Core/GUI/Widgets/Popup/PopupWidget.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

PopupWidget::PopupWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
  m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_window(nullptr), m_frame(nullptr), m_title(nullptr), m_closeButton(nullptr) {}

void PopupWidget::Init(IPopupCloser* popupCloser, const std::string& title, const std::list<std::string> lines) {
    m_popupCloser = popupCloser;
    m_titleString = title;
    m_lines = lines;
    
    AddWindow();
    AddFrame();
    AddTitle();
    AddCloseButton();
    AddLoadingMessage();
    SetVisible(false);
}

void PopupWidget::Release() {
    m_window->release();
    delete m_window;
}

void PopupWidget::AddWindow() {
    m_window = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_window->setWidth(m_parentComponent->getWidth());
    m_window->setHeight(m_parentComponent->getHeight());
    m_parentComponent->addChild(m_window);
}

void PopupWidget::AddFrame() {
    m_frame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_frame->setAnchor(UIComponent::ANCHOR_TOP_CENTER);
    m_frame->setWidth(1265);
    m_frame->setHeight(600);
    m_frame->setY(50);

    m_window->addChild(m_frame);
}

void PopupWidget::AddTitle() {
    m_title = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "");
    m_title->setDropShadowColor(dropShadowColor);
    m_title->setY(12.0);

    m_frame->addChild(m_title);
}

void PopupWidget::AddCloseButton() {
    m_closeButton = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_BOTTOM_CENTER, "Dismiss");
    m_closeButton->setDropShadowColor(dropShadowColor);
    m_closeButton->setY(80);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &PopupWidget::OnClosePopup);
    m_closeButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_frame->addChild(m_closeButton);
}

void PopupWidget::OnClosePopup(UITouchButton::ButtonState state) {
    m_popupCloser->ClosePopup();
}

void PopupWidget::SetVisible(bool visible) {
    m_window->setVisible(visible);
    m_frame->setVisible(visible);
    
    if (visible) {
        m_title->setTextString(m_titleString);
    } else {
        m_title->setTextString("");
    }
}

void PopupWidget::AddLoadingMessage() {
    //m_loading = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_CENTER, "");
    //m_loading->setDropShadowColor(dropShadowColor);
    //m_loading->setY(12.0);

    //m_parentComponent->addChild(m_loading);
}
