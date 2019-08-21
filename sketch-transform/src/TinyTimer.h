#pragma once
#include "ofMain.h"



class TinyTimer {
    
public:
    
    TinyTimer() {
        ofAddListener(ofEvents().update, this, &TinyTimer::update);
    }
    
    void start(float duration) {
        this->duration = duration;
        active = true;
        t0 = ofGetElapsedTimef();
    }
    
    void update(ofEventArgs & e) {
        if (!active) return;
        t1 = ofGetElapsedTimef();
        dt = t1 - t0;
        if (dt >= duration) {
            active = false;
            ofNotifyEvent(event);
        }
    }
    
    bool getActive() {
        return active;
    }
    
    float getElapsedTime() {
        return dt;
    }
    
    ofEvent<void> event;
    
protected:
    
    float t0, t1, dt;
    float duration;
    bool active;
    
};
