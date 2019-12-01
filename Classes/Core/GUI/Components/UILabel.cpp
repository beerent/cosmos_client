//
//  UILabel.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 9/29/13.
//
//

#include "Core/GUI/Components/UILabel.h"
#include "Core/Rendering/RenderableManager.h"
#include "Core/Rendering/Renderables/UIFontRenderable.h"

const STRING_ID UILabel::CLASS_ID_UI_LABEL = StringManager::getIDForString("CLASS_ID_UI_LABEL");
const STRING_ID UILabel::UI_LABEL_FONT_VISUAL = StringManager::getIDForString("UI_LABEL_FONT_VISUAL");
const STRING_ID UILabel::STYLE = StringManager::getIDForString("STYLE");

UILabel::UILabel(PropertyCollection &propertyCollection):UITouchButton(propertyCollection, CLASS_ID_UI_LABEL)
,m_text(NULL)
,m_fontSize(50.0f)
,m_yOffset(0.0f)
,m_xOffset(0.0f)
,m_color(1.0f,1.0f,1.0f,1.0f)
,m_textLayout(ANCHOR_TOP_LEFT)
,m_dropShadow(false)
,m_dropShadowColor(0.0f, 0.0f, 0.0f, 0.5f)
{
    init(propertyCollection);
}

void UILabel::setTextOffset(float x, float y)
{
    m_yOffset = y;
    m_xOffset = x;
}

void UILabel::init(PropertyCollection &propertyCollection)
{
    int numberOfProperties = propertyCollection.getPropertyCount();
    for(int i=0; i < numberOfProperties; ++i)
    {
        const PropertyCollection::Property *property = propertyCollection.getPropertyAtIndex(i);
        if(property->m_name == UI_LABEL_FONT_VISUAL) {
            //add visual
            setFontVisual(property->m_data.StringIDValue);
        }
		else if (property->m_name == STYLE) {
			setStyle(StyleFactory::getInstance().GetStyle(property->m_data.StringIDValue));
		}
    }
}

void UILabel::setStyle(const Style& style) {
	setFontSize(style.getFontSize());

	glm::vec3 fontColor = style.getFontColor();
	setColor(fontColor[0], fontColor[1], fontColor[2]);

	if (style.getHasDropShadow()) {
		glm::vec4 dropColor = style.getDropShadowColor();
		setDropShadowColor(dropColor[0], dropColor[1], dropColor[2], dropColor[3]);
		setDropShadow(true);
	}

	setTextLayout(style.getTextLayout());

	glm::vec2 textOffsets = style.getTextOffsets();
	setTextOffset(textOffsets[0], textOffsets[1]);
}

void UILabel::setFontVisual(STRING_ID id)
{
    RenderableManager *renderableManager = RenderableManager::getInstance();
    m_fontRenderable = (UIFontRenderable*)renderableManager->getRenderable(id);
}

void UILabel::onBeginRender(Renderer &renderer)
{
    UITouchButton::onBeginRender(renderer);

    if(m_text != NULL)
    {
        float xOffset = 0;
        float yOffset = 0;
        float totalTextLength = m_fontSize * m_textWidth;
        
        switch(m_textLayout)
        {
            case ANCHOR_TOP_LEFT:
                xOffset = 0;
                yOffset = 0;
                break;
            case ANCHOR_TOP_CENTER:
                xOffset = m_width / 2.0 - totalTextLength / 2.0;
                yOffset = 0;
                break;
            case ANCHOR_TOP_RIGHT:
                xOffset = m_width - totalTextLength;
                yOffset = 0;
                break;
            case ANCHOR_LEFT:
                xOffset = 0;
                yOffset = m_height / 2.0 - m_fontSize / 2.0;
                break;
            case ANCHOR_CENTER:
                xOffset = m_width / 2.0 - totalTextLength / 2.0;
                yOffset = m_height / 2.0 - m_fontSize / 2.0;
                break;
            case ANCHOR_RIGHT:
                xOffset = m_width - m_textWidth;
                yOffset = m_height / 2.0 - m_fontSize / 2.0;
                break;
            case ANCHOR_BOTTOM_LEFT:
                xOffset = 0;
                yOffset = m_height - m_fontSize;
                break;
            case ANCHOR_BOTTOM_CENTER:
                xOffset = m_width / 2.0 - totalTextLength / 2.0;
                yOffset = m_height - m_fontSize;
                break;
            case ANCHOR_BOTTOM_RIGHT:
                xOffset = m_width - totalTextLength;
                yOffset = m_height - m_fontSize;
                break;
        }

        xOffset+= m_xOffset;
        yOffset+= m_yOffset;
        
        if(m_dropShadow)
        {
            m_fontRenderable->onRender(renderer, renderer.getCurrentModelViewProjectionMatrix(), glm::vec3(totalTextLength, m_fontSize, 1.0f), m_text, m_dropShadowColor, xOffset + m_fontSize / 20.0f, yOffset + m_fontSize / 20.0f);
        }

        m_fontRenderable->onRender(renderer, renderer.getCurrentModelViewProjectionMatrix(), glm::vec3(totalTextLength, m_fontSize, 1.0f), m_text, m_color, xOffset, yOffset);
    }
}

void UILabel::setText(const char *text)
{
    m_text = text;
    m_textWidth = m_fontRenderable->getFont()->getWidthForString(text);
}

std::string UILabel::GetText() {
    return m_text;
}

void UILabel::setTextString(const std::string& text) {
	m_text_backing = text;
	//This is terrible we are leaking string IDs, but we need to do this
	//so that we return a const char* string, since the font renderable uses an odd 
	//rendering optimization that creates a new buffer for every string rendered!
	setText(ID_TO_STRING(STRING_TO_ID(m_text_backing))->c_str());
}

void UILabel::setColor(const glm::vec3& color)
{
    setColor(color.x, color.y, color.z);
}

void UILabel::setColor(float r, float g, float b)
{
    m_color.x = r;
    m_color.y = g;
    m_color.z = b;
}

void UILabel::setDropShadowColor(const glm::vec3& color)
{
    setDropShadowColor(color.x, color.y, color.z);
}

void UILabel::setDropShadowColor(const glm::vec4& color)
{
    setDropShadowColor(color.x, color.y, color.z, color.w);
}

void UILabel::setDropShadowColor(float r, float g, float b, float a)
{
    m_dropShadowColor.x = r;
    m_dropShadowColor.y = g;
    m_dropShadowColor.z = b;
    m_dropShadowColor.w = a;
}

void UILabel::setDropShadowAlpha(float a)
{
    m_dropShadowColor.w = a;
}

void UILabel::setAlpha(float a)
{
    m_color.w = a;
}

void UILabel::setTextLayout(AnchorType anchorType)
{
    m_textLayout = anchorType;
}
