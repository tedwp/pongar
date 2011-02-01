#include "Capture.h"
#include "Game.h"
#include "Graphics.h"
#include "GL/glut.h"

#include <iostream>

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
	//TODO: Ein Marker ist kein Paddle und hat erstmal auch nichts damit zu tun. plz �ndern.
	//TODO: Das geh�rt hier alles absolut nicht rein.
	
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
    glDrawPixels( CAM_WIDTH, CAM_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_bkgnd );

	drawStuffOnTop();

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
    glViewport( 0, 0, CAM_WIDTH, CAM_HEIGHT );

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
void Graphics::showString(std::string& str, float r, float g, float b, int cx, int y)
{
        glColor3f(r, g, b);
		glRasterPos3f((GLfloat) cx, (GLfloat) y, (GLfloat) 0);
		for(unsigned int i = 0; i < str.length(); i++)
        {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , str[i]);
        }
}

void Graphics::drawStuffOnTop(void)
{
	getInstance().showString(m_currentString, 0, 0, 255, 100, 100);
	if(Game::getInstance().getGameStage() == Game::getInstance().STAGE_BEAMERCALIBRATION)
	{
		//memcpy(m_bkgnd, Game::getInstance().m_markerImage->imageData, sizeof(Game::getInstance().m_markerImage) );
		//glDrawPixels( CAM_WIDTH, CAM_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, Game::getInstance().m_markerImage->imageData );
	}
}