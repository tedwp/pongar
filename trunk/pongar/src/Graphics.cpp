#include "Graphics.h"

Graphics::Graphics(void)
{
	isInFullScreen = false;
}


Graphics::~Graphics(void)
{
	fullScreenLeave();
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
	
	glutInitWindowSize( CAM_WIDTH, CAM_HEIGHT );
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
    GLfloat light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_dif[] = { 0.7f, 0.7f, 0.7f, 1.0f };

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

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(Keyboard::pressKey);
	glutSpecialFunc(Keyboard::pressSpecialKey);
		
	fullScreenLeave();

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
	//TODO: Ein Marker ist kein Paddle und hat erstmal auch nichts damit zu tun. plz Ändern.
	//TODO: Das gehört hier alles absolut nicht rein.
	
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

		cvGEMM(paddle1Mat, playingFieldMatInv, 1, NULL, 0, paddle1Mat, 0);
		cvGEMM(paddle2Mat, playingFieldMatInv, 1, NULL, 0, paddle2Mat, 0);

		float paddle1offset = (float) cvGet2D(paddle1Mat, 1, 3).val[0];
		float paddle2offset = (float) cvGet2D(paddle2Mat, 1, 3).val[0];
		
		//set y offset of paddles
		float paddle1z = (float) cvGet2D(paddle1Mat, 2, 3).val[0];
		float playingFieldZ = (float) cvGet2D(playingFieldMat, 2, 3).val[0];
		//std::cout << "z field ";
		//std::cout << playingFieldZ;
		//std::cout << " // z paddle1 ";
		//std::cout << paddle1z;
		//std::cout << std::endl;

		float sensitivityFactor = 4.0;
		//TODO adjust sensitivityFactor depending on z coordinate?!?
		m_paddle1->setOffset(paddle1offset*sensitivityFactor);
		m_paddle2->setOffset(paddle2offset*sensitivityFactor);

		//release matrices
		cvReleaseMat( &playingFieldMat );
		cvReleaseMat( &playingFieldMatInv );
		cvReleaseMat( &paddle1Mat );
		cvReleaseMat( &paddle2Mat );
	}
  
	PlayingField::getInstance().render();
	
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
    gluOrtho2D( 0.0, CAM_WIDTH, 0.0, CAM_HEIGHT );

    glRasterPos2i( 0, CAM_HEIGHT-1 );
    if(SHOW_CAMERA_IMAGE)
	{
		glDrawPixels( CAM_WIDTH, CAM_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_bkgnd );
	}
	
	
	drawStuffOnTop();
	//TODO is it correct to drawStuffOnTop() before popMatrix?
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
void Graphics::drawStuffOnTop(void)
{
	getInstance().showString("Warum zum Henker steht das auf dem Kopf?", 0, 0, 255, 100, 100);
	if(Game::getInstance().getGameStage() == Game::getInstance().STAGE_BEAMERCALIBRATION)
	{
		//memcpy(m_bkgnd, Game::getInstance().m_markerImage->imageData, sizeof(Game::getInstance().m_markerImage) );
		//glDrawPixels( CAM_WIDTH, CAM_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, Game::getInstance().m_markerImage->imageData );
	}
}
void Graphics::resize( int w, int h) 
{
	Graphics::getInstance().doResize(w, h);
}
void Graphics::doResize( int w, int h) 
{

    // set a whole-window viewport
    glViewport( 0, 0, CAM_HEIGHT, CAM_WIDTH );

    // create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	// Note: Just setting the Perspective is an easy hack. In fact, the camera should be calibrated.
	// With such a calibration we would get the projection matrix. This matrix could then be loaded 
	// to GL_PROJECTION.
	// If you are using another camera (which you'll do in most cases), you'll have to adjust the FOV
	// value. How? Fiddle around: Move Marker to edge of display and check if you have to increase or 
	// decrease.
    gluPerspective( CAM_ANGLE, ((double)CAM_WIDTH/(double)CAM_HEIGHT), 0.01, 100 );

    // invalidate display
    glutPostRedisplay();

}

void Graphics::arrayToCvMat(float* transform, CvMat* mat){
	cvZero( mat );
	for (unsigned i = 0; i < 16; i++)
	{
		cvmSet( mat, i/4, i%4, transform[i] );
	}
}
void Graphics::fullScreenEnter(void)
{
	if(!isInFullScreen && FULLSCREEN_ENABLED)
	{	char* result = "";
		//sprintf( result, "%dx%d:%d@%d", width ,height, fullScreenBitRate ,fullScreenRefreshRate);
		result = "640x480:32@60";
		//glutGameModeString( result);
		//glutEnterGameMode();
		//TODO fix above only if gamemode is needed
		isInFullScreen = true;
		glutFullScreen();
	}
}
void Graphics::fullScreenSwitch(void)
{
	isInFullScreen ? fullScreenLeave() : fullScreenEnter();
}
void Graphics::fullScreenLeave(void)
{
	glutPositionWindow((int) ((glutGet(GLUT_SCREEN_WIDTH) - CAM_WIDTH) * .5), (int)  ((glutGet(GLUT_SCREEN_HEIGHT) - CAM_HEIGHT) * .5));
	glutReshapeWindow(CAM_WIDTH, CAM_HEIGHT);
	isInFullScreen = false;
}
void Graphics::showString(char string[], float r, float g, float b, int cx, int y)
{
        glColor3f(r, g, b);
		glRasterPos3f((GLfloat) cx, (GLfloat) y, (GLfloat) 0);
        for(unsigned int i = 0; i < strlen(string); i++)
        {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , string[i]);
        }
}