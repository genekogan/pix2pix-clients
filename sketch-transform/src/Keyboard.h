#pragma once

#include "ofMain.h"
#include "ofxOnscreenKeyboard.h"


class FavoritesKeyboard : public ofxOnscreenKeyboard {
public:
    FavoritesKeyboard();
    void loadInputFont(string fontPath, int fontSize);
    void setMessageString(string statusMsg, float seconds);
    void drawMessageString();
    void drawAll();
    void setInputPosition(int x, int y, int w, int h);
    bool getInsideKeyboard(int x, int y);
    bool mouseMoved(int x, int y);
    bool mouseDragged(int x, int y);
    bool mousePressed(int x, int y);
    bool mouseReleased(int x, int y);

    ofxClickable bCancel;
    ofxClickable bSave;

protected:
    
    ofRectangle inputRect;
    ofTrueTypeFont fontI;
    float t0, dt;
    string statusMsg;
    
};
