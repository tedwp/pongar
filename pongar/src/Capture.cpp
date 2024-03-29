#include "Capture.h"

using namespace std;

Capture::Capture(void)
{
	m_threshold = THRESHOLD;
	m_thresholdBW = THRESHOLD_BW;
}

Capture::~Capture(void)
{
}

Capture& Capture::getInstance(void)
{
	static Capture m_instance;
	return m_instance;

}

void Capture::init()
{
	//cvNamedWindow ("Original Image", CV_WINDOW_AUTOSIZE);
	if(SHOW_CONVERTED && !FULLSCREEN_ENABLED)
	{
		cvNamedWindow ("Converted", CV_WINDOW_AUTOSIZE);
		cvResizeWindow("Converted", 300, 80);
	}
	/*cvNamedWindow ("Stripe", CV_WINDOW_AUTOSIZE);*/
	/*cvNamedWindow ("Marker", 0 );
	cvResizeWindow("Marker", 120, 120 );*/
	
	initVideoStream();
	
	/*int value = m_threshold;
	int max = 255;
	cvCreateTrackbar( "Threshold", "Converted", &value, max, trackbarHandler);
	
	int bw_value = Capture::getInstance().m_thresholdBW;
	cvCreateTrackbar( "BW Threshold", "Converted", &bw_value, max, bw_trackbarHandler);
	*/
	m_memStorage = cvCreateMemStorage();
}


