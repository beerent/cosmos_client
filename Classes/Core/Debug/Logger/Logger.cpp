//
//  Logger.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 7/1/13.
//
//

#include "Core/Debug/Logger/Logger.h"
#include <assert.h>

#ifndef __APPLE__
#include "Windows.h"
#endif

Logger* Logger::m_instance = NULL;
char Logger::mTempCharBuffer[256];

Logger::Logger()
{
    m_channelNames[0] = StringManager::getIDForString("Default");
    m_channelNames[1] = StringManager::getIDForString("Loader");
    m_channelNames[2] = StringManager::getIDForString("Dynamic Object");
    m_channelNames[3] = StringManager::getIDForString("Memory");
    m_channelNames[4] = StringManager::getIDForString("Math 3d");
    m_channelNames[5] = StringManager::getIDForString("Scripting");
    m_channelNames[6] = StringManager::getIDForString("Logic");


    m_channelFlags = 0;
    m_channelFlags |= 1<<DEFUALT;
    m_channelFlags |= 1<<LOADER;
    //m_channelFlags |= 1<<DYNAMIC_OBJECT;
    //m_channelFlags |= 1<<MEMORY;
    //m_channelFlags |= 1<<MATH_3D;
    //m_channelFlags |= 1<<SCRIPTING;
    //m_channelFlags |= 1<<LOGIC;
    
}

Logger& Logger::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new Logger();
    }
    
    return *m_instance;
}

char* Logger::GetTempCharBuffer()
{
    return mTempCharBuffer;
}

void Logger::setChannels(long unsigned const channels)
{
    m_channelFlags = channels;
}


void Logger::Log(std::string const & logMessage)
{
    Log(DEFUALT, logMessage);
}

void Logger::Log(Channel channel, const char* logMessage)
{
    Logger instance = getInstance();
    
    if(1<<channel & instance.m_channelFlags)
    {
#ifdef __APPLE__
		std::cout << "CH: " << StringManager::getStringFromID(instance.m_channelNames[channel])->c_str() << ": " << logMessage << std::endl;
#else
		char buffer[1024];
		sprintf_s(buffer, 1024, "CH: %s : %s \n", StringManager::getStringFromID(instance.m_channelNames[channel])->c_str(), logMessage);
		OutputDebugStringA(buffer);
#endif
    }
}

void Logger::Log(Channel channel, std::string const & logMessage)
{
    Log(channel, logMessage.c_str());
}

void Logger::Warning(Channel channel, std::string const & logMessage)
{
    Logger instance = getInstance();

    if(1<<channel & instance.m_channelFlags)
    {
#ifdef __APPLE__
        std::cout <<"-------------------------------------------------------------------------"<<std::endl;
        std::cout <<"Warning channel " << StringManager::getStringFromID(instance.m_channelNames[channel])->c_str() << ": " << logMessage << std::endl;
        std::cout <<"-------------------------------------------------------------------------"<<std::endl;
#else
		char buffer[1024];
		sprintf_s(buffer, 1024, "------------------------------------------------------------------------ -\n");
		OutputDebugStringA(buffer);
		sprintf_s(buffer, 1024, "Warning channel %s : %s \n", StringManager::getStringFromID(instance.m_channelNames[channel])->c_str(), logMessage.c_str());
		OutputDebugStringA(buffer);
		sprintf_s(buffer, 1024, "------------------------------------------------------------------------ \n");
		OutputDebugStringA(buffer);
#endif
    }
}

void Logger::Error(std::string const & logMessage)
{
#ifdef __APPLE__
    std::cout <<""<<std::endl;
    std::cout <<"****************************************************************************"<<std::endl;
    std::cout <<"Error: " << logMessage << std::endl;
    std::cout <<"****************************************************************************"<<std::endl;
    std::cout <<""<<std::endl;
#else
	char buffer[1024];
	sprintf_s(buffer, 1024, "****************************************************************************\n");
	OutputDebugStringA(buffer);
	sprintf_s(buffer, 1024, "Error: %s \n", logMessage.c_str());
	OutputDebugStringA(buffer); 
	sprintf_s(buffer, 1024, "****************************************************************************\n");
	OutputDebugStringA(buffer);
#endif
    assert(false);
}


void Logger::Assert(bool const condition, std::string const & logMessage)
{
    if(condition)
    {
#ifdef __APPLE__
        std::cout <<""<<std::endl;
        std::cout <<"****************************************************************************"<<std::endl;
        std::cout <<"Assert: " << logMessage << std::endl;
        std::cout <<"****************************************************************************"<<std::endl;
        std::cout <<""<<std::endl;
#else
		char buffer[1024];
		sprintf_s(buffer, 1024, "****************************************************************************\n");
		OutputDebugStringA(buffer);
		sprintf_s(buffer, 1024, "Assert: %s \n", logMessage.c_str());
		OutputDebugStringA(buffer);
		sprintf_s(buffer, 1024, "****************************************************************************\n");
		OutputDebugStringA(buffer);
#endif
		assert(false);
    }
}