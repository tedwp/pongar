#include "Paddle.h"


Paddle::Paddle(void)
{
	m_color.red = 0;
	m_color.green = 0;
	m_color.blue = 0;
	m_marker = NULL;
	m_isLeft = true;
	
}


Paddle::~Paddle(void)
{
}
void Paddle::render(void)
{
	//Todo use yPosition, isLeft, color and m_playingField to render paddle
}

void Paddle::updatePositionFromMarker(void)
{
/*	float* playingFieldTf = m_playingField->getCorrespondingMarker()->getPosition();
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
	cvReleaseMat( &paddle1Mat );*/

}


void Paddle::arrayToCvMat(float* transform, CvMat* mat)
{
	cvZero( mat );
	for (unsigned i = 0; i < 16; i++)
	{
		cvmSet( mat, i/4, i%4, transform[i] );
	}
}



Color Paddle::getColor(void)
{
	return m_color;
}
void Paddle::setColor(Color c)
{
	m_color = c;
}
void Paddle::setColor(int r, int g, int b)
{
	m_color.red = r;
	m_color.green = g;
	m_color.blue = b;
}
float Paddle::getYPosition(void)
{
	return m_yPosition;
}
int Paddle::getScore(void)
{
	return m_score;
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

/*void Paddle::setPlayingField(PlayingField* playingField)
{
	m_playingField = playingField;
}*/
