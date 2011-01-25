#include "Marker.h"

#include "GL\glut.h"


Marker::Marker(int id)
{
	m_id = id;
	m_position = new float[16];
}


Marker::~Marker(void)
{
	delete(m_position);
}


void Marker::render(void)
{
	if(hasPositionChanged())
	{
		float resultTransposedMatrix[16];
		for (int x=0; x < 4; ++x)
		{
			for (int y=0; y < 4; ++y)
			{
				resultTransposedMatrix[x*4+y] = m_position[y*4+x];
			}
		}
	
		glLoadMatrixf( resultTransposedMatrix );
		glScalef(0.20, 0.20, 0.20);
	
		// draw white rectangle
	
		glColor4f( 1.0, 1.0, 1.0, 0.7 );
		glRectf(-0.5, -0.8, 0.5, 0.8);
	}
}

void Marker::updatePosition(float* position)
{
	m_positionChanged = false;
	
	for(unsigned i = 0; i < 16; i++)
	{
		m_positionChanged = true;
	}
	
	if(m_positionChanged)
	{
		m_position = position;
	}
}

float* Marker::getPosition(void)
{
	return m_position;
}

int Marker::getId(void)
{
	return m_id;

}

bool Marker::hasPositionChanged(void)
{
	return m_positionChanged;
}