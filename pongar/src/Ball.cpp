#include "Ball.h"
#include <Math.h>

Ball::Ball(void)
{
	reset();
}


Ball::~Ball(void)
{
}

void Ball::render(void)
{
	glTranslatef( m_x, m_y, 0.0f );
	glColor4f(m_color.red, m_color.green, m_color.blue, m_color.alpha);
	drawCircle(m_radius);
}
void Ball::updateMovement(void)
{
	float realX = m_y;
	float realY = m_x;

	int timePassed = getTimeSinceStart() - m_lastUpdate;
	Paddle* leftPaddle = m_playingField->getLeftPaddle();
	Paddle* rightPaddle = m_playingField->getRightPaddle();
	float lY = - leftPaddle->getYPosition();
	float rY = - rightPaddle->getYPosition();
	
	float leftPaddleStart = - leftPaddle->getSize()/2 + lY;
	float rightPaddleStart = - rightPaddle->getSize()/2 + rY;
	
	float leftPaddleEnd =  leftPaddle->getSize()/2 + lY;
	float rightPaddleEnd =  leftPaddle->getSize()/2 + rY;

	

	//Set new coordinates
	realX += m_direction.first * timePassed * m_speed;
	realY += m_direction.second * timePassed* m_speed;

	bool collisionDetected_lr = false;
	bool collisionDetected_tb = false;
	if(realX - m_radius < - PLAYINGFIELD_WIDTH / 2)
	{
		//left out
		collisionDetected_lr = true;

	}
	if(realX + m_radius > PLAYINGFIELD_WIDTH / 2)
	{
		//right out
		collisionDetected_lr = true;
	}
	if(   realY - m_radius < - PLAYINGFIELD_HEIGHT / 2
	   || realY + m_radius > PLAYINGFIELD_HEIGHT / 2
	  )
	{
		//bottom out
		collisionDetected_tb = true;
	}

	if(collisionDetected_tb || collisionDetected_lr)
	{
		if(collisionDetected_tb)
			m_direction.second*= -1;
	
		else if(collisionDetected_lr)
			m_direction.first*= -1;
		
		UI::getInstance().beep();
	}
	m_state = ONFIELD;

	if(collisionDetected_lr)
	{
		if(   realX < 0
		   && (
		       realY > leftPaddleEnd + m_radius 
			   || realY < leftPaddleStart - m_radius 
			  )
		  )
		{
			m_state = LEFTOUT;
		}
		else if(   realX > 0
		   && (
		       realY > rightPaddleEnd + m_radius 
			   || realY < rightPaddleStart - m_radius 
			  )
		  )
		{
			m_state = RIGHTOUT;
		}
		else
		{
			m_speed *= BALL_SPEED_INCREASE_FACTOR;
		}
	}
	if(!collisionDetected_lr && !collisionDetected_tb)
	{
		m_y = realX ;
		m_x = realY;
	}

	m_lastUpdate = getTimeSinceStart();
}
void Ball::resumeAfterPause(void)
{
	m_lastUpdate = getTimeSinceStart();
}
void Ball::drawCircle(float r)
{
	glBegin(GL_TRIANGLE_FAN);
	for (int i=0; i<360; i++)
	{
		float degInRad = i * 3.14159f/180;
		glVertex2f(cos(degInRad) * r, sin(degInRad)*r);
	}
	glEnd();
}

void Ball::reset(void)
{
	m_speed = BALL_SPEED_INIT;
	m_radius = BALL_RADIUS;
	srand ((unsigned) getTimeSinceStart());
	
	float degInRad = (float) (rand() % 60 - 30)*3.14159f/180;
	

	m_direction.first = cos(degInRad);// * m_speed;
	m_direction.second = sin(degInRad); //* m_speed;

	float tmp = sqrt(pow(m_direction.first, 2) + pow(m_direction.second, 2));
	m_direction.first /= tmp;
	m_direction.second /= tmp;
	//Now direction is normalized
	m_state = ONFIELD;
	m_x = 0.0f;
	m_y = 0.0f;
}
void Ball::setPlayingField(PlayingField* playingField)
{
	m_playingField = playingField;
}

int Ball::getState(void)
{
	return m_state;
}


void Ball::disableAllActions(void)
{
	disableActionSpeedDecrease();
	disableActionSpeedIncrease();
	disableActionSizeDecrease();
	disableActionSizeIncrease();
}
void Ball::enableActionSpeedIncrease(void)
{
	if(!m_actionSpeedIncreaseEnabled)
	{
		m_speed *= ACTION_INCREASESPEED_BALL_FACTOR;
		m_actionSpeedIncreaseEnabled = true;
	}
}
void Ball::disableActionSpeedIncrease(void)
{
	if(m_actionSpeedIncreaseEnabled)
	{
		m_speed /= ACTION_INCREASESPEED_BALL_FACTOR;
		m_actionSpeedIncreaseEnabled = false;
	}
}
void Ball::enableActionSpeedDecrease(void)
{
	if(!m_actionSpeedDecreaseEnabled)
	{
		m_speed *= ACTION_DECREASESPEED_BALL_FACTOR;
		m_actionSpeedDecreaseEnabled = true;
	}
}
void Ball::disableActionSpeedDecrease(void)
{
	if(m_actionSpeedDecreaseEnabled)
	{
		m_speed /= ACTION_DECREASESPEED_BALL_FACTOR;
		m_actionSpeedDecreaseEnabled = false;
	}
}


void Ball::enableActionSizeIncrease(void)
{
	if(!m_actionSizeIncreaseEnabled)
	{
		m_radius *= ACTION_INCREASESPEED_BALL_FACTOR;
		m_actionSizeIncreaseEnabled = true;
	}
}
void Ball::disableActionSizeIncrease(void)
{
	if(m_actionSizeIncreaseEnabled)
	{
		m_radius /= ACTION_INCREASESPEED_BALL_FACTOR;
		m_actionSizeIncreaseEnabled = false;
	}
}

void Ball::enableActionSizeDecrease(void)
{
	if(!m_actionSizeDecreaseEnabled)
	{
		m_radius *= ACTION_DECREASESPEED_BALL_FACTOR;
		m_actionSizeDecreaseEnabled = true;
	}
}
void Ball::disableActionSizeDecrease(void)
{
	if(m_actionSizeDecreaseEnabled)
	{
		m_radius /= ACTION_DECREASESPEED_BALL_FACTOR;
		m_actionSizeDecreaseEnabled = false;
	}
}
float Ball::getRadius(void)
{
	return m_radius;
}