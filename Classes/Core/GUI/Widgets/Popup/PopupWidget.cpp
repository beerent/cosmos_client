#include <Core/GUI/Widgets/Popup/PopupWidget.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

const glm::vec3 YELLOW_TEXT_COLOR(255.0f, 255.0f, 0.0f);
const glm::vec3 RED_TEXT_COLOR(255.0f, 0.0f, 0.0f);
const glm::vec3 PURPLE_TEXT_COLOR(255.0f, 0.0f, 255.0f);
const glm::vec3 BLUE_TEXT_COLOR(0.0f, 0.0f, 255.0f);
const glm::vec3 WHITE_TEXT_COLOR(255.0f, 255.0f, 255.0f);
const glm::vec3 GREEN_TEXT_COLOR(0.0f, 255.0f, 0.0f);

PopupWidget::PopupWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) :
  m_uiComponentFactory(uiComponentFactory), m_parentComponent(parentComponent), m_window(nullptr), m_frame(nullptr), m_title(nullptr), m_closeButton(nullptr), m_line0(nullptr), m_line1(nullptr), m_line2(nullptr), m_line3(nullptr), m_line4(nullptr) , m_line5(nullptr), m_line6(nullptr), m_line7(nullptr), m_line8(nullptr), m_line9(nullptr) {}

void PopupWidget::Init(IPopupCloser* popupCloser, const std::string& key, const std::string& title, const std::vector<std::string> lines) {
    m_popupCloser = popupCloser;
    
    m_key = key;
    m_titleString = title;
    m_lines = lines;
    
    for (int i = 0; i < 10 - lines.size(); i++) {
        if (i % 2 == 0) {
            m_lines.insert(m_lines.begin(), "");
        } else {
            m_lines.push_back("");
        }
    }
    
    AddWindow();
    AddFrame();
    AddTitle();
    AddCloseButton();
    AddLines();
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
    m_title->setDropShadowColor(WHITE_TEXT_COLOR);
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
    m_popupCloser->ClosePopup(m_key);
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

void PopupWidget::AddLines() {
    const std::string text = "";
    float textWidth = 12.5 * text.size();
    
    const int basePadding = 29;
    const int rowPadding = 32;
    
    
    std::string line0 = m_lines[0];
    glm::vec3 textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line0)) {
        textColor = GetColor(line0);
        line0 = RemoveColor(line0);
    }
    m_line0 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line0);
    m_line0->setColor(textColor); m_line0->setDropShadowColor(dropShadowColor); m_line0->setY(basePadding + ( rowPadding * (1 + 0))); m_frame->addChild(m_line0);

    
    std::string line1 = m_lines[1];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line1)) {
        textColor = GetColor(line1);
        line1 = RemoveColor(line1);
    }
    m_line1 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line1);
    m_line1->setColor(textColor); m_line1->setDropShadowColor(dropShadowColor); m_line1->setY(basePadding + ( rowPadding * (1 + 1))); m_frame->addChild(m_line1);
    
    std::string line2 = m_lines[2];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line2)) {
        textColor = GetColor(line2);
        line2 = RemoveColor(line2);
    }
    m_line2 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line2);
    m_line2->setColor(textColor); m_line2->setDropShadowColor(dropShadowColor); m_line2->setY(basePadding + ( rowPadding * (1 + 2))); m_frame->addChild(m_line2);
    
    std::string line3 = m_lines[3];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line3)) {
        textColor = GetColor(line3);
        line3 = RemoveColor(line3);
    }
    m_line3 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line3);
    m_line3->setColor(textColor); m_line3->setDropShadowColor(dropShadowColor); m_line3->setY(basePadding + ( rowPadding * (1 + 3))); m_frame->addChild(m_line3);
    
    std::string line4 = m_lines[4];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line4)) {
        textColor = GetColor(line4);
        line4 = RemoveColor(line4);
    }
    m_line4 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line4);
    m_line4 ->setColor(textColor); m_line4->setDropShadowColor(dropShadowColor); m_line4->setY(basePadding + ( rowPadding * (1 + 4))); m_frame->addChild(m_line4);
    
    std::string line5 = m_lines[5];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line5)) {
        textColor = GetColor(line5);
        line5 = RemoveColor(line5);
    }
    m_line5 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line5);
    m_line5->setColor(textColor); m_line5->setDropShadowColor(dropShadowColor); m_line5->setY(basePadding + ( rowPadding * (1 + 5))); m_frame->addChild(m_line5);
    
    std::string line6 = m_lines[6];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line6)) {
        textColor = GetColor(line6);
        line6 = RemoveColor(line6);
    }
    m_line6 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line6);
    m_line6->setColor(textColor); m_line6->setDropShadowColor(dropShadowColor); m_line6->setY(basePadding + ( rowPadding * (1 + 6))); m_frame->addChild(m_line6);
    
    std::string line7 = m_lines[7];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line7)) {
        textColor = GetColor(line7);
        line7 = RemoveColor(line7);
    }
    m_line7 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line7);
    m_line7->setColor(textColor); m_line7->setDropShadowColor(dropShadowColor); m_line7->setY(basePadding + ( rowPadding * (1 + 7))); m_frame->addChild(m_line7);
    
    std::string line8 = m_lines[8];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line8)) {
        textColor = GetColor(line8);
        line8 = RemoveColor(line8);
    }
    m_line8 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line8);
    m_line8->setColor(textColor); m_line8->setDropShadowColor(dropShadowColor); m_line8->setY(basePadding + ( rowPadding * (1 + 8))); m_frame->addChild(m_line8);
    
    std::string line9 = m_lines[9];
    textColor = WHITE_TEXT_COLOR;
    if (ContainsColor(line9)) {
        textColor = GetColor(line9);
        line9 = RemoveColor(line9);
    }
    m_line9 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, line9);
    m_line9->setColor(textColor); m_line9->setDropShadowColor(dropShadowColor); m_line9->setY(basePadding + ( rowPadding * (1 + 9))); m_frame->addChild(m_line9);
}

bool PopupWidget::ContainsColor(const std::string& text) const {
    if (text.length() < 3) {
        return false;
    }
    
    std::string key = text.substr(0, 3);
    return key == "!r!" || key == "!y!" || key == "!p!" || key == "!b!" || key == "!g!";
}

glm::vec3 PopupWidget::GetColor(const std::string& text) const {
    if (text.length() < 3) {
        return WHITE_TEXT_COLOR;
    }
    
    std::string key = text.substr(0, 3);
    if (key == "!r!")
        return RED_TEXT_COLOR;
    if (key == "!y!")
        return YELLOW_TEXT_COLOR;
    if (key == "!p!")
        return PURPLE_TEXT_COLOR;
    if (key == "!b!")
        return BLUE_TEXT_COLOR;
    if (key == "!g!")
        return GREEN_TEXT_COLOR;
    return WHITE_TEXT_COLOR;
}

std::string PopupWidget::RemoveColor(const std::string& text) const {
    return text.substr(3, text.length());
}
