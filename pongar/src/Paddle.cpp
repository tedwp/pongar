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
}


Paddle::~Paddle(void)
{
}
void Paddle::render(void)
{
	glTranslatef( 0.0f, 0.0f, -0.01f );
	float paddle1YStart =  PADDLE_LENGTH/2 - m_yPosition;
	float paddle1YEnd = -PADDLE_LENGTH/2 - m_yPosition;
	// draw rectangle
	glColor4f( m_color.red, m_color.green, m_color.blue, m_color.alpha );
	if (paddle1YEnd+PADDLE_LENGTH > PLAYINGFIELD_HEIGHT/2)
	{
		paddle1YEnd = PLAYINGFIELD_HEIGHT/2;
		paddle1YStart = paddle1YEnd - PADDLE_LENGTH;
	}
	if (paddle1YStart-PADDLE_LENGTH < -PLAYINGFIELD_HEIGHT/2)
	{
		paddle1YStart = -PLAYINGFIELD_HEIGHT/2;
		paddle1YEnd = paddle1YStart + PADDLE_LENGTH;
	}
	if(isLeft())
		glRectf(paddle1YEnd, -(PLAYINGFIELD_WIDTH / 2) , paddle1YStart, -(PLAYINGFIELD_WIDTH / 2) + PADDLE_WIDTH);
	else
		glRectf(paddle1YEnd, PLAYINGFIELD_WIDTH / 2 - PADDLE_WIDTH, paddle1YStart, PLAYINGFIELD_WIDTH / 2 );
}

void Paddle::updatePositionFromMarker(void)
{
	float* playingFieldTf = m_playingField->getCorrespondingMarker()->getPosition();
	float* paddle1Tf = m_marker->getPosition();
	
	//invert playingFieldTf and apply to paddle1Tf and paddle2Tf
	CvMat* playingFieldMat = cvCreateMat( 4, 4, CV_32FC1 );
	arrayToCvMat(playingFieldTf, playingFieldMat);
	CvMat* paddle1Mat = cvCreateMat( 4, 4, CV_32FC1 );
	arrayToCvMat(paddle1Tf, paddle1Mat);
	
	CvMat* playingFieldMatInv = cvCreateMat(4, 4, CV_32FC1);
	cvInvert(playingFieldMat, playingFieldMatInv);

	cvGEMM(paddle1Mat, playingFieldMatInv, 1, NULL, 0, paddle1Mat, 0);

	float paddle1offset = (float) cvGet2D(paddle1Mat, 1, 3).val[0];
		

	float sensitivityFactor = 4.0;
	//TODO adjust sensitivityFactor depending on z coordinate?!?
	m_yPosition = paddle1offset*sensitivityFactor;

	//release matrices
	cvReleaseMat( &playingFieldMat );
	cvReleaseMat( &playingFieldMatInv );
	cvReleaseMat( &paddle1Mat );

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
void Paddle::resetScore(void)
{
	m_score = 0;
}