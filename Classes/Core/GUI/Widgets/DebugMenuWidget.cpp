//
//  DebugMenuWidget.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/3/14.
//
//

#include "Core/GUI/Widgets/DebugMenuWidget.h"
#include "Core/Input/ActiveCameraController.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GUI/Components/UITouchButton.h"
#include "Core/GUI/Components/UILabel.h"
#include "Core/Settings/Settings.h"

void DebugMenuWidget::init(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent)
{
    setupSettingsButtons(uiComponentFactory, parentComponent);
}

void DebugMenuWidget::setupSettingsButtons(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent)
{
    mSettingsGroup = uiComponentFactory->createUIComponent(StringManager::getIDForString("UIBackGroundArchetype"));
    mSettingsGroup->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    mSettingsGroup->setWidth(370);
    mSettingsGroup->setHeight(280);
    mSettingsGroup->setX(30);
    mSettingsGroup->setY(30);
    
    parentComponent->addChild(mSettingsGroup);

    
    
    UIComponent* settingsContainer = uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    settingsContainer->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    settingsContainer->setWidth(140);
    settingsContainer->setHeight(210);
    settingsContainer->setX(5);
    settingsContainer->setY(35);
    
    mSettingsGroup->addChild(settingsContainer);
    
    
    UILabel* label = static_cast<UILabel*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UILabelArchetype")));
    
    label->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    label->setWidth(300);
    label->setHeight(25);
    label->setX(25.0f);
    label->setY(15.0f);
    label->setFontSize(30.0f);
    label->setColor(075.0f/255.0f, 075.0f/255.0f, 255.0f/255.0f);
    label->setDropShadow(true);
    STRING_ID textStringID = StringManager::getIDForString("Debug Settings");
    label->setText(StringManager::getStringFromID(textStringID)->c_str());
    
    mSettingsGroup->addChild(label);
    

    float xMargin = 5.0f;
    float yMargin = 5.0f;
    
    //KKonecko: Forward
    
    UITouchButton* component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UITuchRadialButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    component->setX(xMargin);
    component->setY(yMargin);
    component->setWidth(50);
    component->setHeight(50);
    component->setToggleMode(true);
    component->setToggleState(Settings::getToggleSetting(Settings::SETTING_SHOW_PHYSICS_DEBUG_INFO));
    
    settingsContainer->addChild(component);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &DebugMenuWidget::onSettingShowPhysicsDebugInfoButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::DEPRESSED,  callBack);
    
    
    label = static_cast<UILabel*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UILabelArchetype")));
    
    label->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    label->setWidth(300);
    label->setHeight(25);
    label->setX(54.0f + xMargin);
    label->setY(20.0f + yMargin);
    label->setFontSize(20.0f);
    label->setDropShadow(true);
    label->setColor(055.0f/255.0f, 055.0f/255.0f, 255.0f/255.0f);
    
    textStringID = StringManager::getIDForString("Show Bullet Collision Volumes");
    label->setText(StringManager::getStringFromID(textStringID)->c_str());
    
    settingsContainer->addChild(label);
    
    
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UITuchRadialButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_LEFT);

    component->setX(xMargin);
    component->setY(50 + yMargin);
    component->setWidth(50);
    component->setHeight(50);
    component->setToggleMode(true);
    component->setToggleState(Settings::getToggleSetting(Settings::SETTING_SHOW_PHYSICS_VOLUME_OBJECTS));
    
    settingsContainer->addChild(component);
    
    callBack.bind(this, &DebugMenuWidget::onSettingShowPhysicsVolumeObjectsButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    
    label = static_cast<UILabel*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UILabelArchetype")));
    
    label->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    label->setWidth(300);
    label->setHeight(25);
    label->setX(54.0f + xMargin);
    label->setY(20.0f + 50.0f + yMargin);
    label->setFontSize(20.0f);
    label->setDropShadow(true);
    label->setColor(055.0f/255.0f, 055.0f/255.0f, 255.0f/255.0f);
    
    textStringID = StringManager::getIDForString("Toggle Engine Collision Volumes");
    label->setText(StringManager::getStringFromID(textStringID)->c_str());
    
    settingsContainer->addChild(label);
    
    
    

    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UITuchRadialButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    component->setX(xMargin);
    component->setY(100  + yMargin);
    component->setWidth(50);
    component->setHeight(50);
    component->setToggleMode(true);
    component->setToggleState(Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_AXIS));
    
    settingsContainer->addChild(component);
    
    callBack.bind(this, &DebugMenuWidget::onSettingShowObjectAxisButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

    
    label = static_cast<UILabel*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UILabelArchetype")));
    
    label->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    label->setWidth(300);
    label->setHeight(25);
    label->setX(54.0f + xMargin);
    label->setY(20.0f + 100.0f + yMargin);
    label->setFontSize(20.0f);
    label->setDropShadow(true);
    label->setColor(055.0f/255.0f, 055.0f/255.0f, 255.0f/255.0f);
    
    textStringID = StringManager::getIDForString("Toggle Show Object Axis");
    label->setText(StringManager::getStringFromID(textStringID)->c_str());
    
    settingsContainer->addChild(label);
    
    
    
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UITuchRadialButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    component->setX(xMargin);
    component->setY(150 + yMargin);
    component->setWidth(50);
    component->setHeight(50);
    component->setToggleMode(true);
    component->setToggleState(Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_NAMES));
    
    settingsContainer->addChild(component);
    
    callBack.bind(this, &DebugMenuWidget::onSettingShowObjectNamesButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    label = static_cast<UILabel*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UILabelArchetype")));
    
    label->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    label->setWidth(300);
    label->setHeight(25);
    label->setX(54.0f + xMargin);
    label->setY(20.0f + 150.0f + yMargin);
    label->setFontSize(20.0f);
    label->setDropShadow(true);
    label->setColor(055.0f/255.0f, 055.0f/255.0f, 255.0f/255.0f);
    
    textStringID = StringManager::getIDForString("Toggle Render Object Names");
    label->setText(StringManager::getStringFromID(textStringID)->c_str());
    
    settingsContainer->addChild(label);

    
}

void DebugMenuWidget::release()
{
    mSettingsGroup->release();
    delete(mSettingsGroup);
}

void DebugMenuWidget::onSettingShowPhysicsDebugInfoButtonStateChanged(UITouchButton::ButtonState state)
{
    Settings::setToggleSetting(Settings::SETTING_SHOW_PHYSICS_DEBUG_INFO, !Settings::getToggleSetting(Settings::SETTING_SHOW_PHYSICS_DEBUG_INFO));
}

void DebugMenuWidget::onSettingShowPhysicsVolumeObjectsButtonStateChanged(UITouchButton::ButtonState state)
{
    Settings::setToggleSetting(Settings::SETTING_SHOW_PHYSICS_VOLUME_OBJECTS, !Settings::getToggleSetting(Settings::SETTING_SHOW_PHYSICS_VOLUME_OBJECTS));
}

void DebugMenuWidget::onSettingShowObjectAxisButtonStateChanged(UITouchButton::ButtonState state)
{
    Settings::setToggleSetting(Settings::SETTING_SHOW_OBJECT_AXIS, !Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_AXIS));
}

void DebugMenuWidget::onSettingShowObjectNamesButtonStateChanged(UITouchButton::ButtonState state)
{
    Settings::setToggleSetting(Settings::SETTING_SHOW_OBJECT_NAMES, !Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_NAMES));
}
