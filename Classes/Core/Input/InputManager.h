//
//  ImputManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 5/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifndef Projectios1_ImputManager_h
#define Projectios1_ImputManager_h

#include <list>
#include "Core/OpenSource/FastDelegate.h"
#include "Core/OpenSource/Vector.hpp"

class InputManager
{
    public:
    
		struct InputEventData
		{
			union uData
			{
				bool m_selected;
				double m_fValue;
				int m_iValue;
				char m_cValue;

				struct position
				{
					int m_x;
					int m_y;
				};
				position m_position;
			};

		private:
			uData m_data;
			int   m_id;

		public:
			void setSelected(bool selected) { m_data.m_selected = selected; }
			bool getSelected() { return m_data.m_selected; }
			void setFValue(double value) { m_data.m_fValue = value; }
			double getFValue() { return m_data.m_fValue; }
			void setIValue(double value) { m_data.m_iValue = value; }
			int  getIValue() { return m_data.m_iValue; }
			void setCharValue(char value) { m_data.m_cValue = value; }
			char getCharValue() { return m_data.m_cValue; }

            void setPosition(ivec2 vector)
            {
                m_data.m_position.m_x = vector.x;
                m_data.m_position.m_y = vector.y;
            }
            ivec2 getPosition(){return ivec2(m_data.m_position.m_x, m_data.m_position.m_y);}
            void setID(int iD){m_id = iD;}
            int getID(){return m_id;}
        };
     
        enum DeviceInputEvent   
        { 
            DEVICE_TOUCH_DOWN, DEVICE_TOUCH_UP, DEVICE_TOUCH_MOVE, DEVICE_KEY_DOWN, DEVICE_KEY_UP
        };
    
        enum InputEvent
        {
            ON_DRAG, ON_PRESS, ON_RELEASE, ON_KEY_DOWN, ON_KEY_UP
        };
            
        typedef fastdelegate::FastDelegate2<InputEvent, InputEventData> onInputEventCallBack;
    
        InputManager();
        ~InputManager();

        static InputManager* getInstance();
        void registerForInputEvents(onInputEventCallBack callBack);
        void unregisterForInputEvents(onInputEventCallBack callBack);
        void onHardwareInputEvent(DeviceInputEvent deviceInputEvent, InputEventData data); 

    private:
    
        std::list<onInputEventCallBack> m_inputListeners;
        std::list<onInputEventCallBack> m_delayedRemoveList;
        void dispatchEvent(InputEvent event, InputEventData value);
        static InputManager* m_pInstance;
};
#endif
