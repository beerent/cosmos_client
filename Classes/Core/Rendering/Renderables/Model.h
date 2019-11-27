//
//  Model.h
//  Projectios1
//
//  Created by Kamil Konecko on 12/8/12.
//
//

#ifndef Projectios1_Model_h
#define Projectios1_Model_h

#include "RenderableObj.h"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Interface.hpp"
#include "Core/Properties/PropertyCollection.h"
#include "Core/Rendering/MeshManager/Mesh.h"
#include "Core/Rendering/TextureManager/Texture.h"
#include "Core/StringManager/StringManager.h"
#include "Core/Rendering/Shaders/ShaderManager.h"

class Material;

class Model : public RenderableObj
{
public:
    CONST_STRING_DEC(CLASS_ID_MODEL_RENDERABLE)
    CONST_STRING_DEC(MESH_NAME);
    CONST_STRING_DEC(TEXTURE_NAME);
	CONST_STRING_DEC(TEXTURE2_NAME);
	CONST_STRING_DEC(TEXTURE3_NAME);
	CONST_STRING_DEC(EMISIVE_TEXTURE_NAME);
    CONST_STRING_DEC(MESH_TYPE);
    CONST_STRING_DEC(MESH_TYPE_STATIC);
    CONST_STRING_DEC(MESH_TYPE_DYNAMIC);
    CONST_STRING_DEC(CULL_BACKFACE);
    CONST_STRING_DEC(MATERIAL);
    
    Model();
    const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView);
	const void onRender(Renderer& renderer, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView, const Material* material);

    void setResourceLoader(IResourceLoader *resourceLoader);
    virtual void _release();
    virtual const void init(PropertyCollection const &propertyCollection);
    const Mesh * getMesh() const {return m_pMesh;};
    Mesh * getMesh() { return m_pMesh; };

protected:
private:
    Mesh*                               m_pMesh;
    Texture*                            m_pTexture;
	Texture*                            m_pTexture2;
	Texture*                            m_pTexture3;
    Texture*                            m_pEmisiveTexture;
    IResourceLoader*                    m_pResourceLoader;
    bool                                mCullBackFace;
    Material*                           mMaterial;
};
#endif
