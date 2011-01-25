#pragma once

class Marker
{

public:
	Marker(int id);
	~Marker(void);
	void setPosition(cvMat newPosition);
private:
		int m_id;

};
