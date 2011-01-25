#pragma once
class Marker
{
public:
	Marker(void);
	~Marker(void);
	void setPosition(cvMat newPosition);
private:
		int id;

};
