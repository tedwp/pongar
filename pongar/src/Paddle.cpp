#include "Paddle.h"


Paddle::Paddle(void)
{
	m_color.red = 255;
	m_color.green = 0;
	m_color.blue = 255;
	m_color.alpha = 255;
	m_marker = NULL;
	m_isLeft = true;
	m_playingField = NULL;
	m_score = 0;
	m_size = PADDLE_LENGTH;
}


Paddle::~Paddle(void)
{
}
void Paddle::render(void)
{
	glTranslatef( 0.0f, 0.0f, -0.01f );
	
	float paddle1YStart =  m_size/2 - m_yPosition;
	float paddle1YEnd = -m_size/2 - m_yPosition;

	if (paddle1YStart > PLAYINGFIELD_HEIGHT/2)
	{
		paddle1YEnd = PLAYINGFIELD_HEIGHT/2;
		paddle1YStart = paddle1YEnd - m_size;
	}
	if (paddle1YEnd < -PLAYINGFIELD_HEIGHT/2)
	{
		paddle1YStart = -PLAYINGFIELD_HEIGHT/2;
		paddle1YEnd = paddle1YStart + m_size;
	}

	//m_yRenderPosition = paddle1YStart;

	// draw paddle
	glColor4f( m_color.red, m_color.green, m_color.blue, m_color.alpha );
	if(isLeft())
		glRectf(paddle1YEnd, -(PLAYINGFIELD_WIDTH / 2) - PADDLE_WIDTH/2 , paddle1YStart, -(PLAYINGFIELD_WIDTH / 2) + PADDLE_WIDTH/2);
	else
		glRectf(paddle1YEnd, PLAYINGFIELD_WIDTH / 2 - PADDLE_WIDTH/2, paddle1YStart, PLAYINGFIELD_WIDTH / 2 + PADDLE_WIDTH/2);
}

void Paddle::updatePositionFromMarker(void)
{
	float* playingFieldTf = m_playingField->getCorrespondingMarker()->getPosition();
	float* paddle1Tf = m_marker->getPosition();
	
	float vec1[3];
	vec1[0] = paddle1Tf[3]-playingFieldTf[3];
	vec1[1] = paddle1Tf[7]-playingFieldTf[7];
	vec1[2] = 0.0;

	//get point on inner axis and paddle axis
	float xaxisVectorInp[4] = {0,1,0,0};
	float xaxisVector[4] = {0,0,0,0};
	//mult vector by matrix
	for(int i=0; i<4; i++){
		xaxisVector[i] = 0;
		for(int j=0; j<4; j++){
			xaxisVector[i] += playingFieldTf[4*i + j] * xaxisVectorInp[j];
		}
	}

	float vec2[3];
	vec2[0] = xaxisVector[0]-playingFieldTf[3];
	vec2[1] = xaxisVector[1]-playingFieldTf[7];
	vec2[2] = 0.0;

	//calculate angle between these vectors
	//first normalize vectors
	float help = sqrt( vec1[0]*vec1[0] + vec1[1]*vec1[1] + vec1[2]*vec1[2] );
	vec1[0] /= help;
	vec1[1] /= help;
	vec1[2] /= help;

	help = sqrt( vec2[0]*vec2[0] + vec2[1]*vec2[1] + vec2[2]*vec2[2] );
	vec2[0] /= help;
	vec2[1] /= help;
	vec2[2] /= help;

	//then calculate the angle
	float angle = (180 / 3.14159f) * acos( vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2] );
	if((vec1[0] * vec2[1] - vec1[1] * vec2[0]) < 0 ) angle *= -1;

	m_yPosition = (float) PLAYINGFIELD_WIDTH/2 * sin(angle*3.14159f/180);

	/*float paddle1offset = (float) PLAYINGFIELD_WIDTH/2 * sin(angle*3.14159f/180);
	
	m_yPosition = paddle1offset;
	m_yRenderPosition = paddle1offset;*/
}


void Paddle::arrayToCvMat(float* transform, CvMat* mat)
{
	cvZero( mat );
	for (unsigned i = 0; i < 16; i++)
	{
		cvmSet( mat, i/4, i%4, transform[i] );
	}
}

float Paddle::getYPosition(void)
{
	return m_yPosition;
}
/*float Paddle::getYRenderPosition(void)
{
	return m_yRenderPosition;
}*/
void Paddle::setMarker(Marker* marker)
{
	m_marker = marker;
}
bool Paddle::isLeft(void)
{
	return m_isLeft;
}
void Paddle::setLeft(bool isLeft)
{
	m_isLeft = isLeft;
}

void Paddle::setPlayingField(PlayingField* playingField)
{
	m_playingField = playingField;
}
void Paddle::increaseScore(void)
{
	m_score++;
}
int Paddle::getScore(void)
{
	return m_score;
}
void Paddle::reset(void)
{
	m_yPosition = 0;
	//m_yRenderPosition = 0;
	m_score = 0;
}
float Paddle::getSize(void)
{
	return m_size;
}
void Paddle::disableAllActions(void)
{
	disableActionSizeDecrease();
	disableActionSizeIncrease();
}
void Paddle::enableActionSizeIncrease(void)
{
	if(!m_actionSizeIncreaseEnabled)
	{
		m_size *= ACTION_INCREASESIZE_PADDLE_FACTOR;
		m_actionSizeIncreaseEnabled = true;
	}
}
void Paddle::disableActionSizeIncrease(void)
{
	if(m_actionSizeIncreaseEnabled)
	{
		m_size /= ACTION_INCREASESIZE_PADDLE_FACTOR;
		m_actionSizeIncreaseEnabled = false;
	}
}
void Paddle::enableActionSizeDecrease(void)
{
	if(!m_actionSizeDecreaseEnabled)
	{
		m_size *= ACTION_DECREASESIZE_PADDLE_FACTOR;
		m_actionSizeDecreaseEnabled = true;
	}
}
void Paddle::disableActionSizeDecrease(void)
{
	if(m_actionSizeDecreaseEnabled)
	{
		m_size /= ACTION_DECREASESIZE_PADDLE_FACTOR;
		m_actionSizeDecreaseEnabled = false;
	}
}