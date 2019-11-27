//
//  MeshManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 12/4/12.
//
//

#ifndef Projectios1_MeshManager_h
#define Projectios1_MeshManager_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Core/Rendering/MeshManager/Mesh.h"
#include "Interface.hpp"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Core/OpenSource/Vector.hpp"
#include "Core/StringManager/StringManager.h"

class MeshManager
{
public:
    enum MeshType{MESH_TYPE_STATIC, MESH_TYPE_DYNAMIC};
    
private:
    
    IResourceLoader *m_resourceLoader;
    std::map<StringManager::StringID, Mesh*> m_meshMap;
    static MeshManager * m_instance;
    Vector3<GLfloat> *m_vertexBuffer;
    Vector3<GLfloat> *m_normalBuffer;
    Vector2<GLfloat> *m_uvBuffer;
    Vector3<GLint>   *m_faceBuffer;
    char *m_charBuffer;
    
    void readOBJMesh(StringManager::StringID name, Mesh &mesh);
    void readObjVertexLine(Vector3<GLfloat> &vector);
    int readObjFaceLine(Vector3<GLint> *triangleIndex);
    void readObjTriangleIndex(Vector3<GLint>&triangleIndex, char **pStart, char**pEnd);
    void readObjUVLine(Vector2<GLfloat> &uv);
    void readObjNormalLine(Vector3<GLfloat> &normalVector) const;

public:
    void init(IResourceLoader *resourceLoader);
    Mesh* getMesh(StringManager::StringID name, MeshType type);
    void release();
    static MeshManager* getInstance();
};

#endif
