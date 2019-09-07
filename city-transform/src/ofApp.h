#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxClickable.h"
#include "ofxCv.h"
#include "ofxSandboxTracker.h"
#include "ofxHTTP.h"
#include "ofxRunway.h"
#include "TinyTimer.h"
#include "calibrationSetting.h"


class ofApp: public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    void save();
    void exit();

    void drawDebug();
    void drawPresent();

    void checkFullscreen();
    void saveButtonClicked();
    void exitButtonClicked();
    void defaultButtonClicked();
    void mappingButtonClicked();

    void updateSandbox();
    void sendToRunway();

    void keyPressed(int key);

    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void timerUpEvent();
    
    ofxRunway runway;
    ofxSandboxTracker sandbox;
    ofxClickable saveButton;
    ofxClickable exitButton;
    ofxClickable defaultButton;
    ofxClickable mappingButton;
    
    ofTrueTypeFont font;

    string cameraHardwareName;
    int focusAbsolute;
    ofVideoGrabber cam;
    ofVideoPlayer video;
    ofImage src;
    ofImage input, output;
    ofTexture outputTex;

    int width;
    int height;
    int srcWidth;
    int srcHeight;
    int srcMode;
    bool debug;
    bool bFullscreen2;
    bool autoMode;
    TinyTimer initialDebug;
};
