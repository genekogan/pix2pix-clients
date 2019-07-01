#include "Faves.h"


//--------------------------------------------------------------
ofEvent <FaveButtonEvent> FaveButtonEvent::events;

//--------------------------------------------------------------
void FavoritesThumbnail::loadIcon(string path) {
    this->iconPath = path;

    icon.load(iconPath);
    icon.crop(512, 0, 512, 512);

    imgIn.load(iconPath);
    imgIn.crop(0, 0, 512, 512);
    
    settings.path = path;
    settings.imgIn = getInputImage();
}

//--------------------------------------------------------------
void FavoritesThumbnail::saveIcon(string path) {
    ofxClickable::saveIcon(path);
    settings.path = path;
    settings.imgIn = getInputImage();
}

//--------------------------------------------------------------
void FavoritesThumbnail::draw() {
    ofxClickable::draw();
    ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString(getDrawerName(), rect.getX()+5, rect.getY()+rect.getHeight()+20);
    ofPopStyle();
}

//--------------------------------------------------------------
void FavoritesThumbnail::buttonClicked() {
    static FaveButtonEvent newEvent;
    newEvent.settings = settings;
    ofNotifyEvent(FaveButtonEvent::events, newEvent);
}

//--------------------------------------------------------------
Favorites::Favorites() {
    isSetup = false;
}

//--------------------------------------------------------------
void Favorites::setup(int iw_, int ih_, int im_, int marginTop_) {
    isSetup = true;
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
    
    font.load("verdana.ttf", 48);
    
    prev.setBackgroundColor(ofColor::lightGray);
    prev.setBackgroundGradient(50);
    prev.setCornerRounded(5);
    prev.loadIcon("back.png");
    prev.resize(80, 80);
    prev.setPosition(40, 10);
    
    next.setBackgroundColor(ofColor::lightGray);
    next.setBackgroundGradient(50);
    next.setCornerRounded(5);
    next.loadIcon("forward.png");
    next.resize(80, 80);
    next.setPosition(250, 10);
    
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
        cout << " - > get counts " << favesH << " " << ih << " " << im << " " << " = " << nr <<endl;
        nPages = max(1, (int) ceil(float(paths.size()) / (nc * nr)));
    }


    cout << "updat ecounts " << paths.size() << " " << nr << " " << nPages << endl;

    
}

//--------------------------------------------------------------
void Favorites::loadPage(int p) {
    if (page == p) return;
    //if (p * nc * nr > paths.size()) return;
    if (p < 0 || p > nPages-1) return;
    page = p;
    p1 = page * nc * nr;
    p2 = min((int) paths.size(), (page + 1) * nc * nr);
    favorites.clear();
    for (int i=p1; i<p2; i++) {
        vector<string> pathParts = ofSplitString(paths[i], "_");
        string drawerName = pathParts[pathParts.size()-2];
        FavoritesThumbnail thumb;
        thumb.setup(drawerName, 0, 0, iw, ih);
        thumb.loadIcon(paths[i]);
        thumb.setDrawerName(drawerName);
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
    cout << "LETS GET PATHS! " << endl;
    int n = dir.listDir();
    for (int i=0; i<n; i++) {
        if (!dir.getFile(i).isDirectory()){
            string path = dir.getFile(i).getAbsolutePath();
            paths.push_back(path);
            cout << "path " << path << endl;
        }
    }
}

//--------------------------------------------------------------
void Favorites::add(ofTexture * texture, string name) {
    string newPath = "favorites/favorites_"+name+"_"+ofGetTimestampString()+".png";
    
    FavoritesThumbnail newFave;
    newFave.setFromTexture(texture);
    newFave.setDrawerName(name);
    ofImage imgIn;
    texture->readToPixels(imgIn);
    imgIn.update();
    imgIn.crop(0, 0, 512, 512);
    newFave.setInputImage(&imgIn);
    
    newFave.saveIcon(newPath);
    paths.push_back(newPath);
    
    ofLog() << "new path "<<newPath;
    int pIdx = paths.size()-1;
    p2 = min((int) paths.size(), (page + 1) * nc * nr);
    
    if (pIdx >= p1 && pIdx < p2) {
        
        
        cout << "NEWFAVE SIZES " << newFave.getIcon().getWidth() << " " << newFave.getIcon().getHeight()<<endl;
        
        
        
        newFave.crop(512, 0, 512, 512);
        cout << "NEWFAVE SIZES OK " << newFave.getIcon().getWidth() << " " << newFave.getIcon().getHeight()<<endl;
        newFave.resize(iw, ih);
        cout << "NEWFAVE SIZES NOW " << newFave.getIcon().getWidth() << " " << newFave.getIcon().getHeight()<<endl;
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
    
    if (nPages >= 0) {
        ofSetColor(ofColor::white);
        prev.draw();
        next.draw();
        int pw = font.getStringBoundingBox(pageStr, 0, 0).getWidth();
        int ph = font.getStringBoundingBox(pageStr, 0, 0).getHeight();
        int px = prev.getRectangle().getX() + prev.getRectangle().getWidth() + 0.5 * (next.getRectangle().getX() - (prev.getRectangle().getX() + prev.getRectangle().getWidth()) - pw);
        int py = prev.getRectangle().getY() + ph + 0.5 * (prev.getRectangle().getHeight() - ph);
        font.drawString(pageStr, px, py);
    }
    
    for (int f=0; f<favorites.size(); f++) {
        favorites[f].draw();
    }
}

//--------------------------------------------------------------
void Favorites::drawPresent() {
    if (main.isAllocated()) {
        float aspect = main.getWidth() / main.getHeight();
        float w, h;
        if (float(ofGetWidth()) / ofGetHeight() > aspect) {
            h = ofGetHeight() - 20;
            w = int(float(h) * aspect);
        } else {
            w = ofGetWidth() - 20;
            h = int(float(w) / aspect);
        }
        int x = int(0.5 * (ofGetWidth() - w));
        int y = int(0.5 * (ofGetHeight() - h));
        main.draw(x, y, w, h);
    }
}

//--------------------------------------------------------------
void Favorites::buttonEvent(FaveButtonEvent &e) {
    main.load(e.settings.path);
    main.crop(512, 0, 512, 512);
    ofImage newCanvas;
    newCanvas.setFromPixels(e.settings.imgIn);
    newCanvas.resize(canvas->getRectangle().getWidth(), canvas->getRectangle().getHeight());
    canvas->setFromPixels(newCanvas);
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
