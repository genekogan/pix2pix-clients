#pragma once
#include "ofMain.h"
#include "FaveButtonSettings.h"

class FaveButtonEvent : public ofEventArgs {
public:
    FaveButtonSettings settings;
    FaveButtonEvent() {}
    static ofEvent<FaveButtonEvent> events;
};
