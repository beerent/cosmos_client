//
//  UIComponent.h
//  Projectios1
//
//  Created by Kamil Konecko on 2/12/13.
//
//

#ifndef Projectios1_UIComponent_h
#define Projectios1_UIComponent_h
#include "Core/StringManager/StringManager.h"
#include <map>
#include <list>
#include "Core/Rendering/Renderer.h"
#include "Core/Properties/PropertyCollection.h"
#include "glm/gtc/quaternion.hpp"
#include "Core/Input/InputManager.h"
#include "Core/Memory/DynamicObject.h"

class UIComponent : public DynamicObject
{
public:
    
    
    enum AnchorType {ANCHOR_TOP_LEFT = 0, ANCHOR_TOP_CENTER, ANCHOR_TOP_RIGHT, ANCHOR_LEFT, ANCHOR_CENTER,
        ANCHOR_RIGHT, ANCHOR_BOTTOM_LEFT, ANCHOR_BOTTOM_CENTER, ANCHOR_BOTTOM_RIGHT};
    enum LayoutScheme {ANCHOR = 0};

    CONST_STRING_DEC(CLASS_ID_UI_COMPONENT);
    
private:
    CONST_STRING_DEC(UI_COMPONENT_WIDTH);
    CONST_STRING_DEC(UI_COMPONENT_HEIGHT);
    CONST_STRING_DEC(VISUAL);

    static int m_uniqueID;
    
    bool m_waitingForReleaseEvent;
    AnchorType m_anchor;
    LayoutScheme m_layout;
    
    StringManager::StringID m_id;
    UIComponent *m_parent;

    bool m_transformationMatrixValid;
    glm::mat4 m_transformation;
    std::map<StringManager::StringID, UIComponent*> m_children;
    std::list<UIComponent*> m_listOfChildren;
    bool m_processingRender;
    
    int m_tuchID = 0;

protected:
    double m_width;
    double m_height;
    double m_x;
    double m_y;
    
    double m_postLayoutX;
    double m_postLayoutY;
    double m_xGlobal;
    double m_yGlobal;
    bool m_captureInput;
    bool mVisible;
    RenderableObj* m_visual;
    
    
    
public:
    void init(PropertyCollection &propertyCollection);
    UIComponent(PropertyCollection &propertyCollection);
    UIComponent(PropertyCollection &propertyCollection, StringManager::StringID classTypeID);
    
    bool addChild(UIComponent * component);
    bool removeChild(StringManager::StringID componentID);
    
    bool ProcessingRender() const;// { return m_processingRender; }

    virtual void onBeginRender(Renderer& renderer);
    virtual void onLayout();
    virtual void onLayout(double globalOffsetX, double globalOffsetY);

    virtual void setWidth(double width){m_width = width;};
    virtual void setHeight(double height){m_height = height;};
    virtual double getHeight(){return m_height;};
    virtual double getWidth(){return m_width;};
    
    void setAnchor(AnchorType anchor){
        m_anchor = anchor;
    }

    void setLayout(LayoutScheme layout){
        m_layout = layout;
    }
    
    void setX(double x){m_x = x;};
    void setY(double y){m_y = y;};
    double getY(){return m_y;};
    double getX(){return m_x;};
    
    StringManager::StringID getID();
    void setID(StringManager::StringID id){m_id = id;};
    virtual const glm::mat4& getTransformation();
    
    virtual void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data);
    virtual void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data, bool &captured);
    virtual void release();
    
    void setVisible(bool value){mVisible = value;};
    bool getVisible(){return mVisible;};
    
protected:
    virtual void handleOnPress();
    virtual void handleOnRelease();
    
private:
    void onAddedToParent(UIComponent *parent){m_parent = parent;};
    void onRemovedFromParent(){m_parent = NULL;};
    void validateTransformationMatrix();
    void doAnchorBasedLayout();
    void convertToLocal(double x, double y, double & outX, double & outY);
    bool containsPoint(double x, double  y);
};
#endif
