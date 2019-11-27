//
//  EntityFactory.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 12/18/12.
//
//

#include "Core/Entity/EntityFactory.h"
#include <Core/Entity/Entities/Camera.h>
#include <Core/Entity/Entities/SkyBox.h>
#include <Core/Entity/Entities/WaterPlaneEntity.h>
#include <Core/Debug/Logger/Logger.h>
#include <Core/Entity/Entities/PhysicsEntity.h>
#include <Core/Entity/Entities/PropulsionEntity.h>
#include <Core/Entity/Entities/WeaponEntity.h>
#include <Core/Entity/Entities/CharacterEntity.h>
#include <string>
#include <algorithm>

EntityFactory* EntityFactory::m_instance = NULL;

CONST_STRING_DEF(EntityFactory, TYPE_ENTITY)
CONST_STRING_DEF(EntityFactory, TYPE_CAMERA)
CONST_STRING_DEF(EntityFactory, TYPE_SKYBOX)
CONST_STRING_DEF(EntityFactory, TYPE_WATER_PLANE)
CONST_STRING_DEF(EntityFactory, TYPE_PHYSICS_ENTITY)
CONST_STRING_DEF(EntityFactory, TYPE_PROPULSION_ENTITY)
CONST_STRING_DEF(EntityFactory, TYPE_WEAPON_ENTITY)
CONST_STRING_DEF(EntityFactory, TYPE_CHARACTER_ENTITY)


void EntityFactory::init(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
    EntityFactory::m_instance = this;
    m_charBuffer = new char [256];
}

Entity* EntityFactory::createEntity(StringManager::StringID name, Entity* parent)
{
    PropertyCollection *pArchetype = NULL;
        
    if(m_entityArchetypeMap.find(name) != m_entityArchetypeMap.end())
    {
        pArchetype = m_entityArchetypeMap[name];
    }
    
    if(pArchetype == NULL)
    {
        pArchetype= readEntityArchetype(name);
    }
    
    return createEntity(pArchetype, parent);
}

Entity* EntityFactory::createEntity(PropertyCollection *pArchetype, Entity* parent)
{
    Entity *pEntity = NULL;
    
    //find type property
    PropertyCollection::Property const* property = NULL;
    
    if(pArchetype->findPropertyByName(PropertyCollection::TYPE, &property))
    {
        StringManager::StringID typeName = property->m_data.StringIDValue;
        if( typeName == TYPE_ENTITY)
        {
            pEntity = new Entity();
            pEntity->init(*pArchetype, parent);
        }
        else if( typeName == TYPE_CAMERA)
        {
            pEntity = new Camera();
            pEntity->init(*pArchetype, parent);
        }
        else if( typeName == TYPE_SKYBOX)
        {
            pEntity = new SkyBox();
            pEntity->init(*pArchetype, parent);
        }
        else if( typeName == TYPE_WATER_PLANE)
        {
            pEntity = new WaterPlaneEntity();
            pEntity->init(*pArchetype, parent);
        }
        else if( typeName == TYPE_PHYSICS_ENTITY)
        {
            pEntity = new PhysicsEntity();
            pEntity->init(*pArchetype, parent);
        }
        else if( typeName == TYPE_PROPULSION_ENTITY)
        {
            pEntity = new PropulsionEntity();
            pEntity->init(*pArchetype, parent);
        }
        else if( typeName == TYPE_WEAPON_ENTITY)
        {
            pEntity = new WeaponEntity();
            pEntity->init(*pArchetype, parent);
        }
		else if (typeName == TYPE_CHARACTER_ENTITY)
		{
			pEntity = new CharacterEntity();
			pEntity->init(*pArchetype, parent);
		}
        else
        {
            //error
        }
    }
    else
    {
        //error
    }

    return pEntity;
}

