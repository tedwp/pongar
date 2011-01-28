#include "Marker.h"
#include "Game.h"

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
		for (int x=0; x<4; ++x)
		{
			for (int y=0; y<4; ++y)
			{
				resultTransposedMatrix[x*4+y] = m_position[y*4+x];
			}
		}
		//Ende - ÜdMi
	
	
		//glLoadTransposeMatrixf( resultMatrix );
		glLoadMatrixf( resultTransposedMatrix );
		glScalef(0.20, 0.20, 0.20);
	
		if (m_purpose == Game::PURPOSE_PLAYINGFIELD){
			Marker* m_paddle1 = Game::getMarkerByPurpose(Game::PURPOSE_PADDLE1);
			Marker* m_paddle2 = Game::getMarkerByPurpose(Game::PURPOSE_PADDLE2);
	
			// draw white rectangle
			glColor4f( 1.0, 1.0, 1.0, 1.0 );
			glRectf(-0.5, -0.8, 0.5, 0.8);
			
			if(m_paddle1 != NULL && m_paddle2 != NULL)
			{
				glTranslatef( 0.0, 0.0, -0.01 );
	
				// draw green rectangle
				glColor4f( 0.0, 1.0, 0.0, 1.0 );
				glRectf(-0.1 - m_paddle1->getOffset(), -0.75, 0.1 - m_paddle1->getOffset(), -0.65);

				// draw red rectangle
				glColor4f( 1.0, 0.0, 0.0, 1.0 );
				glRectf(-0.1 - m_paddle2->getOffset(), 0.65, 0.1 - m_paddle2->getOffset(), 0.75);
			}
		}
	}
}

void Marker::updatePosition(float position [16])
{
	m_positionChanged = false;
	
	for(unsigned i = 0; i < 16; i++)
	{
		if(position[i] != m_position[i])
		m_positionChanged = true;
	}
	
	if(m_positionChanged)
	{
		for( int i = 0; i < 16; i++ )
			m_position[ i ] = position[ i ];
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
int Marker::getPurpose(void)
{
	return m_purpose;
}
void Marker::setPurpose(int p)
{
	m_purpose = p;
}

bool Marker::hasPositionChanged(void)
{
	return m_positionChanged;
}

void Marker::setOffset(float offset){
	m_offset = offset;
}
float Marker::getOffset(){
	return m_offset;
}