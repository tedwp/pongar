#pragma once
class GL
{
public:
	GL(void);
	~GL(void);
	void init(void);
	void prepareForDisplay(void);
	void redrawDisplay(void);
	void resize(void);
};

