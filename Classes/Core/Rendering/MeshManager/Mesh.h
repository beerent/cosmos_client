//
//  Mesh.h
//  Projectios1
//
//  Created by Kamil Konecko on 12/4/12.
//
//

#ifndef Projectios1_Mesh_h
#define Projectios1_Mesh_h
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Core/StringManager/StringManager.h"
#include "Core/OpenSource/Vector.hpp"
#include "Core/Memory/DynamicObject.h"

// Coordinate system
// The in game coordinate system does not match the coordinate system in blender.

// In game - Right-Handed Cartesian Coordinates
// up/down                     y axis +/-  Green
// right/left                  x axis +/-  Red
// back/forward                z axis -/+  Blue

// In blender - Right-Handed Z up coordinate system common to most CAD software
// up/down                     z axis +/-  Blue
// right/left                  x axis +/-  Red
// back/forward                y axis +/-  Green

class Mesh: public DynamicObject
{
public:
    static const StringManager::StringID CLASS_ID_MESH;
    
    Mesh():DynamicObject(CLASS_ID_MESH){};
    Mesh(StringManager::StringID classTypeID):DynamicObject(classTypeID){};
    
    struct VertexData {
        GLfloat m_vertice[3];
        GLfloat m_normal[3];
        GLfloat m_uv[2];
    };
    
    //friend class MeshManager;
    
protected:
    unsigned int m_vertexCount;
    GLushort *m_indexes;
    GLuint m_bufferHandle;
    GLuint m_iboHandle;
    unsigned int m_indexCount;
    StringManager::StringID m_name;
    VertexData *m_vertices;
   
public:
    virtual void init(StringManager::StringID name, Vector3<GLint> *triangleIndexes, unsigned int triangleCount, Vector3<GLfloat>*vertices,
                      unsigned int vertexCount, Vector2<GLfloat> *uves, unsigned int uvCount, Vector3<GLfloat>* normals, unsigned int normalCount);

    unsigned getIndexCount() const {return m_indexCount;};
    unsigned getVertexCount() const {return m_vertexCount;};
    VertexData* getVertexData()  {return m_vertices;};
	
	const VertexData* getVertexData() const { return m_vertices; };
	GLushort * getIndexes(){return m_indexes;};
	const GLushort * getIndexes() const { return m_indexes; };

    void release();
    GLuint getIBOHandle(){return m_iboHandle;};
    GLuint getBufferHandle(){return m_bufferHandle;};
};

#endif