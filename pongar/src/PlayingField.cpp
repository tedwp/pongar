#include "PlayingField.h"

PlayingField::PlayingField(void)
{
	m_leftPaddle = NULL;
	m_rightPaddle = NULL;
	
	m_color.red = 255;
	m_color.green = 0;
	m_color.blue = 255;
	m_color.alpha = 255;


	ballSpeed = BALL_SPEED_INIT;

	//init random angle (between 30 and -30 deg)
	//0 deg = ball goes straight up
	srand ((unsigned) getTimeSinceStart());
	ballAngle = (float) (rand() % 45 + 30);
	if ((int)ballAngle % 2==0) ballAngle = 180 - ballAngle;

	ballPosition.first = 0.0;
	ballPosition.second = 0.0;

	
}


PlayingField::~PlayingField(void)
{
	if(m_leftPaddle != NULL)
		delete	m_leftPaddle;
	if(m_rightPaddle != NULL)
		delete m_rightPaddle;
}

PlayingField& PlayingField::getInstance()
{
	static PlayingField p_instance;
	return p_instance;
}

void PlayingField::render()
{
	if(getCorrespondingMarker() != NULL)
	{
		float* m_position = getCorrespondingMarker()->getPosition();
		float resultTransposedMatrix[16];
		transposeMatrix(m_position, resultTransposedMatrix);
	
		glLoadMatrixf( resultTransposedMatrix );
		glScalef(0.20f, 0.20f, 0.20f);
		// draw white rectangle
		glColor4f( m_color.red, m_color.green, m_color.blue, m_color.alpha);
		glRectf(-PLAYINGFIELD_HEIGHT/2, -PLAYINGFIELD_WIDTH/2, PLAYINGFIELD_HEIGHT/2, PLAYINGFIELD_WIDTH/2);
	}
	/*

	Marker* m_paddle1 = Game::getMarkerByPurpose(Game::PURPOSE_PADDLE1);
	Marker* m_paddle2 = Game::getMarkerByPurpose(Game::PURPOSE_PADDLE2);
	

	if(false && m_paddle1 != NULL && m_paddle2 != NULL)
	{
			
		float paddle2YStart =  PADDLE_LENGTH/2 - m_paddle2->getOffset();
		float paddle2YEnd = -PADDLE_LENGTH/2 - m_paddle2->getOffset();
		// draw red rectangle
		glColor4f( 1.0, 0.0, 0.0, 1.0 );
		if (paddle2YEnd+PADDLE_LENGTH > PLAYINGFIELD_HEIGHT/2) {
			paddle2YEnd = PLAYINGFIELD_HEIGHT/2;
			paddle2YStart = paddle2YEnd - PADDLE_LENGTH;
		}
		if (paddle2YStart-PADDLE_LENGTH < -PLAYINGFIELD_HEIGHT/2) {
			paddle2YStart = -PLAYINGFIELD_HEIGHT/2;
			paddle2YEnd = paddle2YStart + PADDLE_LENGTH;
		}
		glRectf(paddle2YEnd, 0.75f, paddle2YStart, 0.75f+PADDLE_WIDTH);



		if (Game::getInstance().getGameStage() == Game::STAGE_RUNNING){
			//computeBallPosition(paddle1YStart, paddle1YEnd, paddle2YStart, paddle2YEnd);
		}
		glTranslatef( ballPosition.first, ballPosition.second, 0.0 );
		glColor3f(0.0, 0.0, 0.0);
		drawCircle(BALL_RADIUS);

	}*/

	
	if( m_leftPaddle != NULL )
		m_leftPaddle->render();

	if( m_rightPaddle != NULL)
		m_rightPaddle->render();

}

void PlayingField::computeBallPosition(float paddle1Start, float paddle1End, float paddle2Start, float paddle2End)
{
	float degInRad=ballAngle*3.14159f/180;
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

	if (xCollision) {
		// GAME OVER
		Game::getInstance().setGameStage(Game::STAGE_OVER);
	} else if (pCollision) {
		ballPosition.first = ballPosition.first - ballVector.first;
		ballPosition.second = ballPosition.second - ballVector.second;
		ballAngle = 180-ballAngle;
		ballPosition.first = ballPosition.first + ballVector.first;
		ballPosition.second = ballPosition.second + ballVector.second;

		//increase ball speed
		ballSpeed = ballSpeed * BALL_SPEED_INCREASE_FACTOR;
	} else if (yCollision) {
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
		float degInRad=i*3.14159f/180;
		glVertex2f(cos(degInRad)*r, sin(degInRad)*r);
	}
	glEnd();
}

void PlayingField::transposeMatrix(float* src, float* dst){
	for (int x=0; x<4; ++x){
		for (int y=0; y<4; ++y){
			dst[x*4+y] = src[y*4+x];
		}
	}
}

void PlayingField::setCorrespondingMarker(Marker* marker)
{
	m_correspondingMarker = marker;
}

Marker* PlayingField::getCorrespondingMarker(void)
{
	return m_correspondingMarker;
}
void PlayingField::updatePaddlePositions(void)
{
	m_leftPaddle->updatePositionFromMarker();
	m_rightPaddle->updatePositionFromMarker();
}
Paddle* PlayingField::spawnPaddle(bool isLeft)
{
	if(m_leftPaddle == NULL && isLeft)
	{
		setPaddle(new Paddle(), true);
		return m_leftPaddle;
	}
	else if(m_rightPaddle == NULL && !isLeft)
	{
		setPaddle(new Paddle(), false);
		return m_rightPaddle;
	}
	return NULL;
}
void PlayingField::setPaddle(Paddle* paddle, bool isLeft)
{
	paddle->setLeft(isLeft);
	paddle->setPlayingField(this);
	if(isLeft)
		m_leftPaddle = paddle;
	else
		m_rightPaddle = paddle;
}


Color PlayingField::getColor(void)
{
	return m_color;
}
void PlayingField::setColor(Color& c)
{
	m_color = c;
}
void PlayingField::setColor(float r, float g, float b, float a)
{
	m_color.red = r;
	m_color.green = g;
	m_color.blue = b;
	m_color.alpha = a;
}