#include "Sphere.h"

Sphere::Sphere(int sectros, int stack, float radius, Surface surface) :
	Model(),
	m_Sectors(sectros),
	m_Stacks(stack),
	m_Radius(radius),
	m_Surface(surface)
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
		y = radius * sinf(stackAngle);

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
		if (i != 0)
		{
			m_Indecies.push_back(k1);
			m_Indecies.push_back(k2);
			m_Indecies.push_back(i * (m_Sectors + 1));

		}

		if (i != m_Stacks - 1)
		{
			m_Indecies.push_back(i * (m_Sectors + 1));
			m_Indecies.push_back(k2);
			m_Indecies.push_back((i + 1) * (m_Sectors + 1));
		}
	}
}