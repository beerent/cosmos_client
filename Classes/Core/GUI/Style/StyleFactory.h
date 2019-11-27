#pragma once
#include "Core/StringManager/StringManager.h"
#include "glm/glm.hpp"
#include "Core/GUI/Components/UIComponent.h"

class Style {
public:
	virtual int getFontSize() const = 0;
	virtual glm::vec3 getFontColor() const = 0;
	virtual glm::vec4 getDropShadowColor() const = 0;
	virtual bool getHasDropShadow() const = 0;
	virtual UIComponent::AnchorType getTextLayout() const = 0;
	virtual glm::vec2 getTextOffsets() const = 0;
};

class KYCQuestionHeaderStyle : public Style {
public:
	int getFontSize() const {
		return 25;
	}
	
	glm::vec3 getFontColor() const {
		const glm::vec3 labelColor(255.0f / 255.0f, 220.0f / 255.0f, 10.0f / 255.0f);
		return labelColor;
	}

	glm::vec4 getDropShadowColor() const {
		const glm::vec4 dropShadowColor(255.0f / 455.0f, 180.0f / 455.0f, 0.0f / 255.0f, 0.5);
		return dropShadowColor;
	}
	
	bool getHasDropShadow() const {
		return true;
	}

	UIComponent::AnchorType getTextLayout() const {
		return UIComponent::ANCHOR_CENTER;
	}

	glm::vec2 getTextOffsets() const {
		return glm::vec2(0.0f, 0.5f);
	}

};

class KYCDebugLabelStyle : public Style {
public:
	int getFontSize() const {
		return 20;
	}

	glm::vec3 getFontColor() const {
		const glm::vec3 labelColor(125.0f / 255.0f, 125.0f / 255.0f, 25.0f / 255.0f);
		return labelColor;
	}

	glm::vec4 getDropShadowColor() const {
		const glm::vec4 dropShadowColor(255.0f / 455.0f, 180.0f / 455.0f, 0.0f / 255.0f, 0.5);
		return dropShadowColor;
	}

	bool getHasDropShadow() const {
		return false;
	}

	UIComponent::AnchorType getTextLayout() const {
		return UIComponent::ANCHOR_LEFT;
	}

	glm::vec2 getTextOffsets() const {
		return glm::vec2(0.0f, 0.0f);
	}
};


class StyleFactory {
public:
	static const STRING_ID KYC_QUESTION_HEADER;
	static const STRING_ID KYC_DEBUG_LABEL;

	const Style& GetStyle(STRING_ID name) {
		if (name == KYC_QUESTION_HEADER) {
			return m_kycQuestionHeaderStyle;
		}

		if (name == KYC_DEBUG_LABEL) {
			return m_kycDebugLabelStyle;
		}
	}

	static StyleFactory& getInstance() {
		if (m_instance == nullptr) {
			m_instance = new StyleFactory();
		}
		return *m_instance;
	}

private:
	KYCQuestionHeaderStyle m_kycQuestionHeaderStyle;
	KYCDebugLabelStyle m_kycDebugLabelStyle;
 
	static StyleFactory* m_instance;

};