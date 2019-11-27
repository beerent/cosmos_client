//
//  EntityManger.h
//  Projectios1
//
//  Created by Kamil Konecko on 12/18/12.
//
//
#ifndef Projectios1_EntityFactory_h
#define Projectios1_EntityFactory_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Interface.hpp"
#include "Core/OpenSource/Vector.hpp"
#include "Core/StringManager/StringManager.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/Properties/PropertyCollection.h"

class EntityFactory
{
public:
    CONST_STRING_DEC(TYPE_ENTITY)
    CONST_STRING_DEC(TYPE_CAMERA)
    CONST_STRING_DEC(TYPE_SKYBOX)
    CONST_STRING_DEC(TYPE_WATER_PLANE)
    CONST_STRING_DEC(TYPE_PHYSICS_ENTITY)
    CONST_STRING_DEC(TYPE_PROPULSION_ENTITY)
    CONST_STRING_DEC(TYPE_WEAPON_ENTITY)
	CONST_STRING_DEC(TYPE_CHARACTER_ENTITY)
    
private:
    IResourceLoader *m_resourceLoader;
    typedef std::map<StringManager::StringID, PropertyCollection*> EntityArchetypeMap;
    EntityArchetypeMap m_entityArchetypeMap;
    static EntityFactory *m_instance;
    char *m_charBuffer;
 
public:
    void init(IResourceLoader *resourceLoader);
    Entity* createEntity(StringManager::StringID name, Entity* parent = NULL);
    void createEntities(STRING_ID name, Entity* parent);

    void release();
    static EntityFactory* getInstance();
private:
    Entity* createEntity(PropertyCollection *pArchetype, Entity* parent = NULL);
    PropertyCollection * readEntityArchetype(StringManager::StringID name);

    bool readEntityNameInfo(const char* stream, STRING_ID &type, STRING_ID &archetype, STRING_ID &instanceName);
    bool readVector3(const char* stream, Vector3<GLfloat>& outVector);
    bool readVector4(const char* stream, Vector4<GLfloat>& outVector);

};

#endif
