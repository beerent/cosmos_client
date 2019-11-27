//
//  EntityFactory.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 12/18/12.
//
//

#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GUI/Components/UITouchButton.h"
#include "Core/GUI/Components/UILabel.h"
#include "Core/Debug/Logger/Logger.h"
#include <string>
#include <algorithm>

UIComponentFactory* UIComponentFactory::m_instance = NULL;
const StringManager::StringID UIComponentFactory::UI_TUCH_BUTTON = StringManager::getIDForString("UI_TUCH_BUTTON");
const StringManager::StringID UIComponentFactory::UI_GROUP = StringManager::getIDForString("UI_GROUP");
const StringManager::StringID UIComponentFactory::UI_LABEL = StringManager::getIDForString("UI_LABEL");

void UIComponentFactory::init(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
    UIComponentFactory::m_instance = this;
    m_charBuffer = new char [256];
}

UITouchButton* UIComponentFactory::createUITouchButton(const std::string& name, UIComponent::AnchorType anchorType) {
	UITouchButton* touchButton = static_cast<UITouchButton*>(createUIComponent(STRING_TO_ID(name)));
	touchButton->setAnchor(anchorType);
	return touchButton;
}

UILabel* UIComponentFactory::createUILabel(const std::string& name, float width, float height, UIComponent::AnchorType anchorType, const std::string& text) {
	return createUILabel(STRING_TO_ID(name), width, height, anchorType, text);
}

UILabel* UIComponentFactory::createUILabel(StringManager::StringID name, float width, float height, UIComponent::AnchorType anchorType, const std::string& text) {
	UILabel* label = static_cast<UILabel*>(createUIComponent(name));
	label->setWidth(width);
	label->setHeight(height);
	label->setAnchor(anchorType);
	label->setTextString(text);
	return label;
}

UIComponent* UIComponentFactory::createUIComponent(const std::string& name, float width, float height, UIComponent::AnchorType anchorType) {
	UIComponent* component = createUIComponent(STRING_TO_ID(name));
	component->setWidth(width);
	component->setHeight(height);
	component->setAnchor(anchorType);
	return component;
}

UIComponent* UIComponentFactory::createUIComponent(StringManager::StringID name)
{
    PropertyCollection *pArchetype = NULL;
    
    if(m_uiComponentArchetypeMap.find(name) != m_uiComponentArchetypeMap.end())
    {
        pArchetype = m_uiComponentArchetypeMap[name];
    }
    
    if(pArchetype == NULL)
    {
        pArchetype= readUIComponentArchetype(name);
    }
    
    return createUIComponent(pArchetype);
}

UIComponent* UIComponentFactory::createUIComponent(PropertyCollection *pArchetype)
{
    UIComponent *pUIComponent = NULL;
    
    //find type property
    PropertyCollection::Property const* property = NULL;
    
    if(pArchetype->findPropertyByName(PropertyCollection::TYPE, &property))
    {
        StringManager::StringID typeName = property->m_data.StringIDValue;

        if( typeName == UI_TUCH_BUTTON)
        {
            pUIComponent = new UITouchButton(*pArchetype);
        }
        else if( typeName == UI_GROUP)
        {
            pUIComponent = new UIComponent(*pArchetype);
        }
        else if( typeName == UI_LABEL)
        {
            pUIComponent = new UILabel(*pArchetype);
        }
        else
        {
            //error
        }
    }
    else
    {
        //error
    }
    
    return pUIComponent;
}

PropertyCollection * UIComponentFactory::readUIComponentArchetype(StringManager::StringID name)
{
    std::string nameLowerStr = *StringManager::getStringFromID(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
    std::string path = m_resourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() +".txt";
    
    std::ifstream file;
    file.open(path.c_str());
    
    PropertyCollection *propertyCollection = new PropertyCollection();
    m_uiComponentArchetypeMap[name] = propertyCollection;
    
    if(file.good())
    {
        while(file.good())
        {
            file.getline(m_charBuffer, 256);
            //std::cout << m_charBuffer << std::endl;
            propertyCollection->addPropertyFromStream(m_charBuffer);
        }
        file.close();
    }
    else
    {
        Logger::Error("Failed to load property information for " + *StringManager::getStringFromID(name));
        return readUIComponentArchetype(StringManager::getIDForString("UITouchButtonArchetype"));
    }
    
    return propertyCollection;
}

void UIComponentFactory::release()
{
    std::map<StringManager::StringID, PropertyCollection*>::iterator it = m_uiComponentArchetypeMap.begin();
    while(it != m_uiComponentArchetypeMap.end())
    {
        delete it->second;
        ++it;
    }
    
    delete [] m_charBuffer;
}

UIComponentFactory* UIComponentFactory::getInstance()
{
    return m_instance;
}
