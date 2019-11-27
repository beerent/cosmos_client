//
//  Material.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 10/14/15.
//
//

#include "Material.h"
#include "Core/Properties/PropertyCollection.h"

CONST_STRING_DEF(Material, CLASS_ID_MATERIAL)
CONST_STRING_DEF(Material, SHINYNESS)
CONST_STRING_DEF(Material, SHADER)
CONST_STRING_DEF(Material, NEXT_PASS_MATERIAL)
CONST_STRING_DEF(Material, ATTRIBUTE_SHININESS)
CONST_STRING_DEF(Material, EMISIVE)
CONST_STRING_DEF(Material, ATTRIBUTE_EYE_POSITION)
CONST_STRING_DEF(Material, UNIFORM_MODEL_VIEW_MATRIX)
CONST_STRING_DEF(Material, ATTRIBUTE_TEXTURE)
CONST_STRING_DEF(Material, ATTRIBUTE_TEXTURE_2)
CONST_STRING_DEF(Material, ATTRIBUTE_TEXTURE_3)
CONST_STRING_DEF(Material, ATTRIBUTE_TEXTURE_COORDINATE)
CONST_STRING_DEF(Material, ATTRIBUTE_NORMAL)
CONST_STRING_DEF(Material, ATTRIBUTE_COORDINATE_3D)
CONST_STRING_DEF(Material, ATTRIBUTE_TRANSFORM)
CONST_STRING_DEF(Material, ATTRIBUTE_NORMAL_MATRIX)
CONST_STRING_DEF(Material, UNIFORM_LIGHT_POSITION)
CONST_STRING_DEF(Material, UNIFORM_DELTA_TIME)
CONST_STRING_DEF(Material, ATTRIBUTE_AMBIENT_COLOR)
CONST_STRING_DEF(Material, ATTRIBUTE_DIFFUSE_COLOR)
CONST_STRING_DEF(Material, ATTRIBUTE_SPECULAR_COLOR)
CONST_STRING_DEF(Material, GL_CW_ENABLED)
CONST_STRING_DEF(Material, GL_CCW_ENABLED)

void Material::init(PropertyCollection const& propertyCollection)
{    
    propertyCollection.getValueForProperty(SHINYNESS,     mShinyness);
    propertyCollection.getValueForProperty(SHADER,	      mShaderName);
    propertyCollection.getValueForProperty(ATTRIBUTE_SHININESS,      mHasAttributeShininess);
    propertyCollection.getValueForProperty(EMISIVE,       mEmisive);  
	propertyCollection.getValueForProperty(UNIFORM_MODEL_VIEW_MATRIX, mHasUniformModelViewMatrix);
	propertyCollection.getValueForProperty(ATTRIBUTE_EYE_POSITION, mHasAttributeEyePosition);
	propertyCollection.getValueForProperty(ATTRIBUTE_TEXTURE, mHasAttributeTexture);
	propertyCollection.getValueForProperty(ATTRIBUTE_TEXTURE_2, mHasAttributeTexture2);
	propertyCollection.getValueForProperty(ATTRIBUTE_TEXTURE_3, mHasAttributeTexture3);
	propertyCollection.getValueForProperty(ATTRIBUTE_TEXTURE_COORDINATE, mHasAttributeTextureCoordinate);
	propertyCollection.getValueForProperty(ATTRIBUTE_NORMAL, mHasAttributeNormal);
	propertyCollection.getValueForProperty(ATTRIBUTE_COORDINATE_3D, mHasAttributeCoordinate3d);
	propertyCollection.getValueForProperty(ATTRIBUTE_TRANSFORM, mHasAttributeTransform);
	propertyCollection.getValueForProperty(ATTRIBUTE_NORMAL_MATRIX, mHasAttributeNormalMatrix);
	propertyCollection.getValueForProperty(UNIFORM_LIGHT_POSITION, mHasUniformLightPosition);
	propertyCollection.getValueForProperty(ATTRIBUTE_AMBIENT_COLOR, mHasAttributeAmbientColor);
	propertyCollection.getValueForProperty(ATTRIBUTE_DIFFUSE_COLOR, mHasAttributeDiffuseColor);
	propertyCollection.getValueForProperty(ATTRIBUTE_SPECULAR_COLOR, mHasAttributeSpecularColor);
	propertyCollection.getValueForProperty(NEXT_PASS_MATERIAL, mNextPassMaterial);
	propertyCollection.getValueForProperty(GL_CW_ENABLED, mGL_CW);
	propertyCollection.getValueForProperty(GL_CCW_ENABLED, mGL_CCW);
	propertyCollection.getValueForProperty(UNIFORM_DELTA_TIME, mHasUniformDeltaTime);
}
 