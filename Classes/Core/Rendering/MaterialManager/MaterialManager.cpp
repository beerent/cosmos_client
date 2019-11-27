//
//  MaterialManager.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 10/14/15.
//
//

#include "Core/Rendering/MaterialManager/MaterialManager.h"
#include "Core/Debug/Logger/Logger.h"
#include <string>
#include "Core/Properties/PropertyCollection.h"
#include <algorithm>

MaterialManager* MaterialManager::m_instance = NULL;

void MaterialManager::init(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
    MaterialManager::m_instance = this;
}

MaterialManager* MaterialManager::getInstance()
{
    if(m_instance == NULL)
    {
        Logger::Error("MaterialManager Instance is null! Needs to be inited before use.");
    }
    
    return m_instance;
}

Material* MaterialManager::getMaterial(StringManager::StringID name)
{
    Material* material = NULL;
    StrIDToMaterialPtrMap::iterator it = m_materials.find(name);
    if(it != m_materials.end())
    {
        material = it->second;
    }
    else
    {
        material = parseMaterial(name);
        m_materials[name] = material;
    }
    
    return material;
}

Material* MaterialManager::parseMaterial(STRING_ID name)
{
    std::string nameLowerStr = *StringManager::getStringFromID(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
    
    std::string path = m_resourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() +".txt";
    std::ifstream file;
    file.open(path.c_str());
    
    PropertyCollection propertyCollection;
    char buffer[256];
    
    if(file.good())
    {
        while(file.good())
        {
            file.getline(buffer, 256);
            //std::cout << m_charBuffer << std::endl;
            propertyCollection.addPropertyFromStream(buffer);
        }
        
        file.close();
    }
    else
    {
        Logger::Warning(Logger::LOADER, std::string("Failed to load " + *StringManager::getStringFromID(name)));
        return parseMaterial(StringManager::getIDForString("errorMaterial"));
    }
    
    Material* material = new Material();
    material->init(propertyCollection);
    return material;
}