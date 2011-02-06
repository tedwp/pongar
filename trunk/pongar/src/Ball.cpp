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
	drawCircle(BALL_RADIUS);
}
void Ball::updateMovement(void)
{
	float realX = m_y;
	float realY = m_x;

	int timePassed = getTimeSinceStart() - m_lastUpdate;
	//TODO take this into account, right now ball is only dependent on cpu cycles! btw: wtf?!
	//float paddle1Start =  PADDLE_LENGTH/2 - m_playingField->getLeftPaddle()->getYRenderPosition();
	//float paddle2Start =  PADDLE_LENGTH/2 - m_playingField->getRightPaddle()->getYRenderPosition();
  
	float lY = - m_playingField->getLeftPaddle()->getYPosition();
	float rY = - m_playingField->getRightPaddle()->getYPosition();
	
	

	float leftPaddleStart = - PADDLE_LENGTH/2 + lY;
	float rightPaddleStart = - PADDLE_LENGTH/2 + rY;
	
	float leftPaddleEnd =  PADDLE_LENGTH/2 + lY;
	float rightPaddleEnd =  PADDLE_LENGTH/2 + rY;

	

	//Set new coordinates
	realX += m_direction.first * timePassed * m_speed;
	realY += m_direction.second * timePassed* m_speed;

	//UI::getInstance().showHeading(toString(leftPaddleStart) + " to " + toString(leftPaddleEnd));
	UI::getInstance().showHeading(toString(m_playingField->getLeftPaddle()->getYPosition()));
	bool collisionDetected_lr = false;
	bool collisionDetected_tb = false;
	if(realX - BALL_RADIUS < - PLAYINGFIELD_WIDTH / 2)
	{
		//left out
		collisionDetected_lr = true;

	}
	if(realX + BALL_RADIUS > PLAYINGFIELD_WIDTH / 2)
	{
		//right out
		collisionDetected_lr = true;
	}
	if(   realY - BALL_RADIUS < - PLAYINGFIELD_HEIGHT / 2
	   || realY + BALL_RADIUS > PLAYINGFIELD_HEIGHT / 2
	  )
	{
		//bottom out
		collisionDetected_tb = true;
	}


	if(collisionDetected_tb)
		m_direction.second*= -1;
	
	else if(collisionDetected_lr)
		m_direction.first*= -1;

	m_state = ONFIELD;

	if(collisionDetected_lr)
	{
		if(   realX < 0
		   && (
		       realY + BALL_RADIUS > leftPaddleEnd
			   || realY - BALL_RADIUS < leftPaddleStart
			  )
		  )
		{
			m_state = LEFTOUT;
		}
		
		if(   realX > 0
		   && (
		       realY + BALL_RADIUS > rightPaddleEnd
			   || realY - BALL_RADIUS < rightPaddleStart
			  )
		  )
		{
			m_state = RIGHTOUT;
		}
	}
	/*

	bool lrCollision = false;
	bool yCollision = false;
	bool pCollision = false;

	if (realX - BALL_RADIUS <= -(PLAYINGFIELD_WIDTH / 2) + PADDLE_WIDTH //Ball-Äußeres außerhalb der inneren Paddle-Grenze
		&& realY <= -paddle1End && realY >= -paddle1Start //Ball im 
		)
		pCollision = true;
	
	if (realX + BALL_RADIUS >= (PLAYINGFIELD_WIDTH / 2) - PADDLE_WIDTH
		&& realY <= -paddle2End && realY >= -paddle2Start
		)
		pCollision = true;


	if (!pCollision && realX + BALL_RADIUS > PLAYINGFIELD_WIDTH/2)
		lrCollision = true;
	
	if (!pCollision && realX - BALL_RADIUS < -PLAYINGFIELD_WIDTH/2)
		lrCollision = true;
	
	if (realY + BALL_RADIUS > PLAYINGFIELD_HEIGHT/2)
		yCollision = true;
	
	if (realY - BALL_RADIUS < -PLAYINGFIELD_HEIGHT/2)
		yCollision = true;
	
	
	
	
	if(!lrCollision )
		m_state = ONFIELD;
	if(lrCollision  || pCollision || yCollision)
	{
		//beep war nervig, deswegen auskommentiert
		//UI::getInstance().beep();

		if (lrCollision )
		{
			// Left or right?
			m_state = LEFTOUT;
			m_state = RIGHTOUT;

		} else if (pCollision)
		{
			realY-= vector.first;
			realX-= vector.second;
			m_angle = 180 - m_angle;
			realY+= vector.first;
			realX+= vector.second;

			//increase ball m_speed
			m_speed*= BALL_SPEED_INCREASE_FACTOR;
		} else if (yCollision)
		{
			realY-= vector.first;
			realX-= vector.second;
			m_angle = 360 - m_angle;
			realY+=  vector.first;
			realX+=  vector.second;
		}
	}
	*/
	if(!collisionDetected_lr && !collisionDetected_tb)
	{
		m_y = realX ;
		m_x = realY;
	}

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