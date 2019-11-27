//
//  StringManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 12/8/12.
//
//

#ifndef Projectios1_StringManager_h
#define Projectios1_StringManager_h

#include <iostream>
#include <string>
#include <map>

#define STRING_ID StringManager::StringID

#define CONST_STRING_DEC(a) static const StringManager::StringID a;
#define CONST_STRING_DEF(a,b) const StringManager::StringID a::b = StringManager::getIDForString(#b);
#define CONST_STRING_DEF_WS(a,b,c) const StringManager::StringID a::b = StringManager::getIDForString(#c);

#define ID_TO_STRING(a) StringManager::getStringFromID(a)
#define ID_TO_CSTR(a)   StringManager::getStringFromID(a)->c_str()
#define STRING_TO_ID(a) StringManager::getIDForString(a)

class StringManager
{
public:

    class DebugStringID
    {
    public:
        DebugStringID(int newID, std::string* text){mID = newID; mText = text;}
        DebugStringID(int newID){mID = newID; mText = NULL;}
        DebugStringID(){mID = -1; mText = NULL;}
        bool operator == (const DebugStringID &other)const { return mID == other.mID;}
        bool operator == (const int &other)const { return mID == other;}
        bool operator != (const DebugStringID &other)const { return mID != other.mID;}
        bool operator < (const DebugStringID &other)const { return mID < other.mID;}
        bool operator > (const DebugStringID &other)const { return mID > other.mID;}
        const DebugStringID& operator ++ (){ mID++; return *this;}
        const DebugStringID& operator -- (){ mID--; return *this;}
        DebugStringID& operator = (int value){mID = value; mText = getStringFromID(value); return *this;}
        operator int () const {return mID;}
    private:
        int mID;
        std::string* mText;
    };
    
    //typedef int StringID;

    typedef DebugStringID StringID;
    typedef int StringIDInt;
    
    CONST_STRING_DEC(UNDEFINED)

private:
    static StringManager* m_instance;
    
    int m_currentUniqueId;
    std::map<std::string, StringID> m_stringToID;
    std::map<StringID, std::string*> m_idToString;
    StringID _getIDForString(const std::string &name);
    std::string * _getStringFromID(StringID name);
   
public:
    static std::string * getStringFromID(StringID name)
    {
        return getInstance()->_getStringFromID(name);
    }
    
    static StringID getIDForString(const std::string &&name)
    {
        return getInstance()->_getIDForString(name);
    }

    static StringID getIDForString(const std::string &name)
    {
        return getInstance()->_getIDForString(name);
    }
    
    static StringID getIDForString(const char* name)
    {
        return getIDForString(std::string(name));
    }
    
    static std::string* mkStr(const std::string &name)
    {
        StringID strID = getIDForString(name);
        return getStringFromID(strID);
    }
    
    void init();
    void release();
    static StringManager* getInstance();
    
};

#endif
