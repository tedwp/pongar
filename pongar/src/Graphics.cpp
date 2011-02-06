#include <iostream>

#include "Capture.h"
#include "Game.h"
#include "Graphics.h"
#include "GL/glut.h"
#include "UI.h"


Graphics::Graphics(void)
{
	m_isInFullScreen = false;
	m_isInGameMode = false;
	m_mainWindow = 0;
	m_camPosition.x = 0;
	m_camPosition.y = 0;
	m_camPosition.z = 0;
}


Graphics::~Graphics(void)
{
	cleanup();
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
    m_mainWindow = glutCreateWindow("PongAR");

    // initialize the GL library

	
	if(FULLSCREEN_ONSTARTUP)
		fullScreenEnter();
	else fullScreenLeave();


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

	
	glutIgnoreKeyRepeat(false);
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
	PlayingField::getInstance().render();
	
	prepare2D();
	UI::getInstance().drawStuffOnTop();
	finish2D();
	redrawDisplay();

}
void Graphics::prepare2D(void)
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw background image
    glDisable( GL_DEPTH_TEST );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0.0, CAM_WIDTH, 0.0, CAM_HEIGHT );
	glRasterPos2i( 0, CAM_HEIGHT-1 );
   
}
void Graphics::finish2D(void)
{
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
// move to origin
    glMatrixMode( GL_MODELVIEW );
}
void Graphics::prepareForDisplay(void)
{
	// clear buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    prepare2D();
	if(SHOW_CAMERA_IMAGE)
	{
		glDrawPixels( CAM_WIDTH, CAM_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_bkgnd );
	}
	finish2D();

}
void Graphics::redrawDisplay(void)
{
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
	if(!m_isInFullScreen && FULLSCREEN_ENABLED)
	{	
		std::string tmp1 = toString(CAM_WIDTH) + "x" + toString(CAM_HEIGHT) + ":" + toString(FULLSCREEN_BITRATE)+ "@" + toString(FULLSCREEN_REFRESHRATE);
		glutGameModeString(tmp1.c_str());
		if(FULLSCREEN_USEGAMEMODE && FULLSCREEN_ONSTARTUP && (FULLSCREEN_DISABLECHECK || glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)))
		{
			glutEnterGameMode();
			glutSetCursor(GLUT_CURSOR_NONE);
			m_isInGameMode = true;
		}
		else
		{
			glutFullScreen();
		}
		m_isInFullScreen = true;
		
	}
}
void Graphics::fullScreenSwitch(void)
{
	m_isInFullScreen ? fullScreenLeave() : fullScreenEnter();
}
void Graphics::fullScreenLeave(void)
{
	if(!FULLSCREEN_USEGAMEMODE || !FULLSCREEN_ONSTARTUP)
	{
		glutPositionWindow((int) ((glutGet(GLUT_SCREEN_WIDTH) - CAM_WIDTH) * .5), (int)  ((glutGet(GLUT_SCREEN_HEIGHT) - CAM_HEIGHT) * .5));
		glutReshapeWindow(CAM_WIDTH, CAM_HEIGHT);
		m_isInFullScreen = false;
	}
}
void Graphics::showString(std::string str, Color& c, int cx, int y)
{
	y = CAM_HEIGHT - y;
	glColor4f(1.0, 1.0, 1.0, 1.0);
	int f = 2;
	glRasterPos3f((GLfloat) cx -f, (GLfloat) y -f, (GLfloat) 0);
	for(unsigned int i = 0; i < str.length(); i++)
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	
	glRasterPos3f((GLfloat) cx -f, (GLfloat) y, (GLfloat) 0);
	for(unsigned int i = 0; i < str.length(); i++)
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

	glRasterPos3f((GLfloat) cx, (GLfloat) y -f, (GLfloat) 0);
	for(unsigned int i = 0; i < str.length(); i++)
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	
	glRasterPos3f((GLfloat) cx +f, (GLfloat) y, (GLfloat) 0);
	for(unsigned int i = 0; i < str.length(); i++)
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

	glRasterPos3f((GLfloat) cx, (GLfloat) y +f, (GLfloat) 0);
	for(unsigned int i = 0; i < str.length(); i++)
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	
	glRasterPos3f((GLfloat) cx +f, (GLfloat) y +f, (GLfloat) 0);
	for(unsigned int i = 0; i < str.length(); i++)
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);


    glColor4f(c.red, c.green, c.blue, c.alpha);
	glRasterPos3f((GLfloat) cx, (GLfloat) y, (GLfloat) 0);
	for(unsigned int i = 0; i < str.length(); i++)
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}



void Graphics::transposeMatrix(float* src, float* dst)
{
	for (int x=0; x<4; ++x)
	{
		for (int y=0; y<4; ++y)
		{
			dst[x*4+y] = src[y*4+x];
		}
	}
}

void Graphics::cleanup(void)
{
	glutDestroyWindow(m_mainWindow);
	fullScreenLeave();
}

void Graphics::setCamPosition( vertex& pos )
{
	m_camPosition = pos;
}

vertex& Graphics::getCamPosition( void )
{
	return m_camPosition;
}

void Graphics::moveCamera( float x , float y, float z )
{
	m_camPosition.x += x;
	m_camPosition.y += y;
	m_camPosition.z += z;
}