//
//  MeshManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 12/4/12.
//
//

#include "Core/Rendering/MeshManager/MeshManager.h"
#include "Core/Rendering/MeshManager/DynamicMesh.h"
#include "Core/Debug/Logger/Logger.h"
#include <string>
#include <algorithm>

MeshManager* MeshManager::m_instance = NULL;

void MeshManager::init(IResourceLoader *resourceLoader)
{
    m_resourceLoader = resourceLoader;
    MeshManager::m_instance = this;
    m_vertexBuffer  = new Vector3<GLfloat>[50000];
    m_faceBuffer    = new Vector3<GLint>  [50000];
    m_uvBuffer      = new Vector2<GLfloat>[50000];
    m_normalBuffer  = new Vector3<GLfloat>[50000];
    m_charBuffer    = new char [256];
}

Mesh* MeshManager::getMesh(StringManager::StringID name, MeshType type)
{    
    if(m_meshMap.find(name) != m_meshMap.end())
    {
        return m_meshMap[name];
    }
    
    std::string postFix;
    
    Mesh *mesh = NULL;
    
    switch(type)
    {
        case MESH_TYPE_STATIC:
            mesh = new Mesh();
            readOBJMesh(name, *mesh);
            break;
        case MESH_TYPE_DYNAMIC:
            mesh = new DynamicMesh();
            readOBJMesh(name, *mesh);
            break;
        default:
            //KKonecko throw exception!
            return NULL;
    }
 
    m_meshMap[name] = mesh;
    return mesh;
}

void MeshManager::readOBJMesh(StringManager::StringID name, Mesh &mesh)
{
    std::string nameLowerStr = *StringManager::getStringFromID(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
    std::string path = m_resourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() +".obj";
    
    std::ifstream file;
    file.open(path.c_str());
    
    int vertexIndex = 0;
    int triangleIndex = 0;
    int uvIndex = 0;
    int normalIndex = 0;
    
    if(file.good())
    {
    
        while(file.good())
        {
            file.getline(m_charBuffer, 256);
            //std::cout << m_charBuffer << std::endl;
            //Find
            if(file.gcount() > 2)
            {
                switch (m_charBuffer[0]) {
                    case 'o':
                        break;
                    case 'u':
                        break;
                    case 'v':
                        switch (m_charBuffer[1])
                        {
                            case ' ':
                                readObjVertexLine(m_vertexBuffer[vertexIndex++]);
                                break;
                            case 't':
                                readObjUVLine(m_uvBuffer[uvIndex++]);
                                break;
                            case 'n':
                                readObjNormalLine(m_normalBuffer[normalIndex++]);
                                break;
                            default:
                                break;
                        }
                    
                        break;
                    case 'f':
                        if(m_charBuffer[1]==' ')
                        {
                            triangleIndex += readObjFaceLine(&m_faceBuffer[triangleIndex]);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    
        mesh.init(name, m_faceBuffer, triangleIndex, m_vertexBuffer, vertexIndex, m_uvBuffer, uvIndex, m_normalBuffer, normalIndex);
    
        file.close();
    }
    else
    {
        Logger::Warning(Logger::LOADER, std::string("Failed to load model: " + *StringManager::getStringFromID(name)));
        readOBJMesh(StringManager::getIDForString("errorModel"), mesh);
    }
}

void MeshManager::readObjUVLine(Vector2<GLfloat> &uv)
{
    char *pStart = &m_charBuffer[3];
    char *pEnd;    
    uv.x = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    uv.y = std::strtof(pStart, &pEnd);
}

void MeshManager::readObjTriangleIndex(Vector3<GLint>&triangleIndex, char **pStart, char **pEnd)
{
    triangleIndex.x = (unsigned int)std::strtoul(*pStart, &*pEnd, 10) - 1;
    *pStart = *pEnd;
    if(**pStart=='/')
    {
        (*pStart)++;
        triangleIndex.y = (unsigned int)std::strtoul(*pStart, &*pEnd, 10) - 1;
        *pStart = *pEnd;
        if(**pStart=='/')
        {
            (*pStart)++;
            triangleIndex.z = (unsigned int)std::strtoul(*pStart, &*pEnd, 10) - 1;
            *pStart = *pEnd;
        }
        else
        {
            triangleIndex.z = 0;
        }
    }
    else
    {
        triangleIndex.y = 0;
        triangleIndex.z = 0;
    }
}

int MeshManager::readObjFaceLine(Vector3<GLint> *triangleIndex)
{
    int readCount = 0;
    
    char *pStart = &m_charBuffer[2];
    char *pEnd;
    readObjTriangleIndex(*triangleIndex, &pStart, &pEnd);
    ++triangleIndex;
    ++readCount;
    
    if(*pEnd != ' ')
    {
        return readCount;
    }
    
    readObjTriangleIndex(*triangleIndex, &pStart, &pEnd);
    ++triangleIndex;
    ++readCount;
    
    if(*pEnd != ' ')
    {
        return readCount;
    }
    
    readObjTriangleIndex(*triangleIndex, &pStart, &pEnd);
    ++triangleIndex;
    
    return ++readCount;
}

void MeshManager::readObjNormalLine(Vector3<GLfloat> &normalVector) const
{
    char *pStart = &m_charBuffer[2];
    char *pEnd;
    
    normalVector.x = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    normalVector.y = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    normalVector.z = std::strtof(pStart, &pEnd);
}

void MeshManager::readObjVertexLine(Vector3<GLfloat> &vertex)
{
    char *pStart = &m_charBuffer[2];
    char *pEnd;
    
    vertex.x = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    vertex.y = std::strtof(pStart, &pEnd);
    pStart = pEnd;
    vertex.z = std::strtof(pStart, &pEnd);
}


void MeshManager::release()
{
    std::map<StringManager::StringID, Mesh*>::iterator it = m_meshMap.begin();
    while(it != m_meshMap.end())
    {
        it->second->release();
        ++it;
    }
    
    delete [] m_vertexBuffer;
    delete [] m_charBuffer;
    delete [] m_faceBuffer;
    delete [] m_uvBuffer;
    delete [] m_normalBuffer;
}

MeshManager* MeshManager::getInstance()
{
    return m_instance;
}