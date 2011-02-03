#include "Marker.h"
#include "Game.h"


Marker::Marker(int id)
{
	m_id = id;
	m_position = new float[16];
	m_lastUpdateTime = -1;

	m_locked = false;
}


Marker::~Marker(void)
{
	delete(m_position);
}

void Marker::clearHasPositionChanged(void)
{
	m_positionChanged = false;
}
void Marker::updatePosition(float position [16])
{
	if(!m_locked)
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

		m_lastUpdateTime = getTimeSinceStart();
	}
}
bool Marker::isVisible(void)
{
	return m_lastUpdateTime != -1 && (getTimeSinceStart() - m_lastUpdateTime < MARKER_VISIBILITYTIMEOUT);
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

int Marker::getLastUpdateTime(){
	return m_lastUpdateTime;
}
void Marker::lock(void)
{
	m_locked = true;
}
void Marker::unLock(void)
{
	m_locked = false;
}