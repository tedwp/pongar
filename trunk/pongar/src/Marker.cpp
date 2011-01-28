#include "Marker.h"
#include "Game.h"

#include "GL\glut.h"

Marker::Marker(int id)
{
	m_id = id;
	m_position = new float[16];
}


Marker::~Marker(void)
{
	delete(m_position);
}


void Marker::updatePosition(float position [16])
{
	m_positionChanged = false;
	
	for(unsigned i = 0; i < 16; i++)
	{
		if(position[i] != m_position[i])
		m_positionChanged = true;
	}
	
	if(m_positionChanged)
	{
		for( int i = 0; i < 16; i++ )
			m_position[ i ] = position[ i ];
	}
}

float* Marker::getPosition(void)
{
	return m_position;
}

int Marker::getId(void)
{
	return m_id;

}
int Marker::getPurpose(void)
{
	return m_purpose;
}
void Marker::setPurpose(int p)
{
	m_purpose = p;
}

bool Marker::hasPositionChanged(void)
{
	return m_positionChanged;
}

void Marker::setOffset(float offset){
	m_offset = offset;
}
float Marker::getOffset(){
	return m_offset;
}