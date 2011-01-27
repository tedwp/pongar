#include "Capture.h"
#include "Game.h"
#include "Graphics.h"
#include "GL/glut.h"

#include <iostream>

Graphics::Graphics(void)
{
}


Graphics::~Graphics(void)
{
}

Graphics& Graphics::getInstance(void)
{
	static Graphics m_instance;
	return m_instance;

}
void Graphics::init(int argc, char* argv[])
{
	// initialize the window system
    glutInit( &argc, argv);
    glutInitWindowSize( width, height );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutCreateWindow("PongAR");

    // initialize the GL library

    // pixel storage/packing stuff
    glPixelStorei( GL_PACK_ALIGNMENT,   1 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glPixelZoom( 1.0, -1.0 );

    // enable and set colors
    glEnable( GL_COLOR_MATERIAL );
    glClearColor( 0, 0, 0, 1.0 );

    // enable and set depth parameters
    glEnable( GL_DEPTH_TEST );
    glClearDepth( 1.0 );

    // light parameters
    GLfloat light_pos[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_amb[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_dif[] = { 0.7, 0.7, 0.7, 1.0 };

    // enable lighting
    glLightfv( GL_LIGHT0, GL_POSITION, light_pos );
    glLightfv( GL_LIGHT0, GL_AMBIENT,  light_amb );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,  light_dif );
    //glEnable( GL_LIGHTING );
    //glEnable( GL_LIGHT0 );

    // make functions known to GLUT
    glutDisplayFunc( render );
    glutReshapeFunc( resize  );
    glutIdleFunc( getInstance().idle);
}
void Graphics::idle(void)
{
	Game::getInstance().idle();
	glutPostRedisplay();
}

void Graphics::start(void)
{
	glutMainLoop();
}
void Graphics::render()
{
	Graphics::getInstance().doRender();
    
}
void Graphics::doRender()
{
	prepareForDisplay();
	
	//Anfang - Über die Marker iterieren

	float resultTransposedMatrix[16];
	for(unsigned i = 0; i < Game::getMarkers().size(); i++)
	{
		/*std::cout << "\nRendering #";
		std::cout << Game::getMarkers()[i]->getId();*/
		Game::getMarkers()[i]->render();
	}
	
	Marker* m_playingfield = Game::getMarkerByPurpose(Game::PURPOSE_PLAYINGFIELD);
	Marker* m_paddle1 = Game::getMarkerByPurpose(Game::PURPOSE_PADDLE1);
	Marker* m_paddle2 = Game::getMarkerByPurpose(Game::PURPOSE_PADDLE2);
	if(m_playingfield != NULL && m_paddle1 != NULL && m_paddle2 != NULL)
	{
		float* playingFieldTf = m_playingfield->getPosition();
		float* paddle1Tf = m_paddle1->getPosition();
		float* paddle2Tf = m_paddle2->getPosition();
	
		//invert playingFieldTf and apply to paddle1Tf and paddle2Tf
		CvMat* playingFieldMat = cvCreateMat( 4, 4, CV_32FC1 );
		arrayToCvMat(playingFieldTf, playingFieldMat);
		CvMat* paddle1Mat = cvCreateMat( 4, 4, CV_32FC1 );
		arrayToCvMat(paddle1Tf, paddle1Mat);
		CvMat* paddle2Mat = cvCreateMat( 4, 4, CV_32FC1 );
		arrayToCvMat(paddle2Tf, paddle2Mat);
	
		CvMat* playingFieldMatInv = cvCreateMat(4, 4, CV_32FC1);
		cvInvert(playingFieldMat, playingFieldMatInv);

		cvMul(paddle1Mat, playingFieldMatInv, paddle1Mat);
		cvMul(paddle2Mat, playingFieldMatInv, paddle2Mat);

		float paddle1offset = cvGet2D(paddle1Mat, 1, 3).val[0];
		float paddle2offset = cvGet2D(paddle2Mat, 1, 3).val[0];

		//set y offset of paddles
		float sensitivityFactor = 180;
		//TODO adjust sensitivityFactor depending on z coordinate?!?
		m_paddle1->setOffset(paddle1offset*sensitivityFactor);
		m_paddle2->setOffset(paddle2offset*sensitivityFactor);

		//release matrices
		cvReleaseMat( &playingFieldMat );
		cvReleaseMat( &playingFieldMatInv );
		cvReleaseMat( &paddle1Mat );
		cvReleaseMat( &paddle2Mat );
	}

	redrawDisplay();

}
void Graphics::prepareForDisplay(void)
{
	// clear buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw background image
    glDisable( GL_DEPTH_TEST );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0.0, width, 0.0, height );

    glRasterPos2i( 0, height-1 );
    glDrawPixels( width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_bkgnd );

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);

	
    // move to origin
    glMatrixMode( GL_MODELVIEW );

}
void Graphics::redrawDisplay(void)
{
	// redraw
    glutSwapBuffers();
}

void Graphics::resize( int w, int h) 
{
	Graphics::getInstance().doResize(w, h);
}
void Graphics::doResize( int w, int h) 
{

    // set a whole-window viewport
    glViewport( 0, 0, width, height );

    // create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	// Note: Just setting the Perspective is an easy hack. In fact, the camera should be calibrated.
	// With such a calibration we would get the projection matrix. This matrix could then be loaded 
	// to GL_PROJECTION.
	// If you are using another camera (which you'll do in most cases), you'll have to adjust the FOV
	// value. How? Fiddle around: Move Marker to edge of display and check if you have to increase or 
	// decrease.
    gluPerspective( camangle, ((double)width/(double)height), 0.01, 100 );

    // invalidate display
    glutPostRedisplay();

}


void Graphics::arrayToCvMat(float* transform, CvMat* mat){
	cvZero( mat );
	for (unsigned i = 0; i < 16; i++){
		cvmSet( mat, i/4, i%4, transform[i] );
	}
}