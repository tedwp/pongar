#include "Paddle.h"


Paddle::Paddle(void)
{
	m_color[0] = 0;
	m_color[1] = 0;
	m_color[2] = 0;
	m_marker = NULL;
	m_isLeft = true;
}


Paddle::~Paddle(void)
{
}
void Paddle::render(PlayingField* playingField )
{
	//Todo use yPosition, isLeft, color and playingField to render paddle
}

void Paddle::updatePositionFromMarker(void)
{
	//TODO
}

int* Paddle::getColor(void)
{
	return m_color;
}
float Paddle::getYPosition(void)
{
	return m_yPosition;
}
int Paddle::getScore(void)
{
	return m_score;
}
void Paddle::setMarker(Marker* marker)
{
	m_marker = marker;
}
bool Paddle::isLeft(void)
{
	return m_isLeft;
}
void Paddle::setLeft(bool isLeft)
{
	m_isLeft = isLeft;
}