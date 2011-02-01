#include "Ball.h"


Ball::Ball(void)
{
	speed = BALL_SPEED_INIT;

	//init random angle (between 30 and -30 deg)
	//0 deg = ball goes straight up
	srand ((unsigned) getTimeSinceStart());
	angle = (float) (rand() % 60 - 30);

	position.first = 0.0;
	position.second = 0.0;

	
}


Ball::~Ball(void)
{
}

void Ball::render(void)
{
	float paddle1Start =  PADDLE_LENGTH/2 - m_playingField->getLeftPaddle()->getYPosition();
	float paddle2Start =  PADDLE_LENGTH/2 - m_playingField->getRightPaddle()->getYPosition();

	float paddle1End = paddle1Start + PADDLE_LENGTH;
	float paddle2End = paddle2Start + PADDLE_LENGTH;

	float degInRad=angle*3.14159f/180;
	vector.first = speed * sin(degInRad);
	vector.second = speed * cos(degInRad);

	position.first = position.first + vector.first;
	position.second = position.second + vector.second;

	bool xCollision = false;
	bool yCollision = false;
	bool pCollision = false;
	if (position.second+BALL_RADIUS > PLAYINGFIELD_WIDTH/2) xCollision = true;
	if (position.second-BALL_RADIUS < -PLAYINGFIELD_WIDTH/2) xCollision = true;
	if (position.first+BALL_RADIUS > PLAYINGFIELD_HEIGHT/2) yCollision = true;
	if (position.first-BALL_RADIUS < -PLAYINGFIELD_HEIGHT/2) yCollision = true;
	if (!xCollision && position.second-BALL_RADIUS <= -0.75 && position.first<=paddle1Start && position.first>=paddle1End) pCollision = true;
	if (!xCollision && position.second+BALL_RADIUS >= 0.75 && position.first<=paddle2Start && position.first>=paddle2End) pCollision = true;

	if (xCollision)
	{
		// GAME OVER
		Game::getInstance().setGameStage(Game::STAGE_OVER);
	} else if (pCollision) {
		position.first = position.first - vector.first;
		position.second = position.second - vector.second;
		angle = 180-angle;
		position.first = position.first + vector.first;
		position.second = position.second + vector.second;

		//increase ball speed
		speed = speed * BALL_SPEED_INCREASE_FACTOR;
	} else if (yCollision) {
		position.first = position.first - vector.first;
		position.second = position.second - vector.second;
		angle = 360-angle;
		position.first = position.first + vector.first;
		position.second = position.second + vector.second;
	}



	glTranslatef( position.first, position.second, 0.0 );
	glColor4f(m_color.red, m_color.green, m_color.blue, m_color.alpha);
	drawCircle(BALL_RADIUS);


	//TODO render Ball relative to playingField
	//TODO render as a Circle due to backprojection
}

void Ball::drawCircle(float r)
{
	glBegin(GL_TRIANGLE_FAN);
  
	for (int i=0; i<360; ++i)
	{
		float degInRad=i*3.14159f/180;
		glVertex2f(cos(degInRad)*r, sin(degInRad)*r);
	}
	glEnd();
}




void Ball::setPlayingField(PlayingField* playingField)
{
	m_playingField = playingField;
}