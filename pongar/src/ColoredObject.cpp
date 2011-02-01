#include "ColoredObject.h"


ColoredObject::ColoredObject(void)
{
}


ColoredObject::~ColoredObject(void)
{
}

Color ColoredObject::getColor(void)
{
	return m_color;
}
void ColoredObject::setColor(Color& c)
{
	m_color = c;
}
void ColoredObject::setColor(float r, float g, float b, float a)
{
	m_color.red = r;
	m_color.green = g;
	m_color.blue = b;
	m_color.alpha = a;
}