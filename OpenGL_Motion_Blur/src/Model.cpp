#include "Model.h"

Model::Model() :
	m_Postion(glm::vec3(0.0f)),
	m_ModelMatrix(1.0f),
	m_ModelPositionMatrix(glm::mat4(1)),
	m_ModelRotationMatrix(glm::mat4(1)),
	m_ModelScaleMatrix(glm::mat4(1))
{
	m_VertexBufferLayout.Push<float>(3);
	m_VertexBufferLayout.Push<float>(3);
	m_VertexBufferLayout.Push<float>(2);
}

void Model::move(float x, float y, float z)
{
	m_ModelPositionMatrix = glm::translate(m_ModelMatrix, glm::vec3(x, y, z));
}

void Model::rotate(float x, float y, float z)
{
	m_ModelRotationMatrix = glm::rotate(m_ModelRotationMatrix, glm::radians(x), glm::vec3(1, 0, 0));
	m_ModelRotationMatrix = glm::rotate(m_ModelRotationMatrix, glm::radians(y), glm::vec3(0, 1, 0));
	m_ModelRotationMatrix = glm::rotate(m_ModelRotationMatrix, glm::radians(z), glm::vec3(0, 0, 1));
}

void Model::scale(float x, float y, float z)
{
	m_ModelScaleMatrix = glm::scale(m_ModelScaleMatrix, glm::vec3(x, y, z));
}

void Model::setPosition(float x, float y, float z)
{
	m_Postion = glm::vec3(x, y, z);
	m_ModelPositionMatrix = glm::translate(glm::mat4(1.0f), m_Postion);
}

void Model::setRotation(float x, float y, float z)
{
	m_ModelRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::vec3(1, 0, 0));
	m_ModelRotationMatrix = glm::rotate(m_ModelRotationMatrix, glm::radians(y), glm::vec3(0, 1, 0));
	m_ModelRotationMatrix = glm::rotate(m_ModelRotationMatrix, glm::radians(z), glm::vec3(0, 0, 1));
}


void Model::setScale(float x, float y, float z)
{
	m_ModelScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}