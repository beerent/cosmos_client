//
//  PropertyCollection.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 12/18/12.
//
//
#include "Interface.hpp"
#include "Core/Properties/PropertyCollection.h" 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <algorithm>

const StringManager::StringID PropertyCollection::TYPE = StringManager::getIDForString("TYPE");

void PropertyCollection::addPropertyFromStream(char *pStream)
{
    m_properties.push_back(Property());
    
    Property &property = m_properties.back();
    
    char *pEnd;

    switch(pStream[0])
    {
        case 'l':
            property.m_type = INT;
            property.m_name = getStringIDFromStream(&pStream[2], &pEnd);
            pEnd ++;
            property.m_data.intValue =  std::strtol(pEnd, &pEnd, 10);
        break;
        case 'd':
            property.m_type = DOUBLE;
            property.m_name = getStringIDFromStream(&pStream[2], &pEnd);
            pEnd ++;
            property.m_data.doubleValue = std::strtod(pEnd, &pEnd);
        break;
        case 's':
            property.m_type = STR_ID;
            property.m_name = getStringIDFromStream(&pStream[2], &pEnd);
            pEnd ++;
            property.m_data.StringIDValue = getStringIDFromStream(pEnd, &pEnd);
        break;
    }
    
    m_mapOfProperties[property.m_name] = m_properties.size() - 1;
};

unsigned int PropertyCollection::getPropertyCount() const
{
    //sprintf(Logger::GetTempCharBuffer(), "getPropertyCount size %d", static_cast<int>(m_properties.size()));
    //Logger::Log(Logger::DEFUALT, Logger::GetTempCharBuffer());
    return m_properties.size();
}

const PropertyCollection::Property* PropertyCollection::getPropertyAtIndex(int index) const
{
    if(index >= m_properties.size())
    {
        return NULL;
    }
    
    return &m_properties[index];
}

bool PropertyCollection::findPropertyByName(StringManager::StringID const &name, Property const **property)const
{
    StringIDToInt::const_iterator it = m_mapOfProperties.find(name);
    if(it != m_mapOfProperties.end())
    {
        *property = &m_properties[it->second];
        return true;
    }
    
    return false;
}

bool PropertyCollection::getValueForProperty(StringManager::StringID const &name, bool &value) const
{
    long parsedValue = 0;
    bool found =getValueForProperty(name, parsedValue);
    if(parsedValue < 0 || parsedValue > 1)
    {
        sprintf(Logger::GetTempCharBuffer(), "PropertyCollection::getValueForProperty type BOOL missmatch for property %s",
                StringManager::getStringFromID(name)->c_str());
        Logger::Error(Logger::GetTempCharBuffer());
    }
    
    value = static_cast<bool>(parsedValue);
    return found;
}

bool PropertyCollection::getValueForProperty(StringManager::StringID const &name, long &value) const
{
    Property const *property = NULL;
    if(findPropertyByName(name, &property))
    {
        if(property->m_type != PropertyCollection::INT)
        {
            sprintf(Logger::GetTempCharBuffer(), "PropertyCollection::getValueForProperty type INT missmatch for property %s",
                    StringManager::getStringFromID(name)->c_str());
            Logger::Error(Logger::GetTempCharBuffer());
        }
        value = property->m_data.intValue;
        return true;
    }
    
    return false;
}

bool PropertyCollection::getValueForProperty(StringManager::StringID const &name, double &value)const
{
    Property const *property = NULL;
    if(findPropertyByName(name, &property))
    {
        if(property->m_type != PropertyCollection::DOUBLE)
        {
            sprintf(Logger::GetTempCharBuffer(), "PropertyCollection::getValueForProperty type DOUBLE missmatch for property %s",
                    StringManager::getStringFromID(name)->c_str());
            Logger::Error(Logger::GetTempCharBuffer());
        }

        value = property->m_data.doubleValue;
        return true;
    }
    
    return false;
}

bool PropertyCollection::getValueForProperty(StringManager::StringID const &name, float &value)const
{
    Property const *property = NULL;
    if(findPropertyByName(name, &property))
    {
        if(property->m_type != PropertyCollection::DOUBLE)
        {
            sprintf(Logger::GetTempCharBuffer(), "PropertyCollection::getValueForProperty type FLOAT missmatch for property %s",
                    StringManager::getStringFromID(name)->c_str());
            Logger::Error(Logger::GetTempCharBuffer());
        }
        
        value = property->m_data.doubleValue;
        return true;
    }
    
    return false;
}

bool PropertyCollection::getValueForProperty(StringManager::StringID const &name, STRING_ID &value)const
{
    Property const *property = NULL;
    if(findPropertyByName(name, &property))
    {
        if(property->m_type != PropertyCollection::STR_ID)
        {
            sprintf(Logger::GetTempCharBuffer(), "PropertyCollection::getValueForProperty type STR_ID missmatch for property %s",
                    StringManager::getStringFromID(name)->c_str());
            Logger::Error(Logger::GetTempCharBuffer());
        }
        
        value = property->m_data.StringIDValue;
        return true;
    }
    
    return false;
}

StringManager::StringID PropertyCollection::getStringIDFromStream(char *pStart, char **pEnd)
{
    char *pCurrent = pStart;
    char *pNext = pCurrent;
    bool foundEnd = false;
    while(!foundEnd)
    {
        if(*pNext ==' ' || *pNext == 0)
        {
            foundEnd = true;
        }

        pCurrent = pNext;
        pNext ++;
    }
    
    *pEnd = pCurrent;
    
    std::string name = std::string(pStart, pCurrent - pStart);
    return StringManager::getIDForString(name);
}

PropertyCollectionRepository::PropertyCollectionRepository(const IResourceLoader& resourceLoader) :mResourceLoader(resourceLoader) {
}

const PropertyCollection* PropertyCollectionRepository::getPropertyCollection(const STRING_ID& name) {
	auto it = mProperties.find(name);
	PropertyCollection* propertyCollection = nullptr;

	if (it == mProperties.end()) {
		propertyCollection = readPropertyCollection(name);
	}
	else {
		propertyCollection = it->second;
	}

	return propertyCollection;
}

PropertyCollection* PropertyCollectionRepository::readPropertyCollection(StringManager::StringID name) {
	std::string nameLowerStr = *StringManager::getStringFromID(name);
	std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
	std::string path = mResourceLoader.getResourcePath() + "/" + nameLowerStr.c_str() + ".txt";

	std::ifstream file;
	file.open(path.c_str());
	PropertyCollection *propertyCollection = NULL;
	char buffer[256];

	if (file.good()) {
		propertyCollection = new PropertyCollection();
		mProperties[name] = propertyCollection;

		while (file.good())
		{
			file.getline(buffer, 256);
			//std::cout << m_charBuffer << std::endl;
			propertyCollection->addPropertyFromStream(buffer);
		}
		file.close();

		mProperties[name] = propertyCollection;
	}
	else {
		assert(false && "Missing property collection");
		Logger::Warning(Logger::LOADER, std::string("Failed to load archetype : " + *StringManager::getStringFromID(name)));
		return NULL; // TO DO provide a safe bad value
	}

	return propertyCollection;
}
