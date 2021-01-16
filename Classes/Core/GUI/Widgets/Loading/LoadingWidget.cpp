#include <Core/GUI/Widgets/Loading/LoadingWidget.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

LoadingWidget::LoadingWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
  m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_loading(nullptr) {}

void LoadingWidget::Init() {
    AddLoadingMessage();
}

void LoadingWidget::Release() {
    m_loading->setTextString("");
    m_loading->release();
    delete m_loading;
}


void LoadingWidget::AddLoadingMessage() {
    m_loading = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_CENTER, "loading...");
    m_loading->setDropShadowColor(dropShadowColor);
    m_loading->setY(12.0);

    m_parentComponent->addChild(m_loading);
}
