#include "UI.h"
#include <string>
#include <windows.h>

using namespace std;

UI::UI(void) 
{
	m_textColor.red = 0;
	m_textColor.green = 0;
	m_textColor.blue  = 0;
	m_textColor.alpha = 1.0f;
	//m_markerImage = NULL;
}
UI::~UI(void)
{
}

UI& UI::getInstance(void)
{
	static UI m_instance;
	return m_instance;
}

void UI::showPercentageString(string str, int value, int max)
{
	if(value < 0 || value > max)
	{
		m_instructions = str + "Marker nicht zu sehen";
	}
	else
	{
		m_instructions  = str + toString(((float)value / (float)max) * 100.0f) + "%";
	}
}
void UI::beep(void)
{
	Beep(522, 50);
}
void UI::showHeading(string heading)
{
	m_heading = heading;
}
void UI::showInstruction(std::string instruction)
{
	m_instructions = instruction;
}

void UI::drawStuffOnTop(void)
{
	Graphics::getInstance().showString(m_instructions, m_textColor, 50, 20);

	if(m_heading != "")
	{
		Graphics::getInstance().showString(m_heading, m_textColor, 200, 200);
		m_heading = "";
	}

	string th =  toString(Capture::getInstance().getThreshold()/255.0 * 100);
	string thbw =  toString(Capture::getInstance().getThresholdBW()/255.0 * 100);

	Graphics::getInstance().showString("Th: " + th + "% ThBW: " + thbw + "%", m_textColor, 430, 20);

	if(Game::getInstance().getGameStage() == Game::getInstance().STAGE_BEAMERCALIBRATION)
	{
		//memcpy(m_bkgnd, Game::getInstance().m_markerImage->imageData, sizeof(Game::getInstance().m_markerImage) );
		//glDrawPixels( CAM_WIDTH, CAM_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, Game::getInstance().m_markerImage->imageData );
	}
}
void UI::showScores(void)
{
	m_instructions = toString(PlayingField::getInstance().getLeftPaddle()->getScore())
				   + "    <->    "
				   + toString(PlayingField::getInstance().getRightPaddle()->getScore());
}