#include "ofMain.h"

#include "FaveButtonSettings.h"
#include "FaveButton.h"
#include "FaveButtonEvent.h"


class Favorites {

public:

    Favorites();
    
    void setup();
    
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
    
protected:

    vector<string> paths;
    vector<FavoritesThumbnail> favorites;
    ofImage main;

    int favesW;
    int iw, ih, im;
    int nc, nr;
    int p1, p2;
    int nPages, page;

};

