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
      float playingFieldWidth = 1.6;
      float playingFieldHeight = 1.0;
			glRectf(-playingFieldHeight/2, -playingFieldWidth/2, playingFieldHeight/2, playingFieldWidth/2);
			
			if(m_paddle1 != NULL && m_paddle2 != NULL)
			{
				glTranslatef( 0.0, 0.0, -0.01 );
	
        float paddleLength = 0.2;

        // green rectangle
        float paddle1YStart =  0.1 - m_paddle1->getOffset();
        float paddle1YEnd = -0.1 - m_paddle1->getOffset();
				// draw green rectangle
				glColor4f( 0.0, 1.0, 0.0, 1.0 );
        if (paddle1YEnd+paddleLength>playingFieldHeight/2) {
          paddle1YEnd = playingFieldHeight/2;
          paddle1YStart = paddle1YEnd - paddleLength;
        }
        if (paddle1YStart-paddleLength<-playingFieldHeight/2) {
            paddle1YStart = -playingFieldHeight/2;
            paddle1YEnd = paddle1YStart + paddleLength;
        }
				glRectf(paddle1YEnd, -0.75, paddle1YStart, -0.65);
        // red rectangle
				float paddle2YStart =  0.1 - m_paddle2->getOffset();
        float paddle2YEnd = -0.1 - m_paddle2->getOffset();
				// draw green rectangle
				glColor4f( 1.0, 0.0, 0.0, 1.0 );
        if (paddle2YEnd+paddleLength>playingFieldHeight/2) {
          paddle2YEnd = playingFieldHeight/2;
          paddle2YStart = paddle2YEnd - paddleLength;
        }
        if (paddle2YStart-paddleLength<-playingFieldHeight/2) {
            paddle2YStart = -playingFieldHeight/2;
            paddle2YEnd = paddle2YStart + paddleLength;
        }
				glRectf(paddle2YEnd, 0.65, paddle2YStart, 0.75);

        glTranslatef( 0.0, 0.0, 0.0 );
        glColor3f(0.0, 0.0, 0.0);
        drawCircle(0.05);

			}
		}
	}
}

void Marker::drawCircle(float r)
{
  glBegin(GL_TRIANGLE_FAN);
  for (int i=0; i<360; ++i)
  {
    float degInRad=i*3.14159/180;
    glVertex2f(cos(degInRad)*r, sin(degInRad)*r);
  }
  glEnd();
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