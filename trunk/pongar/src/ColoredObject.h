#pragma once
#include "Color.h"

class ColoredObject
{
public:
	ColoredObject(void);
	~ColoredObject(void);
	Color getColor(void);
	void setColor(float r, float g, float b, float a);
	void setColor(Color& c);
protected:
	Color m_color;
};

