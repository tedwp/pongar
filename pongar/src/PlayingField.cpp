#include "PlayingField.h"

PlayingField::PlayingField(void)
{
	m_leftPaddle = NULL;
	m_rightPaddle = NULL;
	
	m_color.red = 255;
	m_color.green = 0;
	m_color.blue = 255;
	m_color.alpha = 255;
}


PlayingField::~PlayingField(void)
{
	if(m_leftPaddle != NULL)
		delete	m_leftPaddle;

	if(m_rightPaddle != NULL)
		delete m_rightPaddle;

	if(m_ball  != NULL)
		delete m_ball;
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
		Graphics::transposeMatrix(m_position, resultTransposedMatrix);
	
		glLoadMatrixf( resultTransposedMatrix );
		glScalef(0.03f, 0.03f, 0.03f);
		// draw white rectangle
		glColor4f( m_color.red, m_color.green, m_color.blue, m_color.alpha);
		//glRectf(-PLAYINGFIELD_HEIGHT/2, -PLAYINGFIELD_WIDTH/2, PLAYINGFIELD_HEIGHT/2, PLAYINGFIELD_WIDTH/2);
		float x1 = -PLAYINGFIELD_HEIGHT/2;
		float y1 = -PLAYINGFIELD_WIDTH/2;
		float x2 = PLAYINGFIELD_HEIGHT/2;
		float y2=PLAYINGFIELD_WIDTH/2;
		glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x2, y1);
		glVertex2f(x2, y2);
		glVertex2f(x1, y2);
		glEnd();

	}
	
	if( m_leftPaddle != NULL )
		m_leftPaddle->render();

	if( m_rightPaddle != NULL)
		m_rightPaddle->render();

	if(m_ball != NULL)
		m_ball->render();

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


Ball* PlayingField::spawnBall(void)
{
	if(m_ball == NULL)
	{
		m_ball = new Ball();
		setBall(m_ball);
	}
	return m_ball;
}

void PlayingField::setBall(Ball* ball)
{	
	m_ball = ball;
	m_ball->setPlayingField(this);
}

Paddle* PlayingField::getLeftPaddle(void)
{
	return m_leftPaddle;
}
Paddle* PlayingField::getRightPaddle(void)
{
	return m_rightPaddle;
}
Ball* PlayingField::getBall(void)
{
	return m_ball;
}