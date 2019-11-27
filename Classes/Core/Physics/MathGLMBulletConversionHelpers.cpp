#include "Core/Physics/MathGLMBulletConversionHelpers.h"

void convertGLMtoBTMatrix(glm::mat4& glmMatrix, btMatrix3x3& btMatrix)
{
}

void convertBTTransformToGLM(const btTransform& transform, glm::mat4& glmMatrix)
{
	btScalar matrix[16] = { 0.f };
	transform.getOpenGLMatrix(matrix);

	for (int i = 0; i < 4; ++i)
	{
		glmMatrix[i][0] = matrix[0 + i * 4];
		glmMatrix[i][1] = matrix[1 + i * 4];
		glmMatrix[i][2] = matrix[2 + i * 4];
		glmMatrix[i][3] = matrix[3 + i * 4];
	}
}

void convertGLMtoBTTransform(const glm::mat4& glmMatrix, btTransform& transform)
{
	btScalar matrix[16] = { 0.f };

	for (int i = 0; i < 4; ++i)
	{
		matrix[0 + i * 4] = glmMatrix[i][0];
		matrix[1 + i * 4] = glmMatrix[i][1];
		matrix[2 + i * 4] = glmMatrix[i][2];
		matrix[3 + i * 4] = glmMatrix[i][3];
	}

	transform.setFromOpenGLMatrix(matrix);
}