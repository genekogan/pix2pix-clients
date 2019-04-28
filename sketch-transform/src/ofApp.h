#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxCanvas.h"
#include "ofxHTTP.h"
#include "ofxRunway.h"
#include "Faves.h"

using namespace cv;
using namespace ofxCv;

//#define PRESENT
#define TEST_MODE


class ofApp : public ofBaseApp{
public:
    void setup();
    void setupMain();
    void update();
    void draw();
    void drawMain(ofEventArgs & args);
    void drawDebug();
    void drawUserView();
    void drawPresent();
    void drawFavorites();

    void goToDrawScreen() {mode=0;}
    void goToFavesScreen() {mode=2;}

    void keyPressed(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void fullscreenCheck1();
    void fullscreenCheck2();

    void sendToRunway();
    void receiveFromRunway();
    void setModel(string model_name, int which_epoch);
    void saveFavorite();

    void selectCheckpoint_ngf1_30() {setModel("ngf1", 30);}
    void selectCheckpoint_ngf1_60() {setModel("ngf1", 60);}
    void selectCheckpoint_ngf9_60() {setModel("ngf9", 60);}
    
    ofxPanel gui;
    ofxButton bCkpt_ngf1_30;
    ofxButton bCkpt_ngf1_60;
    ofxButton bCkpt_ngf9_60;
    NavigateButton goToFaves;
    NavigateButton goToDraw;
    NavigateButton saveFave;
    
    ofxRunway runway;
    Favorites faves;
    
    ofImage src;
    ofImage img;
    ofImage input;
    ofTexture output;
	ofxCanvas canvas;
    ofTrueTypeFont font;

    int width;
    int height;
    int mode;
    bool toSend;
    bool ready;
    bool bFullscreen1;
    bool bFullscreen2;
    
};
