#pragma once
class Graphics
{
public:
	Graphics(void);
	~Graphics(void);
	void init(void);
	void prepareForDisplay(void);
	void redrawDisplay(void);
	void resize(void);
};

