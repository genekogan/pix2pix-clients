#include "ofMain.h"
#include "ofxClickable.h"

//--------------------------------------------------------------
struct FaveButtonSettings {
    string path;
};

//--------------------------------------------------------------
class FaveButtonEvent : public ofEventArgs {
public:
    FaveButtonSettings settings;
    static ofEvent<FaveButtonEvent> events;
};

//--------------------------------------------------------------
class NavigateButton : public ofxClickable {
public:
    void buttonClicked() {ofNotifyEvent(clickEvent, this);}
    ofEvent<void> clickEvent;
};

//--------------------------------------------------------------
class FavoritesThumbnail : public ofxClickable {
public:
    void load(string path);
    void save(string path);
    void buttonClicked();
protected:
    FaveButtonSettings settings;
};

//--------------------------------------------------------------
class Favorites {
public:
    Favorites();
    void setup(int iw_, int ih_, int im_, int marginTop_);
    void add(ofTexture * texture);
    void selectMain(int idx);
    void getPaths();
    
    void updateCounts();
    void updateThumbnailPositions();
    
    void loadPage(int p);
    void nextPage();
    void prevPage();
    
    void draw();
    void drawPresent();
    
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y);
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y);

    void buttonEvent(FaveButtonEvent &e);
    void prevEvent() {prevPage();}
    void nextEvent() {nextPage();}
    
protected:

    vector<string> paths;
    vector<FavoritesThumbnail> favorites;
    NavigateButton prev, next;
    ofTrueTypeFont font;
    
    ofImage main;

    int favesW, favesH;
    int iw, ih, im, marginTop;
    int nc, nr;
    int p1, p2;
    int nPages, page;

};

