#include <Core/GUI/Widgets/Loading/LoadingWidget.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

LoadingWidget::LoadingWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
  m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_loading(nullptr), m_loadingWindow(nullptr), m_loadingFrame(nullptr), m_title(nullptr) {}

void LoadingWidget::Init() {
    AddProfileWindow();
    AddProfileFrame();
    AddTitle();
    AddLoadingMessage();
}

void LoadingWidget::Release() {
    m_loading->setTextString("");
    m_loading->release();
    delete m_loading;
    
    m_loadingWindow->release();
    delete m_loadingWindow;
}

void LoadingWidget::AddProfileWindow() {
    m_loadingWindow = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_loadingWindow->setWidth(m_parentComponent->getWidth());
    m_loadingWindow->setHeight(m_parentComponent->getHeight());
    m_parentComponent->addChild(m_loadingWindow);
}

void LoadingWidget::AddProfileFrame() {
    m_loadingFrame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_loadingFrame->setAnchor(UIComponent::ANCHOR_TOP_CENTER);
    m_loadingFrame->setWidth(1265);
    m_loadingFrame->setHeight(600);
    m_loadingFrame->setY(50);

    m_loadingWindow->addChild(m_loadingFrame);
}

void LoadingWidget::AddTitle() {
    m_title = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "");
    m_title->setDropShadowColor(dropShadowColor);
    m_title->setY(12.0);

    m_loadingFrame->addChild(m_title);
}

void LoadingWidget::SetVisible(bool visible) {
    m_loadingWindow->setVisible(visible);
    m_loadingFrame->setVisible(visible);
    
    if (visible) {
        m_loading->setTextString("loading...");
        m_title->setTextString("Cosmic Chat");
    } else {
        m_loading->setTextString("");
        m_title->setTextString("");
    }
}

void LoadingWidget::AddLoadingMessage() {
    m_loading = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_CENTER, "");
    m_loading->setDropShadowColor(dropShadowColor);
    m_loading->setY(12.0);

    m_parentComponent->addChild(m_loading);
}
