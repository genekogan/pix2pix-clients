#pragma once

#include "ofMain.h"
#include "FaveButtonSettings.h"
#include "FaveButtonEvent.h"



class FavoritesThumbnail {
public:
    FavoritesThumbnail();
    void load(string path);
    void save(string path);
    void resize(int w, int h);
    void setFromTexture(ofTexture * texture);
    void draw();
    void setPosition(int x, int y);
    
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y);
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y);
    
    void buttonClicked();

protected:
    bool isHover, isPressed;
    ofImage thumb;
    ofRectangle rect;
    FaveButtonSettings settings;
};
