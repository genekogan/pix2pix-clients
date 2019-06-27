#include "Keyboard.h"


//--------------------------------------------------------------
FavoritesKeyboard::FavoritesKeyboard() : ofxOnscreenKeyboard() {
    //        bCancel.setName("Cancel");
    //        bCancel.setFont(&font);
    //        bCancel.setBackgroundColor(ofColor::lightGray);
    //        bCancel.setBackgroundGradient(85);
    //        bCancel.setCornerRounded(5);
    
    bSave.setName("Save");
    bSave.setFont(&font);
    bSave.setBackgroundColor(ofColor::lightGray);
    bSave.setBackgroundGradient(85);
    bSave.setCornerRounded(5);
    
    dt = 0;
}

//--------------------------------------------------------------
void FavoritesKeyboard::loadInputFont(string fontPath, int fontSize) {
    fontI.load(fontPath, fontSize);
    bSave.setFont(&fontI);
}

//--------------------------------------------------------------
void FavoritesKeyboard::setMessageString(string statusMsg, float seconds) {
    this->statusMsg = statusMsg;
    this->dt = seconds;
    t0 = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void FavoritesKeyboard::drawMessageString() {
    int sw = fontI.getStringBoundingBox(statusMsg, 0, 0).getWidth();
    int x = inputRect.getX() + int(0.5 * (inputRect.getWidth() - sw));
    ofSetColor(0, 0, 0, 200);
    ofDrawRectRounded(inputRect, 5);
    ofSetColor(255);
    fontI.drawString(statusMsg, x, inputRect.getY() + inputRect.getHeight() - 20);
}

//--------------------------------------------------------------
void FavoritesKeyboard::drawAll() {
    float t1 = ofGetElapsedTimef();
    if (t1 - t0 < dt) {
        drawMessageString();
    }
    
    if (!getVisible()) return;
    
    ofPushStyle();
    
    ofFill();
    ofSetColor(0, 80);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    ofSetColor(255);
    draw();
    
    ofSetColor(0, 0, 0, 200);
    ofDrawRectRounded(inputRect, 5);
    
    ofSetColor(255);
    fontI.drawString(input, inputRect.getX()+20, inputRect.getY() + inputRect.getHeight()-20);
    
    //bCancel.draw();
    bSave.draw();
    
    ofPopStyle();
}

//--------------------------------------------------------------
void FavoritesKeyboard::setInputPosition(int x, int y, int w, int h) {
    inputRect.set(x, y, w, h);
    bSave.resize(160, h - 20);
    bSave.setPosition(x + w - 166, y + 10);
    //bCancel.resize(140, h-10);
    //bCancel.setPosition(x + w - 110, y + 5);
}

//--------------------------------------------------------------
bool FavoritesKeyboard::getInsideKeyboard(int x, int y) {
    bool insideKeys = bgRect.inside(x, y);
    bool insideInput = inputRect.inside(x, y);
    return insideKeys || insideInput;
}

//--------------------------------------------------------------
bool FavoritesKeyboard::mouseMoved(int x, int y) {
    if (!visible) return false;
    ofxOnscreenKeyboard::mouseMoved(x, y);
    bSave.mouseMoved(x, y);
    //bCancel.mouseMoved(x, y);
    return getInsideKeyboard(x, y);
}

//--------------------------------------------------------------
bool FavoritesKeyboard::mouseDragged(int x, int y) {
    if (!visible) return false;
    ofxOnscreenKeyboard::mouseDragged(x, y);
    bSave.mouseDragged(x, y);
    //bCancel.mouseDragged(x, y);
    return getInsideKeyboard(x, y);
}

//--------------------------------------------------------------
bool FavoritesKeyboard::mousePressed(int x, int y) {
    if (!visible) return false;
    ofxOnscreenKeyboard::mousePressed(x, y);
    bSave.mousePressed(x, y);
    //bCancel.mousePressed(x, y);
    return getInsideKeyboard(x, y);
}

//--------------------------------------------------------------
bool FavoritesKeyboard::mouseReleased(int x, int y) {
    if (!visible) return false;
    ofxOnscreenKeyboard::mouseReleased(x, y);
    bSave.mouseReleased(x, y);
    //bCancel.mouseReleased(x, y);
    return getInsideKeyboard(x, y);
}