void EntityFactory::createEntities(STRING_ID name, Entity* parent)
{
    std::string nameLowerStr = *StringManager::getStringFromID(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
    
    std::string path = m_resourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() +"entitygroup.txt";
    
    std::ifstream file;
    file.open(path.c_str());
    
    static const char * CHAR_OBJ = "obj:";
    static const char * CHAR_POS = "pos:";
    static const char * CHAR_SCL = "scl:";
    //static const char * CHAR_ROT = "rot:";
    static const char * CHAR_QUT = "qut:";
    
    STRING_ID type = StringManager::UNDEFINED;
    STRING_ID entityArcheType = StringManager::UNDEFINED;
    STRING_ID instanceName = StringManager::UNDEFINED;

    Vector4<GLfloat> rotation;
    Vector3<GLfloat> position;
    Vector3<GLfloat> scale;
    
    if(file.good())
    {
        while(file.good())
        {
            file.getline(m_charBuffer, 256);
            if(file.gcount() > 4)
            {
                if(std::strncmp(m_charBuffer, CHAR_OBJ, 4) == 0)
                {
                    readEntityNameInfo(m_charBuffer+4, type, entityArcheType, instanceName);
                }
                else
                {
                    Logger::Error("Unexpected tag in Entity Group File " + *StringManager::getStringFromID(name));
                }
            }
            else
            {
                //KKonecko: Probobly just a new line lets exit.
                continue;
            }
/*
            file.getline(m_charBuffer, 256);
            if(file.gcount() > 4)
            {
                if(std::strncmp(m_charBuffer, CHAR_ROT, 4) == 0)
                {
                    readVector3(m_charBuffer+4, rotation);
                }
                else
                {
                    Logger::Error("Unexpected tag in Entity Group File " + *StringManager::getStringFromID(name));
                }
            }
*/
            
            file.getline(m_charBuffer, 256);
            if(file.gcount() > 4)
            {
                if(std::strncmp(m_charBuffer, CHAR_QUT, 4) == 0)
                {
                    readVector4(m_charBuffer+4, rotation);
                }
                else
                {
                    Logger::Error("Unexpected tag in Entity Group File " + *StringManager::getStringFromID(name));
                }
            }
            
            file.getline(m_charBuffer, 256);
            if(file.gcount() > 4)
            {
                if(std::strncmp(m_charBuffer, CHAR_POS, 4) == 0)
                {
                    readVector3(m_charBuffer+4, position);
                }
                else
                {
                    Logger::Error("Unexpected tag in Entity Group File " + *StringManager::getStringFromID(name));
                }
            }

            file.getline(m_charBuffer, 256);
            if(file.gcount() > 4)
            {
                if(std::strncmp(m_charBuffer, CHAR_SCL, 4) == 0)
                {
                    readVector3(m_charBuffer+4, scale);
                }
                else
                {
                    Logger::Error("Unexpected tag in Entity Group File " + *StringManager::getStringFromID(name));
                }
            }
            
            Entity *entity = createEntity(entityArcheType, parent);
            entity->SetPosition(glm::vec3(position.x, position.z, -position.y));
            entity->setScale(glm::vec3(scale.x, scale.z, scale.y));
            entity->mGroupInstanceID = instanceName;
            
/*
            float angleX = rotation.x * 180 / 3.14;
            float angleY = rotation.z * 180 / 3.14;
            float angleZ = -rotation.y * 180 / 3.14;
            
            
            glm::quat rotationX = glm::angleAxis<float>(angleX,1.0,0.0,0.0);
            glm::quat rotationY = glm::angleAxis<float>(angleY,0.0,1.0,0.0);
            glm::quat rotationZ = glm::angleAxis<float>(angleZ,0.0,0.0,1.0);
 */
            glm::quat quatRotation;
            quatRotation.w = rotation.w;
            quatRotation.x = rotation.x;
            quatRotation.y = rotation.z;
            quatRotation.z = -rotation.y;
            entity->SetRotation(quatRotation);
        }
        file.close();
    }
    else
    {
        Logger::Warning(Logger::LOADER, std::string("Failed to load entity group : " + *StringManager::getStringFromID(name)));
        createEntity(StringManager::getIDForString("errorCubeArchetype"), parent);
    }
}

bool EntityFactory::readVector3(const char* stream, Vector3<GLfloat>& outVector)
{
    const char *pStart = stream;
    char *pEnd;
    
    outVector.x = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    outVector.y = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    outVector.z = std::strtof(pStart, &pEnd);

    return true;
}

bool EntityFactory::readVector4(const char* stream, Vector4<GLfloat>& outVector)
{
    const char *pStart = stream;
    char *pEnd;

    outVector.w = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    outVector.x = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    outVector.y = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    outVector.z = std::strtof(pStart, &pEnd);
    
    return true;
}

bool EntityFactory::readEntityNameInfo(const char* stream, STRING_ID &type, STRING_ID &archetype, STRING_ID &instanceName)
{
    const char* readPtr = stream;
    const char* startPtr = stream;
    
    while(*readPtr != '.' && *readPtr != ' ')
    {
        readPtr++;
    }
    
    if(*readPtr == ' ')
    {
        Logger::Error(std::string("Invalid Entity Group Object Name : TYPE "));
        return false;
    }
    
    type = StringManager::getIDForString(std::string(startPtr, readPtr - startPtr));
    
    readPtr ++;
    startPtr = readPtr;
    
    while(*readPtr != '.' && *readPtr != ' ')
    {
        readPtr++;
    }

    if(*readPtr == ' ')
    {
        Logger::Error(std::string("Invalid Entity Group Object Name : Archetype"));
        return false;
    }

    archetype = StringManager::getIDForString(std::string(startPtr, readPtr - startPtr) + std::string("Archetype"));
    
    readPtr ++;
    startPtr = readPtr;

    while(*readPtr != ' ' && *readPtr != 0)
    {
        readPtr++;
    }

    std::string instNameStr = std::string(startPtr, readPtr - startPtr);
    if(instNameStr.length() > 3)
    {
        instNameStr = instNameStr.substr(0, 3);
    }
    instanceName = StringManager::getIDForString(instNameStr);
    
    return true;
}

PropertyCollection * EntityFactory::readEntityArchetype(StringManager::StringID name)
{
    std::string nameLowerStr = *StringManager::getStringFromID(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
    std::string path = m_resourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() +".txt";
    
    std::ifstream file;
    file.open(path.c_str());
    PropertyCollection *propertyCollection = NULL;
    
    if(file.good())
    {
        propertyCollection = new PropertyCollection();
        m_entityArchetypeMap[name] = propertyCollection;

        while(file.good())
        {
            file.getline(m_charBuffer, 256);
            //std::cout << m_charBuffer << std::endl;
            propertyCollection->addPropertyFromStream(m_charBuffer);
        }
        file.close();
    }
    else
    {
        Logger::Warning(Logger::LOADER, std::string("Failed to load archetype : " + *StringManager::getStringFromID(name)));
        return readEntityArchetype(StringManager::getIDForString("errorCubeArchetype"));
    }
    
    return propertyCollection;
}

void EntityFactory::release()
{
    EntityArchetypeMap::iterator it = m_entityArchetypeMap.begin();
    while(it != m_entityArchetypeMap.end())
    {
        delete it->second;
        ++it;
    }
    
    delete [] m_charBuffer;
}

EntityFactory* EntityFactory::getInstance()
{
    return m_instance;
}
