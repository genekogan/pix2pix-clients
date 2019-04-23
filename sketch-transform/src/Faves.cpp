#include "Faves.h"


//--------------------------------------------------------------
void Favorites::mouseMoved(int x, int y) {
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].mouseMoved(x, y);
    }
}
//--------------------------------------------------------------
void Favorites::mouseDragged(int x, int y) {
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].mouseDragged(x, y);
    }
}

//--------------------------------------------------------------
void Favorites::mousePressed(int x, int y) {
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].mousePressed(x, y);
    }
}

//--------------------------------------------------------------
void Favorites::mouseReleased(int x, int y) {
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].mouseReleased(x, y);
    }
}

//--------------------------------------------------------------
Favorites::Favorites() {
    setup();
}

//--------------------------------------------------------------
void Favorites::setup() {
    favesW = ofGetWidth();
    iw = 360;
    ih = 180;
    im = 20;
    
    nc = int(float(favesW - im) / (iw + im));
    page = -1;
    p1 = 0;
    p2 = 0;
    
    getPaths();
    updateCounts();
    loadPage(0);
    updateThumbnailPositions();
    
    ofAddListener(FaveButtonEvent::events, this, &Favorites::buttonEvent);
}

//--------------------------------------------------------------
void Favorites::updateCounts() {
    nr = 0;
    nPages = 0;
    if (paths.size() > 0) {
        nr = ceil(float(paths.size()) / nc);
        nPages = paths.size() / (nc * nr);
    }
}

//--------------------------------------------------------------
void Favorites::loadPage(int p) {
    if (page == p) return;
    if (p * nc * nr > paths.size()) return;
    page = p;
    favorites.clear();
    p1 = page * nc * nr;
    p2 = min((int) paths.size(), (page + 1) * nc * nr);
    for (int i=p1; i<p2; i++) {
        FavoritesThumbnail thumb;
        thumb.load(paths[i]);
        thumb.resize(iw, ih);
        favorites.push_back(thumb);
    }
    updateCounts();
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
    newFave.save(newPath);
    paths.push_back(newPath);
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
        float y = im + iy * (ih + im);
        favorites[f].setPosition(x, y);
    }
}

//--------------------------------------------------------------
void Favorites::draw() {
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].draw();
    }
}

//--------------------------------------------------------------
void Favorites::drawPresent() {
    if (main.isAllocated()) {
        main.draw(0, 0);
    }
}

//--------------------------------------------------------------
void Favorites::buttonEvent(FaveButtonEvent &e) {
    main.load(e.settings.path);
}
