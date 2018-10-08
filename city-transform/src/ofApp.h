#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxSandboxTracker.h"
#include "ofxHTTP.h"
#include "ofxRunway.h"


class ofApp: public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void updateSandbox();
    
    void keyPressed(int key) override;

    ofxRunway runway;
    ofxSandboxTracker sandbox;
    
    string host;
    int numTrackingColors;
    int width;
    int height;
    int srcWidth;
    int srcHeight;
    int srcMode;
    bool isReady;
    bool debug;
    
    ofVideoGrabber cam;
    ofVideoPlayer video;

    ofImage src;
    ofImage input, output;
    
    ofTexture outputTex;
};
