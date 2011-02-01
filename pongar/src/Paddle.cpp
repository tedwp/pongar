#include "Paddle.h"


Paddle::Paddle(void)
{
	m_color.red = 0;
	m_color.green = 0;
	m_color.blue = 0;
	m_marker = NULL;
	m_isLeft = true;
	
	m_historyLength = 0;
	for(unsigned i = 0; i < SMOOTHING_HISTORYSIZE; i++)
	{
		m_historyYPosition[i] = 0;
		m_timestampsYPosition[i] = 0;
	}
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

Color Paddle::getColor(void)
{
	return m_color;
}
void Paddle::setColor(Color c)
{
	m_color = c;
}
void Paddle::setColor(int r, int g, int b)
{
	m_color.red = r;
	m_color.green = g;
	m_color.blue = b;
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