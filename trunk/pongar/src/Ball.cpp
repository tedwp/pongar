#include "Ball.h"

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
	int timePassed = getTimeSinceStart() - m_lastUpdate;
	//TODO take this into account, right now ball is only dependent on cpu cycles! btw: wtf?!
	//float paddle1Start =  PADDLE_LENGTH/2 - m_playingField->getLeftPaddle()->getYRenderPosition();
	//float paddle2Start =  PADDLE_LENGTH/2 - m_playingField->getRightPaddle()->getYRenderPosition();
  
	Paddle* leftPaddle = m_playingField->getLeftPaddle();
	Paddle* rightPaddle = m_playingField->getRightPaddle();

	float paddle1End =  -leftPaddle->getYRenderPosition();
	float paddle2End =  -rightPaddle->getYRenderPosition();

	float paddle1Start = paddle1End + leftPaddle->getSize();
	float paddle2Start = paddle2End + rightPaddle->getSize();

	float degInRad = m_angle*3.14159f/180;
	
	std::pair<float, float> vector;

	vector.first = m_speed * sin(degInRad);
	vector.second = m_speed * cos(degInRad);

	m_x+= vector.first;
	m_y+= vector.second;

	bool xCollision = false;
	bool yCollision = false;
	bool pCollision = false;
	if (m_y + BALL_RADIUS > PLAYINGFIELD_WIDTH/2)
		xCollision = true;
	
	if (m_y - BALL_RADIUS < -PLAYINGFIELD_WIDTH/2)
		xCollision = true;
	
	if (m_x + BALL_RADIUS > PLAYINGFIELD_HEIGHT/2)
		yCollision = true;
	
	if (m_x - BALL_RADIUS < -PLAYINGFIELD_HEIGHT/2)
		yCollision = true;
	
	if (!xCollision
		&& m_y - BALL_RADIUS <= -(PLAYINGFIELD_WIDTH / 2) + leftPaddle->getSize()
		&& m_x <= -paddle1End && m_x >= -paddle1Start
		)
		pCollision = true;
	
	if (!xCollision
		&& m_y + BALL_RADIUS >= (PLAYINGFIELD_WIDTH / 2) - rightPaddle->getSize()
		&& m_x <= -paddle2End && m_x >= -paddle2Start
		)
		pCollision = true;
	
	
	if(!xCollision)
		m_state = ONFIELD;
	if(xCollision || pCollision || yCollision)
	{
		//beep war nervig, deswegen auskommentiert
		UI::getInstance().beep();

		if (xCollision)
		{
			// Left or right?
			m_x < 0 ? m_state = LEFTOUT : m_state = RIGHTOUT;

		} else if (pCollision)
		{
			m_x-= vector.first;
			m_y-= vector.second;
			m_angle = 180 - m_angle;
			m_x+= vector.first;
			m_y+= vector.second;

			//increase ball m_speed
			m_speed*= BALL_SPEED_INCREASE_FACTOR;
		} else if (yCollision)
		{
			m_x-= vector.first;
			m_y-= vector.second;
			m_angle = 360 - m_angle;
			m_x+=  vector.first;
			m_y+=  vector.second;
		}
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
	m_angle = (float) (rand() % 60 - 30);
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