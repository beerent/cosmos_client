//
//  Logger.h
//  Projectios1
//
//  Created by Kamil Konecko on 7/1/13.
//
//

#ifndef Projectios1_Logger_h
#define Projectios1_Logger_h
#include <iostream>
#include <string>
#include <stdio.h>
#include "Core/StringManager/StringManager.h"

class Logger
{
public:
    enum Channel {DEFUALT=0, LOADER, DYNAMIC_OBJECT, MEMORY, MATH_3D, SCRIPTING, LOGIC, CHANNEL_COUNT};
    StringManager::StringID m_channelNames[CHANNEL_COUNT];

private:
    
    static char mTempCharBuffer[256];
    static Logger* m_instance;
    long unsigned m_channelFlags;
    Logger();
    
public:    
    static Logger& getInstance();
    void setChannels(long unsigned const channels);
    static void Log(std::string const & logMessage);
    static void Log(Channel channel, std::string const & logMessage);
    static void Log(Channel channel, const char* logMessage);
    static void Warning(Channel channel, std::string const & logMessage);
    static void Error(std::string const & logMessage);
    static void Assert(bool const condition, std::string const & logMessage);
    static char* GetTempCharBuffer();
};
#endif
