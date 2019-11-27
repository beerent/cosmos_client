//
//  WaterPlaneEntity.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 4/22/13.
//
//

#include "Core/Entity/Entities/WaterPlaneEntity.h"
#include "Core/Entity/Entities/Camera.h"
#include "IEngine.h"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Core/Rendering/Renderables/Model.h"
#include "Core/Rendering/MeshManager/DynamicMesh.h"
#include "Core/Rendering/Renderer.h"

const StringManager::StringID WaterPlaneEntity::CLASS_ID_WATER_PLANE_ENTITY = StringManager::getIDForString("CLASS_ID_WATER_PLANE_ENTITY");

const glm::mat4& WaterPlaneEntity::getTransformation()
{
    IEngine *engine = IEngine::getEngine();
    Camera *cam = engine->getActiveCamera();
    m_renderPosition = cam->getPosition();
    m_renderPosition.y = getPosition().y;
    
    
    //std::cout << "Water Plane Entity r position x " << m_renderPosition.x << " y " << m_renderPosition.y << "z " << m_renderPosition.z <<std::endl;

    //std::cout << "Water Plane Entity a position x " << getPosition().x << " y " << getPosition().y << "z " << getPosition().z <<std::endl;

    
    m_transformationMatrixValid = false;
    validateTransformationMatrix();

    return m_transformation;
}

void WaterPlaneEntity::onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView)
{
    if(renderable.getClassID() == Model::CLASS_ID_MODEL_RENDERABLE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        Model *model = static_cast<Model*>(m_renderable);
        DynamicMesh *mesh = static_cast<DynamicMesh*>(model->getMesh());
        Mesh::VertexData* vertexData = mesh->getVertexData();
        
        GLfloat x = m_renderPosition.x / (m_scaleVec.x * 2.0f);
        GLfloat y = -m_renderPosition.z / (m_scaleVec.y * 2.0f);
        GLfloat scale = 0.01f;//m_scaleVec.x;
        
        vertexData[0].m_uv[0] = (0.0f + x) / scale;
        vertexData[0].m_uv[1] = (0.0f + y) / scale;
        
        vertexData[1].m_uv[0] = (1.0f + x) / scale;
        vertexData[1].m_uv[1] = (0.0f + y) / scale;
        
        vertexData[2].m_uv[0] = (1.0f + x) / scale;
        vertexData[2].m_uv[1] = (1.0f + y) / scale;
        
        vertexData[3].m_uv[0] = (0.0f + x) / scale;
        vertexData[3].m_uv[1] = (0.0f + y) / scale;
        
        vertexData[4].m_uv[0] = (1.0f + x) / scale;
        vertexData[4].m_uv[1] = (1.0f + y) / scale;
        
        vertexData[5].m_uv[0] = (0.0f + x) / scale;
        vertexData[5].m_uv[1] = (1.0f + y) / scale;
        
        mesh->updateVertexData();

        renderable.onRender(renderer, modelViewProjectionMatrix, modelView);
    }
    else
    {
        Entity::onRender(renderer, renderable, modelViewProjectionMatrix, modelView);
    }
}

void WaterPlaneEntity::onBeginRender(Renderer& renderer)
{
    renderer.pushTransformation(getTransformation());
    //glDisable(GL_DEPTH_WRITEMASK);
    renderer.addRenderable(this, m_renderable);
    //glEnable(GL_DEPTH_WRITEMASK);
    renderer.popTransformation();
}

void WaterPlaneEntity::validateTransformationMatrix()
{
    if(m_transformationMatrixValid == false)
    {
        m_transformationMatrixValid = true;
        m_transformation = glm::translate(glm::mat4(1.0f), m_renderPosition) * glm::toMat4(m_rotation) * m_scale;
    }
}