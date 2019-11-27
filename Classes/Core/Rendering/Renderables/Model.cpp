//
//  Model.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 12/8/12.
//
//

#include "Core/Rendering/Renderables/Model.h"
#include <iostream>
#include <cstdio>
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/RenderableManager.h"

#include "Core/OpenSource/Vector.hpp"
#include "Core/Rendering/TextureManager/TextureManager.h"
#include "Core/Rendering/TextureManager/Texture.h"
#include "Core/Rendering/MeshManager/MeshManager.h"
#include "Core/Rendering/MaterialManager/MaterialManager.h"
#include "Core/Rendering/LightingManager/LightingManager.h"
#include "IEngine.h"

CONST_STRING_DEF(Model, CLASS_ID_MODEL_RENDERABLE)
CONST_STRING_DEF(Model, MESH_NAME);
CONST_STRING_DEF(Model, TEXTURE_NAME);
CONST_STRING_DEF(Model, TEXTURE2_NAME);
CONST_STRING_DEF(Model, TEXTURE3_NAME);

CONST_STRING_DEF(Model, EMISIVE_TEXTURE_NAME);
CONST_STRING_DEF(Model, MESH_TYPE);
CONST_STRING_DEF_WS(Model, MESH_TYPE_STATIC, static);
CONST_STRING_DEF_WS(Model, MESH_TYPE_DYNAMIC, dynamic);
CONST_STRING_DEF(Model, CULL_BACKFACE);
CONST_STRING_DEF(Model, MATERIAL);

struct VertexData {
    GLfloat m_vertice[3];
    GLfloat m_color[3];
};

Model::Model():m_pMesh(NULL), m_pTexture(NULL), m_pResourceLoader(NULL), mCullBackFace(false), mMaterial(NULL), RenderableObj(CLASS_ID_MODEL_RENDERABLE)
            , m_pEmisiveTexture(NULL){};

const void Model::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView)
{
	onRender(renderer, modelViewProjectionMatrix, modelView, mMaterial);
}

