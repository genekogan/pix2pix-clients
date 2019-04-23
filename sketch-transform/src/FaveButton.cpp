#include "FaveButton.h"



//--------------------------------------------------------------
FavoritesThumbnail::FavoritesThumbnail() {
    rect = ofRectangle(0, 0, 0, 0);
    isHover = false;
    isPressed = false;
}

//--------------------------------------------------------------
void FavoritesThumbnail::load(string path) {
    thumb.load(path);
    settings.path = path;
}

//--------------------------------------------------------------
void FavoritesThumbnail::save(string path) {
    thumb.save(path);
}

//--------------------------------------------------------------
void FavoritesThumbnail::resize(int w, int h) {
    rect.setWidth(w);
    rect.setHeight(h);
    thumb.resize(rect.getWidth(), rect.getHeight());
}

//--------------------------------------------------------------
void FavoritesThumbnail::setFromTexture(ofTexture * texture) {
    texture->readToPixels(thumb);
}

//--------------------------------------------------------------
void FavoritesThumbnail::draw() {
    if (isHover) {
        ofPushStyle();
        ofSetColor(ofColor::green);
        ofDrawRectangle(rect.getX()-4, rect.getY()-4, rect.getWidth()+8, rect.getHeight()+8);
        ofPopStyle();
    }
    if (isPressed) {
        ofPushStyle();
        ofSetColor(ofColor::orange);
        ofDrawRectangle(rect.getX()-4, rect.getY()-4, rect.getWidth()+8, rect.getHeight()+8);
        ofPopStyle();
    }
    
    thumb.draw(rect.getX(), rect.getY());
}

//--------------------------------------------------------------
void FavoritesThumbnail::setPosition(int x, int y) {
    rect.setX(x);
    rect.setY(y);
}

//--------------------------------------------------------------
void FavoritesThumbnail::mouseMoved(int x, int y){
    isHover = rect.inside(x, y);
}

//--------------------------------------------------------------
void FavoritesThumbnail::mouseDragged(int x, int y){
    
}

//--------------------------------------------------------------
void FavoritesThumbnail::mousePressed(int x, int y){
    if (isHover) {
        isPressed = true;
    }
}

//--------------------------------------------------------------
void FavoritesThumbnail::mouseReleased(int x, int y){
    if (isPressed) {
        isPressed = false;
        buttonClicked();
    }
}

//--------------------------------------------------------------
void FavoritesThumbnail::buttonClicked() {
    static FaveButtonEvent newEvent;
    newEvent.settings = settings;
    ofNotifyEvent(FaveButtonEvent::events, newEvent);
}
