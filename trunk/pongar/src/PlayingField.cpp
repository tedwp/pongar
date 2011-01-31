#include "PlayingField.h"
#include "conf.h"

PlayingField::PlayingField(void)
{
	ballSpeed = BALL_SPEED_INIT;

	//init random angle (between 30 and -30 deg)
	//0 deg = ball goes straight up
	srand (time(NULL));
	ballAngle = rand() % 45 + 30;
	if ((int)ballAngle % 2==0) ballAngle = 180 - ballAngle;

	ballPosition.first = 0.0;
	ballPosition.second = 0.0;
}


PlayingField::~PlayingField(void)
{
}

PlayingField& PlayingField::getInstance()
{
	static PlayingField p_instance;
	return p_instance;
}

void PlayingField::render()
{
	Marker* m_PlayingField = Game::getMarkerByPurpose(Game::PURPOSE_PLAYINGFIELD);
	if(m_PlayingField!=NULL && m_PlayingField->hasPositionChanged())
	{
		if (m_PlayingField->getPurpose() == Game::PURPOSE_PLAYINGFIELD){
			float* m_position = m_PlayingField->getPosition();
			float resultTransposedMatrix[16];
			for (int x=0; x<4; ++x)
			{
				for (int y=0; y<4; ++y)
				{
					resultTransposedMatrix[x*4+y] = m_position[y*4+x];
				}
			}
	
			glLoadMatrixf( resultTransposedMatrix );
			glScalef(0.20, 0.20, 0.20);

			Marker* m_paddle1 = Game::getMarkerByPurpose(Game::PURPOSE_PADDLE1);
			Marker* m_paddle2 = Game::getMarkerByPurpose(Game::PURPOSE_PADDLE2);
	
			// draw white rectangle
			glColor4f( 1.0, 1.0, 1.0, 1.0 );
			glRectf(-PLAYINGFIELD_HEIGHT/2, -PLAYINGFIELD_WIDTH/2, PLAYINGFIELD_HEIGHT/2, PLAYINGFIELD_WIDTH/2);
			
			if(m_paddle1 != NULL && m_paddle2 != NULL)
			{
				glTranslatef( 0.0, 0.0, -0.01 );
	
				// green rectangle
				float paddle1YStart =  PADDLE_LENGTH/2 - m_paddle1->getOffset();
				float paddle1YEnd = -PADDLE_LENGTH/2 - m_paddle1->getOffset();
				// draw green rectangle
				glColor4f( 0.0, 1.0, 0.0, 1.0 );
				if (paddle1YEnd+PADDLE_LENGTH > PLAYINGFIELD_HEIGHT/2) {
					paddle1YEnd = PLAYINGFIELD_HEIGHT/2;
					paddle1YStart = paddle1YEnd - PADDLE_LENGTH;
				}
				if (paddle1YStart-PADDLE_LENGTH < -PLAYINGFIELD_HEIGHT/2) {
					paddle1YStart = -PLAYINGFIELD_HEIGHT/2;
					paddle1YEnd = paddle1YStart + PADDLE_LENGTH;
				}
				glRectf(paddle1YEnd, -0.78, paddle1YStart, -0.75);
				// red rectangle
				float paddle2YStart =  PADDLE_LENGTH/2 - m_paddle2->getOffset();
		        float paddle2YEnd = -PADDLE_LENGTH/2 - m_paddle2->getOffset();
				// draw green rectangle
				glColor4f( 1.0, 0.0, 0.0, 1.0 );
				if (paddle2YEnd+PADDLE_LENGTH > PLAYINGFIELD_HEIGHT/2) {
					paddle2YEnd = PLAYINGFIELD_HEIGHT/2;
					paddle2YStart = paddle2YEnd - PADDLE_LENGTH;
				}
				if (paddle2YStart-PADDLE_LENGTH < -PLAYINGFIELD_HEIGHT/2) {
					paddle2YStart = -PLAYINGFIELD_HEIGHT/2;
					paddle2YEnd = paddle2YStart + PADDLE_LENGTH;
				}
				glRectf(paddle2YEnd, 0.75, paddle2YStart, 0.78);

				computeBallPosition(paddle1YStart, paddle1YEnd, paddle2YStart, paddle2YEnd);
				glTranslatef( ballPosition.first, ballPosition.second, 0.0 );
				glColor3f(0.0, 0.0, 0.0);
				drawCircle(BALL_RADIUS);

			}
		}
	}
}

void PlayingField::computeBallPosition(float paddle1Start, float paddle1End, float paddle2Start, float paddle2End)
{
	float degInRad=ballAngle*3.14159/180;
	ballVector.first = ballSpeed * sin(degInRad);
	ballVector.second = ballSpeed * cos(degInRad);

	ballPosition.first = ballPosition.first + ballVector.first;
	ballPosition.second = ballPosition.second + ballVector.second;

	bool xCollision = false;
	bool yCollision = false;
	bool pCollision = false;
	if (ballPosition.second+BALL_RADIUS > PLAYINGFIELD_WIDTH/2) xCollision = true;
	if (ballPosition.second-BALL_RADIUS < -PLAYINGFIELD_WIDTH/2) xCollision = true;
	if (ballPosition.first+BALL_RADIUS > PLAYINGFIELD_HEIGHT/2) yCollision = true;
	if (ballPosition.first-BALL_RADIUS < -PLAYINGFIELD_HEIGHT/2) yCollision = true;
	if (!xCollision && ballPosition.second-BALL_RADIUS <= -0.75 && ballPosition.first<=paddle1Start && ballPosition.first>=paddle1End) pCollision = true;
	if (!xCollision && ballPosition.second+BALL_RADIUS >= 0.75 && ballPosition.first<=paddle2Start && ballPosition.first>=paddle2End) pCollision = true;

	if (xCollision)
	{
		// GAME OVER
	}
	if (pCollision)
	{
		ballPosition.first = ballPosition.first - ballVector.first;
		ballPosition.second = ballPosition.second - ballVector.second;
		ballAngle = 180-ballAngle;
		ballPosition.first = ballPosition.first + ballVector.first;
		ballPosition.second = ballPosition.second + ballVector.second;

		//increase ball speed
		ballSpeed = ballSpeed * BALL_SPEED_INCREASE_FACTOR;
	}
	if (yCollision)
	{
		ballPosition.first = ballPosition.first - ballVector.first;
		ballPosition.second = ballPosition.second - ballVector.second;
		ballAngle = 360-ballAngle;
		ballPosition.first = ballPosition.first + ballVector.first;
		ballPosition.second = ballPosition.second + ballVector.second;
	}
}

void PlayingField::drawCircle(float r)
{
	glBegin(GL_TRIANGLE_FAN);
  
	for (int i=0; i<360; ++i)
	{
		float degInRad=i*3.14159/180;
		glVertex2f(cos(degInRad)*r, sin(degInRad)*r);
	}
	glEnd();
}