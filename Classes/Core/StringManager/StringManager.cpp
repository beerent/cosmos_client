//
//  StringManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 12/8/12.
//
//

#include "Core/StringManager/StringManager.h"

CONST_STRING_DEF(StringManager, UNDEFINED)

StringManager* StringManager::m_instance = NULL;

//KKonecko: Refactor this so that we are not duplicating strings in memeory.
StringManager::StringID StringManager::_getIDForString(const std::string &name)
{
    int id = -1;
    
    if(m_stringToID.find(name) == m_stringToID.end())
    {
        id = ++m_currentUniqueId;
        
        //m_stringToID[name] = id;
        std::string *str = new std::string(name);
        StringID strID = StringID(id, str);
        m_stringToID[name] = strID;
        m_idToString[strID] = str;
    }
    
    return m_stringToID[name];
}

std::string * StringManager::_getStringFromID(StringID id)
{
    if(m_idToString.find(id) != m_idToString.end())
    {
        return m_idToString[id];
    }

    return NULL;
}

void StringManager::init()
{
    m_instance = this;
    m_currentUniqueId = 111;
}

void StringManager::release()
{
    std::map<StringID, std::string*>::iterator it = m_idToString.begin();
    while(it != m_idToString.end())
    {
        delete it->second;
        ++it;
    }
    m_idToString.clear();
    m_stringToID.clear();
}

StringManager* StringManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new StringManager();
        m_instance->init();
    }
    
    return m_instance;
}

