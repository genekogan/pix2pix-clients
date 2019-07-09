#include "ofMain.h"
#include "ofxClickable.h"
#include "ofxCanvas.h"


//--------------------------------------------------------------
class FavoritesThumbnail;
struct FaveButtonSettings {
    string path;
    ofImage imgIn;
};


//--------------------------------------------------------------
class FaveButtonEvent : public ofEventArgs {
public:
    FaveButtonSettings settings;
    static ofEvent<FaveButtonEvent> events;
};


//--------------------------------------------------------------
class FavoritesThumbnail : public ofxClickable {
public:
    void loadIcon(string path);
    void saveIcon(string path);
    void buttonClicked();
    void setDrawerName(string drawerName) {this->drawerName = drawerName;}
    void setInputImage(ofImage *img) {imgIn.setFromPixels(img->getPixels());}
    string getDrawerName() {return drawerName;}
    ofImage getInputImage() {return imgIn;}
    void draw();

protected:
    FaveButtonSettings settings;
    ofImage imgIn;
    string drawerName;
};


//--------------------------------------------------------------
class Favorites {
public:
    Favorites();
    
    void setup(int iw_, int ih_, int im_, int marginTop_);
    void setCanvasReference(ofxCanvas *canvas) {this->canvas = canvas;}
    
    void add(ofTexture * texture, string name);
    void selectMain(int idx);
    void getPaths();
    bool getIsSetup() {return isSetup;}
    
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
    
    string convertDrawerName(string oldDrawerName);
    
    vector<string> paths;
    vector<FavoritesThumbnail> favorites;
    ofxClickable prev;
    ofxClickable next;
    ofTrueTypeFont font, font2;
    
    ofImage main;

    int favesW, favesH;
    int iw, ih, im, marginTop;
    int nc, nr;
    int p1, p2;
    int nPages, page;
    bool isSetup;

    ofxCanvas *canvas;
};

