//
//  PropertyCollection.h
//  Projectios1
//
//  Created by Kamil Konecko on 12/18/12.
//
//

#ifndef Projectios1_PropertyCollection_h
#define Projectios1_PropertyCollection_h

#include <string>
#include <vector>
#include <map>
#include "Core/StringManager/StringManager.h"
#include "Core/Debug/Logger/Logger.h"

class IResourceLoader;
class PropertyCollection
{
public:
    
    static const StringManager::StringID TYPE;
    
    enum PropertyType{INT,STR_ID,DOUBLE};

    struct Property{
        Property()
        {
            static int test = 0;
            test++;
        }
        PropertyType m_type;
        StringManager::StringID m_name;
        union Data{
            long intValue;
            double doubleValue;
            StringManager::StringIDInt StringIDValue;
        }m_data;
    };

    PropertyCollection()
    {
    }
    
    void addPropertyFromStream(char *pStream);
    const Property * getPropertyAtIndex(int index) const;
    unsigned int getPropertyCount() const;
    
    bool findPropertyByName(StringManager::StringID const &name, Property const **property)const;
    bool getValueForProperty(StringManager::StringID const &name, bool &value)const;
    bool getValueForProperty(StringManager::StringID const &name, long &value)const;
    bool getValueForProperty(StringManager::StringID const &name, double &value)const;
    bool getValueForProperty(StringManager::StringID const &name, float &value)const;
    bool getValueForProperty(StringManager::StringID const &name, STRING_ID &value)const;    
    
private:
    std::vector<Property> m_properties;
    typedef std::map<StringManager::StringID, int> StringIDToInt;
    
    StringIDToInt m_mapOfProperties;
    StringManager::StringID getStringIDFromStream(char *pStart, char **pEnd);    
};

class PropertyCollectionRepository {
public:
	PropertyCollectionRepository(const IResourceLoader& resourceLoader);
	const PropertyCollection* getPropertyCollection(const STRING_ID& name);

private:
	const IResourceLoader& mResourceLoader;
	std::map<STRING_ID, PropertyCollection*> mProperties;

	PropertyCollection* readPropertyCollection(StringManager::StringID name);
};

#endif
