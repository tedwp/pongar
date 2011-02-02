#include "UI.h"
#include <string>

using namespace std;

UI::UI(void) 
{
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
	Graphics::getInstance().showString(m_instructions, 0, 0, 255, 10, 10);


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