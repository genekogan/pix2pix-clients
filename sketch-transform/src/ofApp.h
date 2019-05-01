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

#define PRESENT
//#define TEST_MODE


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

  //  void selectCheckpoint_ngf1_30() {setModel("ngf1", 30);}
 //   void selectCheckpoint_ngf1_60() {setModel("ngf1", 60);}
//    void selectCheckpoint_ngf9_60() {setModel("ngf9", 60);}

    void selectCheckpoint_landscape_sk_curated_50_10() {setModel("landscape_sk_curated_50", 10);}
    void selectCheckpoint_landscape_sk_curated_50_40() {setModel("landscape_sk_curated_50", 40);}
    void selectCheckpoint_landscape_sk_curated_50_80() {setModel("landscape_sk_curated_50", 80);}
    void selectCheckpoint_landscape_sk_curated_segcolored_10() {setModel("landscape_sk_curated_segcolored", 10);}
    void selectCheckpoint_landscape_sk_curated_segcolored_40() {setModel("landscape_sk_curated_segcolored", 40);}
    void selectCheckpoint_landscape_sk_curated_segcolored_70() {setModel("landscape_sk_curated_segcolored", 70);}
    void selectCheckpoint_landscape_sk_curated_small_250_50() {setModel("landscape_sk_curated_small_250", 50);}
    void selectCheckpoint_landscape_sk_curated_small_250_150() {setModel("landscape_sk_curated_small_250", 150);}
    void selectCheckpoint_landscape_sk_curated_small_250_300() {setModel("landscape_sk_curated_small_250", 300);}
    void selectCheckpoint_landscape_sk_curated_small_250_400() {setModel("landscape_sk_curated_small_250", 400);}
    void selectCheckpoint_landscape_sk_curated_small_250_500() {setModel("landscape_sk_curated_small_250", 500);}
    void selectCheckpoint_landscape_sk_curated_transfer_50_10() {setModel("landscape_sk_curated_transfer_50", 10);}
    void selectCheckpoint_landscape_sk_curated_transfer_50_40() {setModel("landscape_sk_curated_transfer_50", 40);}
    void selectCheckpoint_landscape_sk_curated_transfer_50_100() {setModel("landscape_sk_curated_transfer_50", 100);}
    
    ofxPanel gui;
//    ofxButton bCkpt_ngf1_30;
 //   ofxButton bCkpt_ngf1_60;
  //  ofxButton bCkpt_ngf9_60;
    NavigateButton goToFaves;
    NavigateButton goToDraw;
    NavigateButton saveFave;
    


    ofxButton bCkpt_landscape_sk_curated_50_10;
    ofxButton bCkpt_landscape_sk_curated_50_40;
    ofxButton bCkpt_landscape_sk_curated_50_80;
    ofxButton bCkpt_landscape_sk_curated_segcolored_10;
    ofxButton bCkpt_landscape_sk_curated_segcolored_40;
    ofxButton bCkpt_landscape_sk_curated_segcolored_70;
    ofxButton bCkpt_landscape_sk_curated_small_250_50;
    ofxButton bCkpt_landscape_sk_curated_small_250_150;
    ofxButton bCkpt_landscape_sk_curated_small_250_300;
    ofxButton bCkpt_landscape_sk_curated_small_250_400;
    ofxButton bCkpt_landscape_sk_curated_small_250_500;
    ofxButton bCkpt_landscape_sk_curated_transfer_50_10;
    ofxButton bCkpt_landscape_sk_curated_transfer_50_40;
    ofxButton bCkpt_landscape_sk_curated_transfer_50_100;


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
    int cWidth;
    int cHeight;
    int mode;
    bool toSend;
    bool ready;
    bool bFullscreen1;
    bool bFullscreen2;

    bool toSendManual = false;
};
