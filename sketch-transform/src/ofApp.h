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


        void setupMain();
    		void drawMain(ofEventArgs & args);


    
    void drawDebug();
    void drawPresent();
    void drawFavorites();

    void keyPressed(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void sendToRunway();
    void receiveFromRunway();

    void setModel(string model_name, int which_epoch);
    void epochChanged(int & e);

    ofxRunway runway;

    ofImage src;
    ofImage img;
    ofImage input;
//    ofImage output;

  ofTexture output;

	ofxCanvas canvas;

    int width;
    int height;
    int mode;
    bool toSend;
    bool ready;
    
    ofxPanel gui;
    ofParameter<int> epoch;



    void touchMoved(int x, int y, int id) {
      cout << "TM " << x << "  "<< y << endl;
    }


    void saveFavorite();

    vector<ofImage> favorites;
    

};