const void Model::onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView, const Material* material)
{
	bool validate = true;

    RenderableObj::onRender(renderer, modelViewProjectionMatrix, modelView);
    
    GLuint program = 0;

    if(material == NULL)
    {
		material = MaterialManager::getInstance()->getMaterial(StringManager::getIDForString("TexturedMaterial"));
    }

	program = ShaderManager::getProgram(material->getShaderName());
		
    const char* attributeTextureCoordinateName = "mTexCord";
    const char* attributeTextureName = "mytexture";
	const char* attributeTexture2Name = "mytexture2";
	const char* attributeTexture3Name = "mytexture3";

	GLuint attributeTextureCoordinate = 0;
	GLuint attributeTexture = 0;
	GLuint attributeTexture2 = 0;
	GLuint attributeTexture3 = 0;

	if (material->hasAttributeTextureCoordinate())
	{
		attributeTextureCoordinate = glGetAttribLocation(program, attributeTextureCoordinateName);

		if (attributeTextureCoordinate == -1) {
			std::cout << "Could not bind attribute %s\n" << stderr << " name " << attributeTextureCoordinateName << std::endl;
			return;
		}
	}

	glUseProgram(program);

	if (material->hasAttributeTexture())
	{
		attributeTexture = glGetUniformLocation(program, attributeTextureName);

		if (attributeTexture == -1) {
			std::cout << "Could not bind attribute %s\n" << stderr << " name " << attributeTextureName << std::endl;
			return;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pTexture->getTextureGLID());
		glUniform1i(attributeTexture, 0);
	}

	bool found = false;
	if (material->getShaderName() == StringManager::getIDForString("WATER_SHADER"))
	{
		found = true;
	}

	if (material->hasAttributeTexture2())
	{
		attributeTexture2 = glGetUniformLocation(program, attributeTexture2Name);

		if (attributeTexture2 == -1) {
			std::cout << "Could not bind attribute %s\n" << stderr << " name " << attributeTexture2Name << std::endl;
			return;
		}

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_pTexture2->getTextureGLID());
		glUniform1i(attributeTexture2, 1);
	}

	if (material->hasAttributeTexture3())
	{
		attributeTexture3 = glGetUniformLocation(program, attributeTexture3Name);

		if (attributeTexture3 == -1) {
			std::cout << "Could not bind attribute %s\n" << stderr << " name " << attributeTexture3Name << std::endl;
			return;
		}

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_pTexture3->getTextureGLID());
		glUniform1i(attributeTexture3, 2);
	}

    glBindBuffer(GL_ARRAY_BUFFER, m_pMesh->getBufferHandle());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pMesh->getIBOHandle());
    
    glEnable(GL_BLEND);
    if(getRenderGroup() == TRANSPARENT_STR)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    else
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }
    
	bool cullBackFace = false;

	if(material->gl_CW())
	{
		cullBackFace = true;
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}
	else if (material->gl_CCW())
	{
		cullBackFace = true;
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);

	}
    else if(mCullBackFace)
    {
		cullBackFace = true;
		glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    
	validate == true ? Renderer::validate() : 0;

	GLuint attributeCoord3d = -1;
	if (material->hasAttributeCoordinate3d())
	{
		const char* attribute_name = "coord3d";
		attributeCoord3d = glGetAttribLocation(program, attribute_name);
		if (attributeCoord3d == -1) {
			std::cout << "Could not bind attribute %s\n" << stderr << " name " << attribute_name << std::endl;
			return;
		}

		glEnableVertexAttribArray(attributeCoord3d);

		/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
		glVertexAttribPointer(
			attributeCoord3d, // attribute
			3,// number of elements per vertex, here (x,y)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			sizeof(Mesh::VertexData),             // no extra data between each position
			(GLvoid*)offsetof(struct Mesh::VertexData, m_vertice)                // pointer to the C array
		);

		validate == true ? Renderer::validate() : 0;
	}
    
	if (material->hasAttributeTextureCoordinate())
	{
		glEnableVertexAttribArray(attributeTextureCoordinate);
		glVertexAttribPointer(
			attributeTextureCoordinate, // attribute
			2,                 // number of elements per vertex, here (x,y)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			sizeof(Mesh::VertexData),                 // no extra data between each position
			(GLvoid*)offsetof(struct Mesh::VertexData, m_uv)                  // pointer to the C array
		);
	}

	if (material->hasAttributeTransform())
	{
		const char* attributeTransformName = "mMVP";
		GLuint attributeTransform = glGetUniformLocation(program, attributeTransformName);

		if (attributeTransform == -1) {
			std::cout << "Could not bind attribute %s\n" << stderr << " name " << attributeTransformName << std::endl;
			return;
		}

		glUniformMatrix4fv(attributeTransform, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
	}

	LightingManager* lighting = IEngine::getEngine()->getLightingManager();

	if (material->hasAttributeAmbientColor())
	{
		const char* attributeAmbientColorName = "ambientColor";
		GLuint attributeAmbientColor = glGetAttribLocation(program, attributeAmbientColorName);
		glVertexAttrib3fv(attributeAmbientColor, glm::value_ptr(lighting->getAmbientColor()));
		validate == true ? Renderer::validate() : 0;
	}

	if (material->hasAttributeDiffuseColor())
	{
		const char* attributeDiffuseColorName = "diffuseColor";
		GLuint attributeDiffuseColor = glGetAttribLocation(program, attributeDiffuseColorName);
		glVertexAttrib3fv(attributeDiffuseColor, glm::value_ptr(lighting->getDiffuseColor()));
		validate == true ? Renderer::validate() : 0;
	}

	if (material->hasAttributeSpecularColor())
	{
		const char* attributeSpecularColorName = "specularColor";
		GLuint attributeSpecularColor = glGetAttribLocation(program, attributeSpecularColorName);
		glVertexAttrib3fv(attributeSpecularColor, glm::value_ptr(lighting->getSpecularColor()));
		validate == true ? Renderer::validate() : 0;
	}

    if(material->hasAttributeShininess())
    {           
        const char* attributeShininessName = "shininess";
        GLuint attributeShininess = glGetAttribLocation(program, attributeShininessName);
        glVertexAttrib1f(attributeShininess, mMaterial->getShinyness());
		validate == true ? Renderer::validate() : 0;
	}

	GLuint attributeNormal = -1;
	if (material->hasAttributeNormal())
	{
		//This is optional, Ideally we would have some meta data that would make this check redundant
		const char* attributeNormalName = "mNormal";
		attributeNormal = glGetAttribLocation(program, attributeNormalName);
		glEnableVertexAttribArray(attributeNormal);
		glVertexAttribPointer(
			attributeNormal,   // attribute
			3,                 // number of elements per vertex, here (x,y)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			sizeof(Mesh::VertexData),                 // no extra data between each position
			(GLvoid*)offsetof(struct Mesh::VertexData, m_normal)                  // pointer to the C array
		);
	}

	if (material->hasUniformLightPosition())
	{
		const char* attributeLightPosName = "lightPos";
		GLuint uniformLightPos = glGetUniformLocation(program, attributeLightPosName);
		glUniform3fv(uniformLightPos, 1, glm::value_ptr(lighting->getInfinitLightSourceVector()));
		validate == true ? Renderer::validate() : 0;
	}

	if (material->hasUniformDeltaTime())
	{
		const char* attributeDeltaTimeName = "deltaTime";
		GLuint uniformDeltaTime = glGetUniformLocation(program, attributeDeltaTimeName);
		float loopDeltaTime = IEngine::getEngine()->getLoopingAccumulatedDeltaTime();
		glUniform1f(uniformDeltaTime, loopDeltaTime);
		validate == true ? Renderer::validate() : 0;
	}

	if (material->hasAttributeNormalMatrix())
	{
		const char* attributeNormalMatrixName = "normalMatrix";
		GLuint uniformNormalMatrix = glGetUniformLocation(program, attributeNormalMatrixName);
		glm::mat3 normalMatrix(modelView); // Why do this here instead of a shader?
										   //normalMatrix = glm::inverse(normalMatrix);
		glUniformMatrix3fv(uniformNormalMatrix, 1, 0, glm::value_ptr(normalMatrix));
		validate == true ? Renderer::validate() : 0;
	}
        
    if(material->isEmisive())
    {
        const char* attributeEmisiveTextureName = "myEmisivetexture";
        GLuint attributeEmisiveTexture = glGetUniformLocation(program, attributeEmisiveTextureName);
            
        if (attributeEmisiveTexture == -1) {
            std::cout<< "Could not bind attribute %s\n" << stderr << " name " << attributeEmisiveTexture << std::endl;
            return;
        }
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_pEmisiveTexture->getTextureGLID());
        glUniform1i(attributeEmisiveTexture,1);
    }

	if (material->hasAttributeEyePosition())
	{
		const char* attributeEyePositionName = "eyePos";
		GLuint uniformEyePosition = glGetUniformLocation(program, attributeEyePositionName);
		glUniform3fv(uniformEyePosition, 1, glm::value_ptr(renderer.getEyePosition()));
		validate == true ? Renderer::validate() : 0;
	}

	if (material->hasUniformModelViewMatrix())
	{
		const char* attributeModelViewName = "ModelView";
		GLuint uniformModelView = glGetUniformLocation(program, attributeModelViewName);
		glUniformMatrix4fv(uniformModelView, 1, 0, glm::value_ptr(modelView));
		validate == true ? Renderer::validate() : 0;
	}


    /* Push each element in buffer_vertices to the vertex shader */
    //    glDrawArrays(GL_TRIANGLES, 0, 24);
    GLint size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT,0);
    
	if (material->hasAttributeCoordinate3d())
	{
		glDisableVertexAttribArray(attributeCoord3d);
	}

	if (material->hasAttributeTextureCoordinate())
	{
		glDisableVertexAttribArray(attributeTextureCoordinate);
	}

	if (material->hasAttributeNormal())
	{
		glDisableVertexAttribArray(attributeNormal);
	}

	if (cullBackFace == true)
	{
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}
    
	validate == true ? Renderer::validate() : 0;

	STRING_ID nextPassMaterialName = material->getNextPassMaterial();
	if (nextPassMaterialName != StringManager::UNDEFINED)
	{
		const Material* nextMaterial = MaterialManager::getInstance()->getMaterial(nextPassMaterialName);
		onRender(renderer, modelViewProjectionMatrix, modelView, nextMaterial);
	}
}

