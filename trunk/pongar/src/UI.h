#pragma once
#include <iostream>
#include <string>

#include "conf.h"

#include "Graphics.h"

class UI
{
public:
	static UI& getInstance(void);

	void showPercentageString(std::string str, int value, int max);
	void showHeading(std::string heading);
	void showInstruction(std::string instruction);
	void drawStuffOnTop(void);
	

private:
	UI(void);
	UI(const UI&);
	~UI(void);
	std::string m_heading;
	std::string m_instructions;
};
/*
std::string UI::m_heading;
std::string UI::m_instructions;*/
