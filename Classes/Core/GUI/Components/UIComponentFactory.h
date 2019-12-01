//
//  EntityManger.h
//  Projectios1
//
//  Created by Kamil Konecko on 12/18/12.
//
//
#ifndef Projectios1_UIComponentFactory_h
#define Projectios1_UIComponentFactory_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Interface.hpp"
#include "Core/StringManager/StringManager.h"
#include "Core/GUI/Components/UIComponent.h"
#include "Core/Properties/PropertyCollection.h"
#include <Core/GUI/Components/UILabel.h>

class UIComponentFactory
{
public:
    static const StringManager::StringID UI_TUCH_BUTTON;
    static const StringManager::StringID UI_GROUP;
    static const StringManager::StringID UI_LABEL;
    
private:
    IResourceLoader *m_resourceLoader;
    std::map<StringManager::StringID, PropertyCollection*> m_uiComponentArchetypeMap;
    static UIComponentFactory * m_instance;
    char *m_charBuffer;
    
public:
    void init(IResourceLoader *resourceLoader);
    
    UIComponent* createUIComponent(StringManager::StringID name);
	UIComponent* createUIComponent(const std::string& name, float width, float height, UIComponent::AnchorType anchorType);
    
	UITouchButton* createUITouchButton(const std::string& name, UIComponent::AnchorType anchorType);
    
	UILabel* createUILabel(StringManager::StringID name, float width, float height, UIComponent::AnchorType anchorType, const std::string& text);
	UILabel* createUILabel(const std::string& name, float width, float height, UIComponent::AnchorType anchorType, const std::string& text);
    

    void release();
    static UIComponentFactory* getInstance();
private:
    UIComponent* createUIComponent(PropertyCollection *pArchetype);
    PropertyCollection * readUIComponentArchetype(StringManager::StringID name);
};

#endif
