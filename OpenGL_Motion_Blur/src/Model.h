#pragma once

#include <vector>
#include <cmath>
#include "glm/gtx/transform.hpp"

struct Vertex
{
	float position[3];
	float normal[3];
	float textCord[2];
};

enum class Surface
{
	Flat,
	Smooth
};

class Model
{
protected:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indecies;
	glm::mat4 m_ModelPositionMatrix;
	glm::mat4 m_ModelRotationMatrix;
	glm::mat4 m_ModelScaleMatrix;
	glm::mat4 m_ModelMatrix;
public:
	Model();
	~Model() {};

	void move(float x, float y, float z);
	void rotate(float x, float y, float z);
	void scale(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	inline std::vector<Vertex> getVertices() { return m_Vertices; }
	inline std::vector<unsigned int> getIndecies() { return m_Indecies; }
	inline glm::mat4 getModelMatrix() { return m_ModelPositionMatrix * m_ModelRotationMatrix * m_ModelScaleMatrix; }
	inline int getSize() { return m_Vertices.size() * sizeof(Vertex); };
};
