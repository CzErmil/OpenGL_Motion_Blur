#pragma once
#include "Model.h"

class Sphere : public Model
{
private:
	int m_Sectors, m_Stacks;
	float m_Radius;
	Surface m_Surface;
public:
	Sphere(int sectros, int stack, float radius, Surface surface);
	~Sphere() {};

private:
	void calculateSmoothSphere();
	void calculateFlatSphere();
	void calculateFlatSurface(Vertex& v1, Vertex& v2, Vertex& v3);
};

