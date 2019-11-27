//
//  Mesh.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 12/4/12.
//
//

#include "Core/Rendering/MeshManager/Mesh.h"
const StringManager::StringID Mesh::CLASS_ID_MESH = StringManager::getIDForString("CLASS_ID_MESH");

//KKonecko: This is not the most efficient way of loading meshes but lets go with this for now.
void Mesh::init(StringManager::StringID name, Vector3<GLint> *triangleIndexes, unsigned int triangleIndexCount, Vector3<GLfloat>*vertices,
                unsigned int vertexCount, Vector2<GLfloat> *uves, unsigned int uvCount, Vector3<GLfloat>* normals, unsigned int normalCount)
{
    m_indexes = new GLushort[triangleIndexCount];
    GLushort * pIndexes = m_indexes;
    m_vertices = new VertexData[triangleIndexCount];
    VertexData * pVertices = m_vertices;
 
    for(unsigned int i =0; i < triangleIndexCount; i++)
    {
        *pIndexes = i;
        pVertices->m_vertice[0] = (vertices[triangleIndexes->x]).x;
        pVertices->m_vertice[1] = (vertices[triangleIndexes->x]).y;
        pVertices->m_vertice[2] = (vertices[triangleIndexes->x]).z;
        pVertices->m_uv[0] = (uves[triangleIndexes->y]).x;
        pVertices->m_uv[1] = (uves[triangleIndexes->y]).y;

        if(normalCount)
        {
            pVertices->m_normal[0] = (normals[triangleIndexes->z]).x;
            pVertices->m_normal[1] = (normals[triangleIndexes->z]).y;
            pVertices->m_normal[2] = (normals[triangleIndexes->z]).z;
        }

        pVertices++;
        triangleIndexes++;
        pIndexes++;
    }
    
    m_vertexCount = triangleIndexCount;
    m_indexCount = triangleIndexCount;
    
    glGenBuffers(1, &m_bufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::VertexData) * getVertexCount(), getVertexData(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &m_iboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * getIndexCount(), getIndexes(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    DynamicObject::init();
}

void Mesh::release()
{
    glDeleteBuffers(1, &m_bufferHandle);
    delete[] m_vertices;
    delete[] m_indexes;
    DynamicObject::release();
}