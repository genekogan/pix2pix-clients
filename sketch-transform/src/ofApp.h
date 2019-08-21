#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxClickable.h"
#include "ofxCanvas.h"
#include "ofxHTTP.h"
#include "ofxRunway.h"
#include "Faves.h"
#include "Keyboard.h"
#include "TinyTimer.h"

using namespace cv;
using namespace ofxCv;

#define PRESENT
#define TEST_MODE
//#define DEBUG_VIEW


enum SketchMode {
    PRESENTATION,
    FAVORITES
};


class TemplateThumbnail : public ofxClickable {
public:
    void buttonClicked() {ofNotifyEvent(event, iconPath, this);}
    ofEvent<string> event;
    static ofEvent<FaveButtonEvent> events;
};


class ofApp : public ofBaseApp{
public:
    
    void faveButtonEvent(FaveButtonEvent &e);
    
    
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

    void goToDrawScreen() {mode = PRESENTATION;}
    void goToFavesScreen() {mode = FAVORITES;}

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
    void selectRandomFavorite();
    void saveFavoritePrompt();
    void saveFavorite(string name);
    void saveTemplate();
    void meander();

    void canvasPanelEvent(ofxCanvasButtonEvent &e);
    void keyboardCancelEvent();
    void keyboardSaveEvent();
    void timerUpEvent();
    
    ofxPanel gui;
    ofxClickable goToFaves;
    ofxClickable goToDraw;
    ofxClickable saveFave;
    ofxClickable randomFave;
    TinyTimer saveFaveTimer;
    
    ofxRunway runway;
    Favorites faves;
    FavoritesKeyboard keyboard;
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
    int cX;
    int cY;
    int cWidth;
    int cHeight;
    SketchMode mode;
    bool toSend;
    bool toClearCanvas;
    bool bFullscreen1;
    bool bFullscreen2;
    
    bool autoMeander;
    bool meanderMouseActive;
    float meanderNoise1, meanderNoise2;
};
