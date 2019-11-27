//
//  Material.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/14/15.
//
//

#ifndef __Projectios2__Material__
#define __Projectios2__Material__
#include "Core/Memory/DynamicObject.h"
#include "Core/StringManager/StringManager.h"
#include "Core/Rendering/Shaders/ShaderManager.h"

#include <stdio.h>
class PropertyCollection;


class Material : public DynamicObject
{
public:
    friend class MaterialManager;

	CONST_STRING_DEC(CLASS_ID_MATERIAL)
		CONST_STRING_DEC(SHINYNESS)
		CONST_STRING_DEC(SHADER)
		CONST_STRING_DEC(NEXT_PASS_MATERIAL)
		CONST_STRING_DEC(ATTRIBUTE_SHININESS)
		CONST_STRING_DEC(EMISIVE)
		CONST_STRING_DEC(ATTRIBUTE_EYE_POSITION)
		CONST_STRING_DEC(ATTRIBUTE_NORMAL)
		CONST_STRING_DEC(ATTRIBUTE_COORDINATE_3D)
		CONST_STRING_DEC(ATTRIBUTE_TRANSFORM)
		CONST_STRING_DEC(UNIFORM_MODEL_VIEW_MATRIX)
		CONST_STRING_DEC(UNIFORM_LIGHT_POSITION)
		CONST_STRING_DEC(UNIFORM_DELTA_TIME)
		CONST_STRING_DEC(ATTRIBUTE_TEXTURE)
		CONST_STRING_DEC(ATTRIBUTE_TEXTURE_2)
		CONST_STRING_DEC(ATTRIBUTE_TEXTURE_3)
		CONST_STRING_DEC(ATTRIBUTE_TEXTURE_COORDINATE)
		CONST_STRING_DEC(ATTRIBUTE_NORMAL_MATRIX)
		CONST_STRING_DEC(ATTRIBUTE_AMBIENT_COLOR)
		CONST_STRING_DEC(ATTRIBUTE_DIFFUSE_COLOR)
		CONST_STRING_DEC(ATTRIBUTE_SPECULAR_COLOR)
		CONST_STRING_DEC(GL_CW_ENABLED)
		CONST_STRING_DEC(GL_CCW_ENABLED)

		Material() :DynamicObject(CLASS_ID_MATERIAL), mEmisive(false), mHasAttributeShininess(false), mShinyness(0),
		mHasAttributeEyePosition(false), mHasUniformModelViewMatrix(false), mHasAttributeTransform(false),
		mHasAttributeCoordinate3d(false), mHasAttributeTexture(false), mHasAttributeTexture2(false), mHasAttributeTexture3(false), 
		mHasAttributeTextureCoordinate(false),
		mHasAttributeNormalMatrix(false), mHasUniformLightPosition(false), mHasAttributeAmbientColor(false),
		mHasAttributeDiffuseColor(false), mHasAttributeSpecularColor(false), mHasUniformDeltaTime(false),
		mShaderName(StringManager::UNDEFINED), mNextPassMaterial(StringManager::UNDEFINED),
		mGL_CW(false), mGL_CCW(false){};
    
    long getShinyness()const    {return mShinyness;}
    bool hasAttributeShininess()const     {return mHasAttributeShininess;}
    bool isEmisive()const       {return mEmisive;}
	bool hasAttributeEyePosition()const { return mHasAttributeEyePosition; }
	bool hasUniformModelViewMatrix()const { return mHasUniformModelViewMatrix; }
	bool hasUniformLightPosition()const { return mHasUniformLightPosition; }
	bool hasUniformDeltaTime()const { return mHasUniformDeltaTime; }

	bool hasAttributeTexture() const { return mHasAttributeTexture; }
	bool hasAttributeTexture2() const { return mHasAttributeTexture2; }
	bool hasAttributeTexture3() const { return mHasAttributeTexture3; }

	bool hasAttributeTextureCoordinate() const { return mHasAttributeTextureCoordinate; }
	bool hasAttributeNormal() const { return mHasAttributeNormal; }
	bool hasAttributeCoordinate3d() const { return mHasAttributeCoordinate3d; }
	bool hasAttributeTransform() const { return mHasAttributeTransform; }
	bool hasAttributeNormalMatrix() const { return mHasAttributeNormalMatrix; }
	bool hasAttributeAmbientColor() const { return mHasAttributeAmbientColor; }
	bool hasAttributeDiffuseColor() const { return mHasAttributeDiffuseColor; }
	bool hasAttributeSpecularColor() const { return mHasAttributeSpecularColor; }
	bool gl_CW() const { return mGL_CW; }
	bool gl_CCW() const { return mGL_CCW; }

    STRING_ID getShaderName()const {return mShaderName;}
	STRING_ID getNextPassMaterial()const { return mNextPassMaterial; }
    
protected:
    virtual void init(PropertyCollection const& propertyCollection);

private:
    long                                   mShinyness;
    STRING_ID						       mShaderName;
	STRING_ID							   mNextPassMaterial;
    bool                                   mHasAttributeShininess;
    bool                                   mEmisive;
	bool								   mHasAttributeEyePosition;
	bool								   mHasUniformModelViewMatrix;
	bool								   mHasUniformLightPosition;
	bool								   mHasAttributeTexture;
	bool								   mHasAttributeTexture2;
	bool								   mHasAttributeTexture3;
	bool								   mHasAttributeTextureCoordinate;
	bool								   mHasAttributeNormal;
	bool								   mHasAttributeCoordinate3d;
	bool								   mHasAttributeTransform;
	bool								   mHasAttributeNormalMatrix;
	bool								   mHasAttributeAmbientColor;
	bool								   mHasAttributeDiffuseColor;
	bool								   mHasAttributeSpecularColor;
	bool								   mHasUniformDeltaTime;
	bool								   mGL_CW;
	bool								   mGL_CCW;

};


#endif /* defined(__Projectios2__Material__) */
