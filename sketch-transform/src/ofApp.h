#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxCanvas.h"
#include "ofxHTTP.h"
#include "ofxRunway.h"


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    ofxRunway runway;
    
    string host;
    int width;
    int height;
    bool isReady;
    bool debug;
    
    ofImage src;
    ofImage input, output;
    
    ofTexture outputTex;

	ofxCanvas canvas;
    ofImage inputGray;
    
    bool toSend = false;
    
    ofImage img;
};
