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
        float t1 = ofGetElapsedTimef();
        float dt = t1 - t0;
        if (dt >= duration) {
            active = false;
            ofNotifyEvent(event);
        }
    }

    ofEvent<void> event;

protected:

    float t0;
    float duration;
    bool active;
    
};
