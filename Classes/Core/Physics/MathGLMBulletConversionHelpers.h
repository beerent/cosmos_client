
#ifndef MathGLMBulletConversionHelpers_h
#define MathGLMBulletConversionHelpers_h
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "btBulletDynamicsCommon.h"

void convertGLMtoBTMatrix(glm::mat4& glmMatrix, btMatrix3x3& btMatrix);
void convertBTTransformToGLM(const btTransform& transform, glm::mat4& glmMatrix);
void convertGLMtoBTTransform(const glm::mat4& glmMatrix, btTransform& transform);

#endif
