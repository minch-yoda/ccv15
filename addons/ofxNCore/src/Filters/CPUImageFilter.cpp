/*
*  CPUImageFilter.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#include "CPUImageFilter.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvFloatImage.h"

//--------------------------------------------------------------------------------
void CPUImageFilter::amplify ( CPUImageFilter& mom, float level ) {

	float scalef = level / 128.0f;
	cvMul( mom.getCvImage(), mom.getCvImage(), cvImageTemp, scalef );
	swapTemp();
	flagImageChanged();
}

void CPUImageFilter::highpass ( float blur1, float blur2 ) {

	//Blur Original Image
	if(blur1 > 0)
	cvSmooth( cvImage, cvImageTemp, CV_BLUR , (blur1 * 2) + 1);

	//Original Image - Blur Image = Highpass Image
	cvSub( cvImage, cvImageTemp, cvImageTemp );

	//Blur Highpass to remove noise
	if(blur2 > 0)
	cvSmooth( cvImageTemp, cvImageTemp, CV_BLUR , (blur2 * 2) + 1);

	swapTemp();
	flagImageChanged();
}

//--------------------------------------------------------------------------------
void CPUImageFilter::operator =	( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void CPUImageFilter::operator = ( const ofxCvGrayscaleImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any chages
        ofxCvGrayscaleImage& mom = const_cast<ofxCvGrayscaleImage&>(_mom); 
            
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );           
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
        }
    } else {
        ofLog(OF_LOG_WARNING, "in =, you are assigning a ofxCvGrayscaleImage to itself");
    }
}

//--------------------------------------------------------------------------------
void CPUImageFilter::operator = ( const ofxCvColorImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom); 
	if( matchingROI(getROI(), mom.getROI()) ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_RGB2GRAY );
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void CPUImageFilter::operator = ( const ofxCvFloatImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom); 
	if( matchingROI(getROI(), mom.getROI()) ) {
		//cvConvertScale( mom.getCvImage(), cvImage, 1.0f, 0);
        cvConvert( mom.getCvImage(), cvImage );        
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}