void Capture::initVideoStream(void)
{
	m_cap = cvCaptureFromCAM (CAM_INDEX);
	m_cap = cvCreateCameraCapture (CAM_INDEX);
	cvSetCaptureProperty(m_cap, CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
	cvSetCaptureProperty(m_cap, CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
	
	if (!m_cap) {
		cout << "No webcam found\n";
		exit(0);
	}
}

void Capture::updateMarkerPositions(void)
{
	//bool isFirstStripe = true;

	bool isFirstMarker = true;

	IplImage* iplGrabbed = cvQueryFrame(Capture::getInstance().m_cap);

	if(!iplGrabbed){
		printf("Could not query frame. Trying to reinitialize.\n");
		cvReleaseCapture (&Capture::getInstance().m_cap);
		initVideoStream();
		return;
	}

	CvSize picSize = cvGetSize(iplGrabbed);
	IplImage* iplDisplay;
	if (IMAGE_FLIP_H || IMAGE_FLIP_V)
	{
		iplDisplay = cvCreateImage(picSize, iplGrabbed->depth, iplGrabbed->nChannels);

		if(IMAGE_FLIP_H && IMAGE_FLIP_V)
		{
			cvFlip(iplGrabbed, iplDisplay, 1);
			cvFlip(iplDisplay, iplDisplay, 0);
		}
		else if(IMAGE_FLIP_H)
		{
			cvFlip(iplGrabbed, iplDisplay, 1);
		}else if(IMAGE_FLIP_V)
		{
			cvFlip(iplGrabbed, iplDisplay, 0);
		}
	} else {
		iplDisplay = cvCloneImage(iplGrabbed);
	}
	
	memcpy( Graphics::getInstance().m_bkgnd, iplDisplay->imageData, sizeof(Graphics::getInstance().m_bkgnd) );

	IplImage* iplConverted = cvCreateImage(picSize, IPL_DEPTH_8U, 1);
	IplImage* iplThreshold = cvCreateImage(picSize, IPL_DEPTH_8U, 1);

	cvConvertImage(iplDisplay, iplConverted, 0);
	cvThreshold(iplConverted, iplThreshold, getInstance().m_thresholdBW, 255, CV_THRESH_BINARY);
	//cvAdaptiveThreshold(iplConverted, iplThreshold, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 33, 5);

	// Find Contours
	CvSeq* contours;
	cvFindContours(
		iplThreshold, Capture::getInstance().m_memStorage, &contours, sizeof(CvContour),
		CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE
	);

	for (; contours; contours = contours->h_next)
	{
		CvSeq* result = cvApproxPoly(
			contours, sizeof(CvContour), Capture::getInstance().m_memStorage, CV_POLY_APPROX_DP,
			cvContourPerimeter(contours)*0.02, 0
		);

		CvRect r = cvBoundingRect(result);
		if (r.height < 20 || r.width < 20 || r.height >= iplGrabbed->height - 10 || r.width >= iplGrabbed->width - 10) {
			continue;
		}

		if (result->total==4)
		{
			int count = 4;
			CvPoint *rect = new CvPoint[4];
			cvCvtSeqToArray(result, rect);
			cvPolyLine(iplGrabbed, &rect, &count, 1, 1, CV_RGB(255,0,0), 2);
			
			float lineParams[16];

			for (int i=0; i<4; ++i)
			{
				cvCircle (iplGrabbed, rect[i], 3, CV_RGB(0,255,0), -1);

				double dx = (double)(rect[(i+1)%4].x-rect[i].x)/7.0;
				double dy = (double)(rect[(i+1)%4].y-rect[i].y)/7.0;

				int stripeLength = (int)(0.8*sqrt (dx*dx+dy*dy));
				if (stripeLength < 5)
				stripeLength = 5;

				//make stripeLength odd (because of the shift in nStop)
				stripeLength |= 1;

				//e.g. stripeLength = 5 --> from -2 to 2
				int nStop  = stripeLength>>1;
				int nStart = -nStop;

				CvSize stripeSize;
				stripeSize.width = 3;
				stripeSize.height = stripeLength;

				CvPoint2D32f stripeVecX;
				CvPoint2D32f stripeVecY;

				//normalize vectors
				double diffLength = sqrt ( dx*dx+dy*dy );
				stripeVecX.x = (float) (dx / diffLength);
				stripeVecX.y = (float) (dy / diffLength);

				stripeVecY.x =  stripeVecX.y;
				stripeVecY.y = -stripeVecX.x;

				IplImage* iplStripe = cvCreateImage( stripeSize, IPL_DEPTH_8U, 1 );

				// Array for edge point centers
				CvPoint2D32f points[6];

				for (int j=1; j<7; ++j)
				{
					double px = (double)rect[i].x+(double)j*dx;
					double py = (double)rect[i].y+(double)j*dy;

					CvPoint p;
					p.x = (int)px;
					p.y = (int)py;
					cvCircle (iplGrabbed, p, 2, CV_RGB(0,0,255), -1);

					for ( int m = -1; m <= 1; ++m )
					{
						for ( int n = nStart; n <= nStop; ++n )
						{
							CvPoint2D32f subPixel;

							subPixel.x =(float) ( (double)p.x + ((double)m * stripeVecX.x) + ((double)n * stripeVecY.x));
							subPixel.y =(float) ( (double)p.y + ((double)m * stripeVecX.y) + ((double)n * stripeVecY.y));

							CvPoint p2;
							p2.x = (int)subPixel.x;
							p2.y = (int)subPixel.y;

							/*if (isFirstStripe)
								cvCircle (iplGrabbed, p2, 1, CV_RGB(255,0,255), -1);
							else
								cvCircle (iplGrabbed, p2, 1, CV_RGB(0,255,255), -1);
*/
							int pixel = Capture::getInstance().subpixSampleSafe (iplConverted, subPixel);

							int w = m + 1; //add 1 to shift to 0..2
							int h = n + ( stripeLength >> 1 ); //add stripelenght>>1 to shift to 0..stripeLength

							*(iplStripe->imageData + h * iplStripe->widthStep  + w) =  pixel; //set pointer to correct position and safe subpixel intensity
						}
					}

					//use sobel operator on stripe
					// ( -1 , -2, -1 )
					// (  0 ,  0,  0 )
					// (  1 ,  2,  1 )
					
					double* sobelValues = new double[stripeLength-2];
					for (int n = 1; n < (stripeLength-1); n++)
					{
						unsigned char* stripePtr = ( unsigned char* )( iplStripe->imageData + (n-1) * iplStripe->widthStep );
						double r1 = -stripePtr[ 0 ] - 2 * stripePtr[ 1 ] - stripePtr[ 2 ];

						stripePtr += 2*iplStripe->widthStep;
						double r3 =  stripePtr[ 0 ] + 2 * stripePtr[ 1 ] + stripePtr[ 2 ];
						sobelValues[n-1] = r1+r3;
					}

					double maxVal = -1;
					int maxIndex = 0;
					for (int n=0; n<stripeLength-2; ++n)
					{
						if ( sobelValues[n] > maxVal )
						{
							maxVal = sobelValues[n];
							maxIndex = n;
						}
					}

					double y0,y1,y2; // y0 .. y1 .. y2
					y0 = (maxIndex <= 0) ? 0 : sobelValues[maxIndex-1];
					y1 = sobelValues[maxIndex];
					y2 = (maxIndex >= stripeLength-2) ? 0 : sobelValues[maxIndex+1];

					//formula for calculating the x-coordinate of the vertex of a parabola, given 3 points with equal distances 
					//(xv means the x value of the vertex, d the distance between the points): 
					//xv = x1 + (d / 2) * (y2 - y0)/(2*y1 - y0 - y2)

					double pos = (y2 - y0) / (4*y1 - 2*y0 - 2*y2 ); //d = 1 because of the normalization and x1 will be added later
					
					CvPoint2D32f edgeCenter; //exact point with subpixel accuracy
					int maxIndexShift = maxIndex - (stripeLength>>1);

					//shift the original edgepoint accordingly
					edgeCenter.x = (float) ( (double)p.x + (((double)maxIndexShift+pos) * stripeVecY.x));
					edgeCenter.y = (float) ((double)p.y + (((double)maxIndexShift+pos) * stripeVecY.y));

					/*CvPoint p_tmp;
					p_tmp.x = (int)edgeCenter.x;
					p_tmp.y = (int)edgeCenter.y;
					cvCircle (iplGrabbed, p_tmp, 1, CV_RGB(0,0,255), -1);*/

					points[j-1].x = edgeCenter.x;
					points[j-1].y = edgeCenter.y;

					/*if (isFirstStripe)
					{
						//IplImage* iplTmp = cvCreateImage( cvSize(100,300), IPL_DEPTH_8U, 1 );
						//cvResize( iplStripe, iplTmp, CV_INTER_NN );
						//cvShowImage ( "Stripe", iplTmp );//iplStripe );
						//cvReleaseImage( &iplTmp );
						isFirstStripe = false;
					}*/

				} // end of loop over edge points of one edge
				cvReleaseImage ( &iplStripe );

				// we now have the array of exact edge centers stored in "points"
				CvMat mat = cvMat ( 1, 6, CV_32FC2, points);
				cvFitLine ( &mat, CV_DIST_L2, 0, 0.01, 0.01, &lineParams[4*i] );
				// cvFitLine stores the calculated line in lineParams in the following way:
				// vec.x, vec.y, point.x, point.y

				/*CvPoint p;
				p.x=(int)lineParams[4*i+2] - (int)(50.0*lineParams[4*i+0]);
				p.y=(int)lineParams[4*i+3] - (int)(50.0*lineParams[4*i+1]);

				CvPoint p2;
				p2.x = (int)lineParams[4*i+2] + (int)(50.0*lineParams[4*i+0]);
				p2.y = (int)lineParams[4*i+3] + (int)(50.0*lineParams[4*i+1]);

				cvLine (iplGrabbed, p, p2, CV_RGB(0,255,255), 1, 8, 0);*/

			} // end of loop over the 4 edges

			// so far we stored the exact line parameters and show the lines in the image
			// now we have to calculate the exact corners
			CvPoint2D32f corners[4];

			for (int i=0; i<4; ++i)
			{
				int j = (i+1)%4;
				double x0,x1,y0,y1,u0,u1,v0,v1;
				x0 = lineParams[4*i+2]; y0 = lineParams[4*i+3];
				x1 = lineParams[4*j+2]; y1 = lineParams[4*j+3];

				u0 = lineParams[4*i+0]; v0 = lineParams[4*i+1];
				u1 = lineParams[4*j+0]; v1 = lineParams[4*j+1];

				// (x|y) = p + s * vec
				// s = Ds / D (see cramer's rule)
				// (x|y) = p + (Ds / D) * vec
				// (x|y) = (p * D / D) + (Ds * vec / D)
				// (x|y) = (p * D + Ds * vec) / D
				// (x|y) = a / c;
				double a =  x1*u0*v1 - y1*u0*u1 - x0*u1*v0 + y0*u0*u1;
				double b = -x0*v0*v1 + y0*u0*v1 + x1*v0*v1 - y1*v0*u1;
				double c =  v1*u0-v0*u1;

				if ( c == 0 ) //lines parallel?
				{
					//std::cout << "lines parallel" << std::endl;
					continue;
				}

				a /= c;
				b /= c;

				//exact corner
				corners[i].x = (float) a; 
				corners[i].y = (float) b;
				/*CvPoint p;
				p.x = (int)corners[i].x;
				p.y = (int)corners[i].y;

				cvCircle (iplGrabbed, p, 5, CV_RGB(i*60,i*60,0), -1);*/
			} //finished the calculation of the exact corners

			CvPoint2D32f targetCorners[4];
			targetCorners[0].x = -0.5; targetCorners[0].y = -0.5;
			targetCorners[1].x =  5.5; targetCorners[1].y = -0.5;
			targetCorners[2].x =  5.5; targetCorners[2].y =  5.5;
			targetCorners[3].x = -0.5; targetCorners[3].y =  5.5;

			//create and calculate the matrix of perspective transform
			CvMat* projMat = cvCreateMat (3, 3, CV_32F );
			cvWarpPerspectiveQMatrix ( corners, targetCorners, projMat);

			//create image for the marker
			CvSize markerSize;
			markerSize.width  = 6;
			markerSize.height = 6;
			IplImage* iplMarker = cvCreateImage( markerSize, IPL_DEPTH_8U, 1 );

			//change the perspective in the marker image using the previously calculated matrix
			cvWarpPerspective(iplConverted, iplMarker, projMat, CV_WARP_FILL_OUTLIERS,  cvScalarAll(0));
			
			cvThreshold(iplMarker, iplMarker, Capture::getInstance().m_thresholdBW, 255, CV_THRESH_BINARY);

			//now we have a B/W image of a supposed Marker

			// check if border is black
			int code = 0;
			for (int i = 0; i < 6; ++i)
			{
				int pixel1 = ((unsigned char*)(iplMarker->imageData + 0*iplMarker->widthStep + i))[0]; //top
				int pixel2 = ((unsigned char*)(iplMarker->imageData + 5*iplMarker->widthStep + i))[0]; //bottom
				int pixel3 = ((unsigned char*)(iplMarker->imageData + i*iplMarker->widthStep))[0]; //left
				int pixel4 = ((unsigned char*)(iplMarker->imageData + i*iplMarker->widthStep + 5))[0]; //right
				if ( ( pixel1 > 0 ) || ( pixel2 > 0 ) || ( pixel3 > 0 ) || ( pixel4 > 0 ) )
				{
					code = -1;
					break;
				}
			}

			if ( code < 0 ) continue;

			//copy the BW values into cP
			int cP[4][4];
			for ( int i=0; i < 4; ++i)
			{
				for ( int j=0; j < 4; ++j)
				{
					cP[i][j] = ((unsigned char*)(iplMarker->imageData + (i+1)*iplMarker->widthStep + (j+1) ))[0];
					cP[i][j] = (cP[i][j]==0) ? 1 : 0; //if black then 1 else 0
				}
			}

			//save the ID of the marker
			int codes[4];
			codes[0] = codes[1] = codes[2] = codes[3] = 0;
			for (int i=0; i < 16; i++)
			{
				int row = i>>2;
				int col = i%4;

				codes[0] <<= 1;
				codes[0] |= cP[row][col]; // 0�

				codes[1] <<= 1;
				codes[1] |= cP[3-col][row]; // 90�

				codes[2] <<= 1;
				codes[2] |= cP[3-row][3-col]; // 180�

				codes[3] <<= 1;
				codes[3] |= cP[col][3-row]; // 270�
			}

			if ( (codes[0]==0) || (codes[0]==0xffff) )
				continue;

			//account for symmetry
			code = codes[0];
			int angle = 0;
			for ( int i=1; i<4; ++i )
			{
				if ( codes[i] < code )
				{
					code = codes[i];
					angle = i;
				}
			}

			//correct order of corners
			if(angle != 0)
			{
				CvPoint2D32f corrected_corners[4];
				for(int i = 0; i < 4; i++)	corrected_corners[(i + angle)%4] = corners[i];
				for(int i = 0; i < 4; i++)	corners[i] = corrected_corners[i];
			}

			if ( isFirstMarker )
				isFirstMarker = false;

			// transfer camera coords to screen coords
			for(int i = 0; i<4; i++)
			{
				corners[i].x -= CAM_WIDTH/2;
				corners[i].y = -corners[i].y + CAM_HEIGHT/2;
			}
			//std::cout << "\nMarker found: " << code;
			Marker* m = Game::getMarkerById(code);
			if(m != NULL)
			{
				float shizzle[16];
				estimateSquarePose( shizzle, corners, m->getSize() );
				m->updatePosition(shizzle);
			}

			cvReleaseMat (&projMat);
			cvReleaseImage (&iplMarker);
			delete[] rect;
		} // end of if(result->total == 4)
	} // end of loop over contours

	/*cvShowImage("Original Image", iplGrabbed);
	*/
	if(SHOW_CONVERTED && !FULLSCREEN_ENABLED)
		cvShowImage("Converted", iplThreshold);
	

	//isFirstStripe = true;
	isFirstMarker = true;

	cvReleaseImage (&iplConverted);
	cvReleaseImage (&iplThreshold);
	cvReleaseImage (&iplDisplay);

	cvClearMemStorage ( Capture::getInstance().m_memStorage );
}


int Capture::subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p )
{
	int x = int( floorf ( p.x ) );
	int y = int( floorf ( p.y ) );

	if ( x < 0 || x >= pSrc->width  - 1 ||
		 y < 0 || y >= pSrc->height - 1 )
		return 127;

	int dx = int ( 256 * ( p.x - floorf ( p.x ) ) );
	int dy = int ( 256 * ( p.y - floorf ( p.y ) ) );

	unsigned char* i = ( unsigned char* ) ( ( pSrc->imageData + y * pSrc->widthStep ) + x );
	int a = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
	i += pSrc->widthStep;
	int b = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
	return a + ( ( dy * ( b - a) ) >> 8 );
}

void Capture::cleanup()
{
	cvReleaseMemStorage (&m_memStorage);
	cvReleaseCapture (&(Capture::getInstance().m_cap));
	//cvDestroyWindow ("Original Image");
	
	if(SHOW_CONVERTED && !FULLSCREEN_ENABLED)
		cvDestroyWindow ("Converted");
	//cvDestroyWindow ("Stripe");
	//cvDestroyWindow ("Marker");
}

//trackbar
/*void Capture::trackbarHandler(int pos)
{
	getInstance().m_thresholdBW = pos;
}

void Capture::bw_trackbarHandler(int pos)
{
	getInstance().m_thresholdBW = pos;
}*/

int Capture::getThreshold(void)
{
	return m_threshold;
}
int Capture::getThresholdBW(void)
{
	return m_thresholdBW;
}
void Capture::increaseThreshold(void)
{
	if(m_threshold < 255)
		m_threshold++;
}
void Capture::increaseThresholdBW(void)
{
	if(m_thresholdBW < 255)
		m_thresholdBW++;
}
void Capture::decreaseThreshold(void)
{
	if(m_thresholdBW > 0)
		m_threshold--;
}
void Capture::decreaseThresholdBW(void)
{
	if(m_thresholdBW > 0)
		m_thresholdBW--;
}