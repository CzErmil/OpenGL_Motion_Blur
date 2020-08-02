#include "Sphere.h"

Sphere::Sphere(int sectros, int stack, float radius, Surface surface) :
	Model(),
	m_Sectors(sectros),
	m_Stacks(stack),
	m_Radius(radius),
	m_Surface(surface)
{
	if (m_Surface == Surface::Smooth)
		calculateSmoothSphere();
	else if (m_Surface == Surface::Flat)
		calculateFlatSphere();
}

void Sphere::calculateSmoothSphere()
{
	float sectorAngle, stackAngle;
	float x, y, z, xz;
	float nx, ny, nz;
	float u, v;

	int k1, k2;

	float lenghtInv = 1.0f / m_Radius;

	float sectorStep = 2 * (float)M_PI / m_Sectors;
	float stackStep = (float)M_PI / m_Stacks;

	for (int i = 0; i <= m_Stacks; i++)
	{
		stackAngle = (float)M_PI_2 - (float)i * stackStep;

		xz = m_Radius * cosf(stackAngle);
		y = m_Radius * sinf(stackAngle);

		for (int j = 0; j <= m_Sectors; j++)
		{

			sectorAngle = j * sectorStep;

			x = xz * sin(sectorAngle);
			z = xz * cos(sectorAngle);

			nx = x * lenghtInv;
			ny = y * lenghtInv;
			nz = z * lenghtInv;

			u = (float)j / (m_Sectors);
			v = 1 - (float)i / (m_Stacks);

			m_Vertices.push_back({ x,y,z,nx,ny,nz,u,v });
		}
	}

	for (int i = 0; i < m_Stacks; i++)
	{
		k1 = i * (m_Sectors + 1);
		k2 = k1 + m_Sectors + 1;

		for (int j = 0; j < m_Sectors; j++, k1++, k2++)
		{
			if (i != 0)
			{
				m_Indecies.push_back(k1);
				m_Indecies.push_back(k2);
				m_Indecies.push_back(k1 + 1);

			}

			if (i != m_Stacks - 1)
			{
				m_Indecies.push_back(k1 + 1);
				m_Indecies.push_back(k2);
				m_Indecies.push_back(k2 + 1);
			}
		}
	}
}

void Sphere::calculateFlatSphere()
{
	std::vector<Vertex> tmpVertices;
	Vertex v1, v2, v3, v4;
	float sectorAngle, stackAngle;
	float x, y, z, xz;
	float u, v;

	int k1, k2;
	int index = 0;

	float sectorStep = 2 * (float)M_PI / m_Sectors;
	float stackStep = (float)M_PI / m_Stacks;

	for (int i = 0; i <= m_Stacks; i++)
	{
		stackAngle = (float)M_PI_2 - (float)i * stackStep;

		xz = m_Radius * cosf(stackAngle);
		y = m_Radius * sinf(stackAngle);

		for (int j = 0; j <= m_Sectors; j++)
		{
			sectorAngle = j * sectorStep;

			x = xz * sin(sectorAngle);
			z = xz * cos(sectorAngle);

			u = (float)j / (m_Sectors);
			v = 1 - (float)i / (m_Stacks);

			tmpVertices.push_back({ x,y,z,0,0,0,u,v });
		}
	}


	for (int i = 0; i < m_Stacks; i++)
	{
		k1 = i * (m_Sectors + 1);
		k2 = k1 + m_Sectors + 1;

		for (int j = 0; j < m_Sectors; j++, k1++, k2++)
		{
			v1 = tmpVertices[k1];
			v2 = tmpVertices[k2];
			v3 = tmpVertices[k1 + 1];
			v4 = tmpVertices[k2 + 1];

			if (i == 0)
			{
				m_Indecies.push_back(index++);
				m_Indecies.push_back(index++);
				m_Indecies.push_back(index++);

				calculateFlatSurface(v1, v2, v4);

				m_Vertices.push_back(v1);
				m_Vertices.push_back(v2);
				m_Vertices.push_back(v4);
			}
			else if (i == m_Stacks - 1)
			{
				m_Indecies.push_back(index++);
				m_Indecies.push_back(index++);
				m_Indecies.push_back(index++);

				calculateFlatSurface(v1, v2, v3);

				m_Vertices.push_back(v1);
				m_Vertices.push_back(v2);
				m_Vertices.push_back(v3);
			}
			else
			{
				m_Indecies.push_back(index);
				m_Indecies.push_back(index+1);
				m_Indecies.push_back(index+2);

				m_Indecies.push_back(index+2);
				m_Indecies.push_back(index+1);
				m_Indecies.push_back(index+3);

				calculateFlatSurface(v1, v2, v3);

				std::copy(v1.normal, v1.normal + 3, v4.normal);

				m_Vertices.push_back(v1);
				m_Vertices.push_back(v2);
				m_Vertices.push_back(v3);
				m_Vertices.push_back(v4);

				index += 4;
			}
		}
	}
}

void Sphere::calculateFlatSurface(Vertex& v1, Vertex& v2, Vertex& v3)
{
	glm::vec3 p1 = glm::vec3(v1.position[0], v1.position[1], v1.position[2]);
	glm::vec3 p2 = glm::vec3(v2.position[0], v2.position[1], v2.position[2]);
	glm::vec3 p3 = glm::vec3(v3.position[0], v3.position[1], v3.position[2]);
	glm::vec3 U = p2 - p1;
	glm::vec3 V = p3 - p1;
	glm::vec3 N;

	N = glm::cross(U, V);
	N = glm::normalize(N);

	v1.normal[0] = v2.normal[0] = v3.normal[0] = N.x;
	v1.normal[1] = v2.normal[1] = v3.normal[1] = N.y;
	v1.normal[2] = v2.normal[2] = v3.normal[2] = N.z;
}