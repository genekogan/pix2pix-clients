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

    ofImage src;
    ofImage img;
    ofImage input, output;
    ofImage inputGray;
    ofTexture outputTex;
	ofxCanvas canvas;

    int width;
    int height;
    bool debug;
    bool toSend;
    bool ready;
    
};
