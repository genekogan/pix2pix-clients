#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxClickable.h"
#include "ofxCanvas.h"
#include "ofxHTTP.h"
#include "ofxRunway.h"
#include "Faves.h"

using namespace cv;
using namespace ofxCv;

#define PRESENT
//#define TEST_MODE


class TemplateThumbnail : public ofxClickable {
public:
    void buttonClicked() {ofNotifyEvent(event, iconPath, this);}
    ofEvent<string> event;
    static ofEvent<FaveButtonEvent> events;
};


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
    void drawTemplates();

    void loadTemplates();
    void templateClicked(string & templatePath);

    void goToDrawScreen() {mode=0;}
    void goToFavesScreen() {mode=2;}

    void keyPressed(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void mouseMovedTemplates(int mx, int my);
    void mousePressedTemplates(int mx, int my);
    void mouseDraggedTemplates(int mx, int my);
    void mouseReleasedTemplates(int mx, int my);

    void fullscreenCheck1();
    void fullscreenCheck2();

    void sendToRunway();
    void receiveFromRunway();
    void setModel(string model_name, int which_epoch);
    void saveFavorite();
    void saveTemplate();

    ofxPanel gui;
    ofxClickable goToFaves;
    ofxClickable goToDraw;
    ofxClickable saveFave;
    
    ofxRunway runway;
    Favorites faves;
    vector<TemplateThumbnail*> templates;
    
    ofImage src;
    ofImage img;
    ofImage input;
    ofTexture output;
	
    ofxCanvas canvas;
    ofxCanvasPanel panelTop, panelLeft;
    
    ofTrueTypeFont font;

    int width;
    int height;
    int cWidth;
    int cHeight;
    int mode;
    bool toSend;
    bool bFullscreen1;
    bool bFullscreen2;

    bool toSendManual = false;


    void canvasPanelEvent(ofxCanvasButtonEvent &e);
    bool toClearCanvas;
    
};