void Model::_release()
{
    RenderableObj::_release();
}

void Model::setResourceLoader(IResourceLoader *resourceLoader)
{
    m_pResourceLoader = resourceLoader;
}

const void Model::init(PropertyCollection const &propertyCollection)
{
    RenderableObj::init(propertyCollection);
   
    STRING_ID textureName = StringManager::UNDEFINED;
	STRING_ID texture2Name = StringManager::UNDEFINED;
	STRING_ID texture3Name = StringManager::UNDEFINED;
    STRING_ID emisiveTextureName = StringManager::UNDEFINED;
    STRING_ID meshName;
    STRING_ID meshType = MESH_TYPE_STATIC;
    
    propertyCollection.getValueForProperty(TEXTURE_NAME,            textureName);
	propertyCollection.getValueForProperty(TEXTURE2_NAME, texture2Name);
	propertyCollection.getValueForProperty(TEXTURE3_NAME, texture3Name);

	propertyCollection.getValueForProperty(EMISIVE_TEXTURE_NAME,    emisiveTextureName);
    propertyCollection.getValueForProperty(MESH_NAME,               meshName);
    propertyCollection.getValueForProperty(MESH_TYPE,               meshType);
    propertyCollection.getValueForProperty(CULL_BACKFACE,           mCullBackFace);
    
    STRING_ID materialName = StringManager::UNDEFINED;
    propertyCollection.getValueForProperty(MATERIAL,           materialName);
    
    if(materialName != StringManager::UNDEFINED)
    {
        mMaterial = MaterialManager::getInstance()->getMaterial(materialName);
    }
    
    if( meshType == MESH_TYPE_DYNAMIC)
    {
        m_pMesh = MeshManager::getInstance()->getMesh(meshName, MeshManager::MESH_TYPE_DYNAMIC);
    }
    else
    {
        m_pMesh = MeshManager::getInstance()->getMesh(meshName, MeshManager::MESH_TYPE_STATIC);
    }
    
    TextureManager* textureManager = TextureManager::getInstance();
    m_pTexture = textureManager->getTexture(textureName);

	if (texture2Name != StringManager::UNDEFINED)
	{
		m_pTexture2 = textureManager->getTexture(texture2Name);
	}
	else
	{
		m_pTexture2 = NULL;
	}

	if (texture3Name != StringManager::UNDEFINED)
	{
		m_pTexture3 = textureManager->getTexture(texture3Name);
	}
	else
	{
		m_pTexture3 = NULL;
	}

	if(emisiveTextureName != StringManager::UNDEFINED)
    {
        m_pEmisiveTexture = textureManager->getTexture(emisiveTextureName);
    }
    else
    {
        m_pEmisiveTexture = NULL;
    }
    
}
