//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxSandboxTracker.h"
#include "ofxHTTP.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    
    void update() override;
    void draw() override;
    void draw2() ;

    void sendInput() ;
    
    void keyPressed(int key) override;

    ofImage img;



    ofxSandboxTracker sandbox;
    
    int numTrackingColors = 3;
    int width, height;
    int srcMode = 2;
    
    ofVideoGrabber cam;
    ofVideoPlayer video;
    ofImage src;

    bool debug;


	ofxPanel panel;


    ofFbo fbo;
    ofFloatImage img_in, img_out;
    ofPixels pix_in;

};
