#include "ofMain.h"
#include "ofxClickable.h"
#include "ofxCanvas.h"


//--------------------------------------------------------------
class FavoritesThumbnail;
struct FaveButtonSettings {
    string path;
    string inputPath;
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
    void buttonClicked();
    void setDrawerName(string drawerName) {this->drawerName = drawerName;}
    string getDrawerName() {return drawerName;}
    string getInputPath() {return inputPath;}
    void draw();

protected:
    FaveButtonSettings settings;
    string inputPath;
    string drawerName;
};


//--------------------------------------------------------------
class Favorites {
public:
    string inputPath_;
    Favorites();
    
    void setup(int iw_, int ih_, int im_, int marginTop_, int width_, int height_);
    void setCanvasReference(ofxCanvas *canvas) {this->canvas = canvas;}
    void setCanvasOverwrite(bool canvasOverwrite_);

    void add(ofImage * inputImage, ofImage * outputImage, string name);
    void selectMain(int idx);
    void getPaths();
    bool getIsSetup() {return isSetup;}
    
    void loadPage(int p);
    void nextPage();
    void prevPage();
    
    void draw();
    void drawPresent();

    void selectRandom();
    
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y);
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y);

    void buttonEvent(FaveButtonEvent &e);
    void prevEvent() {prevPage();}
    void nextEvent() {nextPage();}
    
protected:
    
    void resizeMain();
    void overwriteCanvas(string path);
    void updateCounts();
    void updateThumbnailPositions();
    void checkIfPrevNextAvailable();
    string convertDrawerName(string oldDrawerName);
    
    vector<string> paths;
    vector<FavoritesThumbnail> favorites;
    ofxClickable prev;
    ofxClickable next;
    ofTrueTypeFont font, font2;
    ofImage main;
    int mx, my;

    bool canvasOverwrite;
    int favesW, favesH;
    int iw, ih, im, marginTop;
    int nc, nr;
    int p1, p2;
    int nPages, page;
    bool isSetup;
    int width, height;

    ofxCanvas *canvas;
};

