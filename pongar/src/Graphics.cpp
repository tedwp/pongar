#include "temp.h"

#include "Graphics.h"
#include "GL/glut.h"

Graphics::Graphics(void)
{
}


Graphics::~Graphics(void)
{
	if( m_instance != NULL )
		getInstance()->cleanup();
		delete( m_instance );


}

Graphics* Graphics::getInstance(void)
{
	if(m_instance == NULL)
	{
		m_instance = new Graphics();
	}
	return m_instance;

}
void Graphics::init(void)
{
	// initialize the window system
    glutInit( 0, NULL );
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
    glutIdleFunc( idle );
}
void Graphics::start(void)
{
	glutMainLoop();
}
void Graphics::render()
{
	Graphics::getInstance()->doRender();
    
}
void Graphics::doRender()
{
	prepareForDisplay();
	
	//Anfang - Über die Marker iterieren

		
	
	float resultTransposedMatrix[16];
	for (int x=0; x<4; ++x)
	{
		for (int y=0; y<4; ++y)
		{
			resultTransposedMatrix[x*4+y] = resultMatrix_005A[y*4+x];
		}
	}


	
	//Ende - ÜdMi
	
	
	//glLoadTransposeMatrixf( resultMatrix );
	glLoadMatrixf( resultTransposedMatrix );
	glScalef(0.20, 0.20, 0.20);
	
	// draw white rectangle
	
	glColor4f( 1.0, 1.0, 1.0, 0.7 );
	glRectf(-0.5, -0.8, 0.5, 0.8);

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
    glDrawPixels( width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, bkgnd );

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
	Graphics::getInstance()->doResize(w, h);
}
void Graphics::doResize( int w, int h) 
{
	//width = w;
	//height = h;

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
void Graphics::cleanup(void)
{
	cvReleaseMemStorage (&memStorage);
	cvReleaseCapture (&cap);
	cvDestroyWindow ("Original Image");
	cvDestroyWindow ("Converted");
	cvDestroyWindow ("Stripe");
	cvDestroyWindow ("Marker");
}