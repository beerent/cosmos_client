//
//  UILabel.h
//  Projectios1
//
//  Created by Kamil Konecko on 9/29/13.
//
//

#ifndef Projectios1_UILabel_h
#define Projectios1_UILabel_h

#include "Core/GUI/Components/UITouchButton.h"
#include "glm/glm.hpp"
#include "Core/GUI/Style/StyleFactory.h"


//Foward decleration
class UIFontRenderable;

class UILabel: public UITouchButton
{
public:

    static const STRING_ID CLASS_ID_UI_LABEL;
    static const STRING_ID UI_LABEL_FONT_VISUAL;
	static const STRING_ID STYLE;
    
    UILabel(PropertyCollection &propertyCollection);
    virtual void onBeginRender(Renderer& renderer);
    void setText(const char *text);
    std::string GetText();
	void setTextString(const std::string& text);
    void setTextLayout(AnchorType anchorType);
    void setFontSize(float size){m_fontSize = size;}
    void setTextOffset(float x, float y);
    void setColor(const glm::vec3& color);
    void setColor(float r, float g, float b);
    void setDropShadowColor(const glm::vec3& color);
    void setDropShadowColor(const glm::vec4& color);
    void setDropShadowColor(float r, float g, float b, float a = 1.0f);
    void setAlpha(float a);
    void setDropShadowAlpha(float a);
    void setDropShadow(bool enabled){m_dropShadow = enabled;};
	void setStyle(const Style& style);

protected:
    void init(PropertyCollection &propertyCollection);
    
private:
    UIFontRenderable* m_fontRenderable;
    float             m_fontSize; //KKonecko: Font size in pixels.
    float             m_textWidth;
    float             m_xOffset;
    float             m_yOffset;
    bool              m_dropShadow;
    glm::vec4         m_color;
    glm::vec4         m_dropShadowColor;
    AnchorType        m_textLayout;
    
    std::string m_text_backing;
	const char* m_text;

    void setFontVisual(STRING_ID id);
};
#endif
