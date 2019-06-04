#include "Faves.h"


//--------------------------------------------------------------
ofEvent <FaveButtonEvent> FaveButtonEvent::events;

//--------------------------------------------------------------
void FavoritesThumbnail::loadIcon(string path) {
    ofxClickable::loadIcon(path);
    settings.path = path;
}

//--------------------------------------------------------------
void FavoritesThumbnail::saveIcon(string path) {
    ofxClickable::saveIcon(path);
    settings.path = path;
}

//--------------------------------------------------------------
void FavoritesThumbnail::buttonClicked() {
    static FaveButtonEvent newEvent;
    newEvent.settings = settings;
    ofNotifyEvent(FaveButtonEvent::events, newEvent);
}

//--------------------------------------------------------------
Favorites::Favorites() {
    setup(320, 160, 24, 90);
}

//--------------------------------------------------------------
void Favorites::setup(int iw_, int ih_, int im_, int marginTop_) {
    favesW = ofGetWidth();
    favesH = ofGetHeight() - marginTop;
    
    iw = iw_;
    ih = ih_;
    im = im_;
    marginTop = marginTop_;
    
    nc = int(float(favesW - im) / (iw + im));
    page = -1;
    p1 = 0;
    p2 = 0;
    
    getPaths();
    updateCounts();
    loadPage(0);
    updateThumbnailPositions();
    
    font.load("verdana.ttf", 36);
    prev.loadIcon("back.png");
    next.loadIcon("forward.png");
    prev.resize(64, 64);
    next.resize(64, 64);
    prev.setPosition(24, 5);
    next.setPosition(180, 5);

    ofAddListener(FaveButtonEvent::events, this, &Favorites::buttonEvent);
    ofAddListener(prev.clickEvent, this, &Favorites::prevEvent);
    ofAddListener(next.clickEvent, this, &Favorites::nextEvent);
}

//--------------------------------------------------------------
void Favorites::updateCounts() {
    nr = 0;
    nPages = 0;
    if (paths.size() > 0) {
        nr = int(favesH / (ih + im));//ceil(float(paths.size()) / nc);
        nPages = ceil(float(paths.size()) / (nc * nr));
    }
}

//--------------------------------------------------------------
void Favorites::loadPage(int p) {
    if (page == p) return;
    if (p * nc * nr > paths.size()) return;
    page = p;
    p1 = page * nc * nr;
    p2 = min((int) paths.size(), (page + 1) * nc * nr);
    favorites.clear();
    for (int i=p1; i<p2; i++) {
        FavoritesThumbnail thumb;
        thumb.loadIcon(paths[i]);
        thumb.resize(iw, ih);
        favorites.push_back(thumb);
    }
    updateThumbnailPositions();
}

//--------------------------------------------------------------
void Favorites::nextPage() {
    loadPage(page+1);
}

//--------------------------------------------------------------
void Favorites::prevPage() {
    loadPage(page-1);
}

//--------------------------------------------------------------
void Favorites::getPaths() {
    paths.clear();
    ofDirectory dir("favorites");
    int n = dir.listDir();
    for (int i=0; i<n; i++) {
        if (!dir.getFile(i).isDirectory()){
            string path = dir.getFile(i).getAbsolutePath();
            paths.push_back(path);
        }
    }
}

//--------------------------------------------------------------
void Favorites::add(ofTexture * texture) {
    FavoritesThumbnail newFave;
    newFave.setFromTexture(texture);
    string newPath = "favorites/favorites"+ofGetTimestampString()+".png";
    newFave.saveIcon(newPath);
    paths.push_back(newPath);
    ofLog() << "new path "<<newPath;
    int pIdx = paths.size()-1;
    p2 = min((int) paths.size(), (page + 1) * nc * nr);
    if (pIdx >= p1 && pIdx < p2) {
        newFave.resize(iw, ih);
        favorites.push_back(newFave);
    }
    updateCounts();
    updateThumbnailPositions();
}

//--------------------------------------------------------------
void Favorites::updateThumbnailPositions() {
    for (int f=0; f<favorites.size(); f++) {
        int ix = f % nc;
        int iy = int(f / nc);
        float x = im + ix * (iw + im);
        float y = marginTop + im + iy * (ih + im);
        favorites[f].setPosition(x, y);
    }
}

//--------------------------------------------------------------
void Favorites::draw() {
    string pageStr = ofToString(page+1)+"/"+ofToString(nPages);
    
    ofSetColor(255);
    prev.draw();
    next.draw();
    font.drawString(pageStr, 90, 52);
    
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].draw();
    }
}

//--------------------------------------------------------------
void Favorites::drawPresent() {
    
    if (main.isAllocated()) {
        int w = ofGetWidth() - 20;
        int h = int(float(w) / (main.getWidth() / main.getHeight()));
        int y = int(0.5 * (ofGetHeight() - h));
        main.draw(10, y, w, h);
    }
}

//--------------------------------------------------------------
void Favorites::buttonEvent(FaveButtonEvent &e) {
    main.load(e.settings.path);
}

//--------------------------------------------------------------
void Favorites::mouseMoved(int x, int y) {
    prev.mouseMoved(x, y);
    next.mouseMoved(x, y);
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].mouseMoved(x, y);
    }
}
//--------------------------------------------------------------
void Favorites::mouseDragged(int x, int y) {
    prev.mouseDragged(x, y);
    next.mouseDragged(x, y);
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].mouseDragged(x, y);
    }
}

//--------------------------------------------------------------
void Favorites::mousePressed(int x, int y) {
    prev.mousePressed(x, y);
    next.mousePressed(x, y);
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].mousePressed(x, y);
    }
}

//--------------------------------------------------------------
void Favorites::mouseReleased(int x, int y) {
    prev.mouseReleased(x, y);
    next.mouseReleased(x, y);
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].mouseReleased(x, y);
    }
}
