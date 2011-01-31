#include "Game.h"
#include "Graphics.h"
#include <vector>

using namespace std;

Game::Game(void) 
{
}


Game::~Game(void)
{
	for(unsigned i = 0; i < m_markers.size(); i++)
	{
		delete(m_markers[i]);
	}
	m_markers.clear();
}

Game& Game::getInstance(void)
{
	static Game m_instance;
	return m_instance;

}

void Game::init( int argc, char* argv[] )
{
	m_gameStage = STAGE_INITIALIZATION;

	getInstance().registerMarker(2884, PURPOSE_PADDLE1);
	getInstance().registerMarker(626, PURPOSE_PADDLE2);
	getInstance().registerMarker(90, PURPOSE_PLAYINGFIELD);
		
	Capture::getInstance().init();
	Graphics::getInstance().init(argc, argv);

	glutTimerFunc( 25, timer, 0 );
}
void Game::registerMarker(int id, int purpose)
{
	Marker* m = new Marker(id);
	m->setPurpose(purpose);
	m_markers.push_back(m);
}
void Game::start(void)
{
	Graphics::getInstance().start();
}

vector<Marker*> Game::getMarkers(void)
{
	return getInstance().m_markers;
}

Marker* Game::getMarkerById(int id)
{
	for(unsigned i = 0; i < getInstance().m_markers.size(); i++)
	{
		if(	(getInstance().m_markers[i])->getId() == id)
		{
			return getInstance().m_markers[i];
		}
	}
	return NULL;
}

Marker* Game::getMarkerByPurpose(int purpose)
{
	for(unsigned i = 0; i < getInstance().m_markers.size(); i++)
	{
		if(	(getInstance().m_markers[i])->getPurpose() == purpose)
		{
			return getInstance().m_markers[i];
		}
	}
	return NULL;
}

void Game::timer(int value)
{
	
	/**
	Hier kommt die Spiellogik rein
	Diese Methode wird von Anfang an immer wieder aufgerufen.
	Um verschiedene Spielphasen abzubilden, wäre ein Property m_gameStage sinnvoll,
	z.B. m_gameStage = Game.STAGE_INITIALIZATION oder m_gameStage = Game.STAGE_PAUSE oder ..._RUNNING usw.

	- Die Zeit muss gemessen werden und es muss klar sein, wie lange der letzte Aufruf dieser Methode zurück liegt (~ FPS)
	  Wichtig für Animationen und andere Sachen (z.B. 3 Sekunden Spielfeldkalibrierung oÄ)

	 Grundidee:
	 - Steuerung des Spiels durch Karten. Jeweils einem PURPOSE zugeordnet (siehe Game::init())
	 - Zugriff auf Paddle-Marker mit Hilfe von Game::getMarkerByPurpose();
	 - Jedes Paddle-Objekt hat ein Marker-Property, mit dem es seine Position aktualisieren kann.
	 **/
	int gameStage = Game::getInstance().getGameStage();
	switch(gameStage)
	{
		case STAGE_INITIALIZATION:
			// Nicht zu verwechseln mit init()! Hier wird das Spiel aus Sicht der Benutzers initialisiert, alles technische geschieht hingegen in init()
			Game::getInstance().performInitialization();
			break;
		
		case STAGE_RUNNING:
			Game::getInstance().updateMarkerOffsets();

			//performRunning();
			break;

		case STAGE_PAUSE:
			//performPause();
			break;
		default:
		break;
	}
 
	glutTimerFunc( 25, timer, 0 );
}
void Game::idle( void )
{
	Capture::getInstance().updateMarkerPositions();
}

void Game::performInitialization(void){
	Marker* m_PlayingField = Game::getMarkerByPurpose(Game::PURPOSE_PLAYINGFIELD);
	if(m_PlayingField!=NULL){
		int currentTime = glutGet(GLUT_ELAPSED_TIME);
		int lastUpdateTime = m_PlayingField->getLastUpdateTime();
		if (lastUpdateTime == 0){
			lastUpdateTime = currentTime;
		}
		int elapsedTime = currentTime - lastUpdateTime;
		if (elapsedTime > 5000){
			//playing field marker is now invisible for 5 seconds -> start game!
			m_gameStage = STAGE_RUNNING;
		}
	}
}

void Game::updateMarkerOffsets(void){
	
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
		float paddle1z = cvGet2D(paddle1Mat, 2, 3).val[0];
		float playingFieldZ = cvGet2D(playingFieldMat, 2, 3).val[0];
		
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
}

void Game::cleanup( void )
{
	Capture::getInstance().cleanup();
}

int Game::getGameStage(){
	return m_gameStage;
}

void Game::setGameStage(int stage){
	m_gameStage = stage;
}

void Game::arrayToCvMat(float* transform, CvMat* mat){
	cvZero( mat );
	for (unsigned i = 0; i < 16; i++){
		cvmSet( mat, i/4, i%4, transform[i] );
	}
}