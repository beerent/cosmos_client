//
//  DynamicMesh.h
//  Projectios1
//
//  Created by Kamil Konecko on 4/22/13.
//
//

#ifndef Projectios1_DynamicMesh_h
#define Projectios1_DynamicMesh_h

#include "Core/Rendering/MeshManager/Mesh.h"


class DynamicMesh : public Mesh
{
public:
    static const StringManager::StringID CLASS_ID_DYNAMIC_MESH;
    
    DynamicMesh():Mesh(CLASS_ID_DYNAMIC_MESH){};
    virtual void updateVertexData();
    
    virtual void init(StringManager::StringID name, Vector3<GLint> *triangleIndexes, unsigned int triangleCount, Vector3<GLfloat>*vertices, unsigned int vertexCount, Vector2<GLfloat> *uves, unsigned int uvCount);
    
};

#endif
