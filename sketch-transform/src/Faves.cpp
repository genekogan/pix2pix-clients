#include "Faves.h"


//--------------------------------------------------------------
ofEvent <FaveButtonEvent> FaveButtonEvent::events;

//--------------------------------------------------------------
void FavoritesThumbnail::loadIcon(string path) {
    this->iconPath = path;

    icon.load(iconPath);
    icon.crop(512, 0, 512, 512);
    icon.resize(rect.getWidth(), rect.getHeight());

    imgIn.load(iconPath);
    imgIn.crop(0, 0, 512, 512);
    imgIn.resize(rect.getWidth(), rect.getHeight());
    
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
    ofRectangle nameRect = font->getStringBoundingBox(getDrawerName(), 0, 0);
    ofSetColor(ofColor::black, 150);
    ofDrawRectangle(rect.getX(), rect.getY(), nameRect.getWidth()+8, 40);
    ofSetColor(ofColor::white);
    font->drawString(getDrawerName(), rect.getX()+4, rect.getY()+32);
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
    canvasOverwrite = false;
}

//--------------------------------------------------------------
void Favorites::setup(int iw_, int ih_, int im_, int marginTop_) {
    isSetup = true;
    
    iw = iw_;
    ih = ih_;
    im = im_;
    marginTop = marginTop_;

    favesW = ofGetWidth();
    favesH = ofGetHeight() - marginTop;

    nc = int(float(favesW - im) / (iw + im));
    page = -1;
    p1 = 0;
    p2 = 0;
    
    getPaths();
    updateCounts();
    loadPage(0);
    updateThumbnailPositions();
    
    font.load("verdana.ttf", 48);
    font2.load("verdana.ttf", 28);
    
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
void Favorites::setCanvasOverwrite(bool canvasOverwrite_) {
    canvasOverwrite = canvasOverwrite_;
}

//--------------------------------------------------------------
void Favorites::updateCounts() {
    nr = 0;
    nPages = 0;
    if (paths.size() > 0) {
        nr = floor(favesH / (ih + im));//ceil(float(paths.size()) / nc);
        nPages = max(1, (int) ceil(float(paths.size()) / (nc * nr)));
    }
}

//--------------------------------------------------------------
void Favorites::loadPage(int p) {
    if (page == p) return;
    
    if (p < 0 || p > nPages-1) return;
    page = p;
    p1 = page * nc * nr;
    p2 = min((int) paths.size(), (page + 1) * nc * nr);

    favorites.clear();
    for (int i=p1; i<p2; i++) {
        vector<string> pathParts = ofSplitString(paths[i], "_");
        string drawerName = pathParts[pathParts.size()-2];
        drawerName = convertDrawerName(drawerName);
        FavoritesThumbnail thumb;
        thumb.setup(drawerName, 0, 0, iw, ih);
        thumb.loadIcon(paths[i]);
        thumb.setFont(&font2);
        thumb.setDrawerName(drawerName);
        thumb.resize(iw, ih);
        favorites.push_back(thumb);
    }
    
    checkIfPrevNextAvailable();
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
void Favorites::add(ofTexture * texture, string name) {
    string newPath = "favorites/favorites_"+name+"_"+ofGetTimestampString()+".png";
    
    FavoritesThumbnail newFave;
    newFave.setFromTexture(texture);
    newFave.setDrawerName(name);
    newFave.setFont(&font2);
    ofImage imgIn;
    texture->readToPixels(imgIn);
    imgIn.update();
    imgIn.crop(0, 0, 512, 512);
    newFave.setInputImage(&imgIn);
    
    newFave.saveIcon(newPath);
    paths.push_back(newPath);
    
    int pIdx = paths.size()-1;
    p2 = min((int) paths.size(), (page + 1) * nc * nr);
    
    if (pIdx >= p1 && pIdx < p2) {
        newFave.crop(512, 0, 512, 512);
        newFave.resize(iw, ih);
        favorites.push_back(newFave);
    }
    updateCounts();
    updateThumbnailPositions();
    checkIfPrevNextAvailable();

    if (paths.size() == 1) {
        nextPage();
    }
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
void Favorites::checkIfPrevNextAvailable() {
    if (page <= 0) {prev.disable();}
    else {prev.enable();}
    if (page >= nPages-1) {next.disable();}
    else {next.enable();}
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
    if (!main.isAllocated()) {
        return;
    }

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

//--------------------------------------------------------------
void Favorites::selectRandom() {
    if (favorites.size() == 0) {
        ofLog() << " there are no favorites " << favorites.size() << endl;
        return;
    }
    int rIdx = int(ofRandom(favorites.size()));
    ofImage newCanvas;
    main.load(favorites[rIdx].getIconPath());
    newCanvas.load(favorites[rIdx].getIconPath());
    main.crop(512, 0, 512, 512);



    float aspect = main.getWidth() / main.getHeight();
    float w, h;
    if (float(ofGetWidth()) / ofGetHeight() > aspect) {
        h = ofGetHeight() - 20;
        w = int(float(h) * aspect);
    } else {
        w = ofGetWidth() - 20;
        h = int(float(w) / aspect);
    }
    main.resize(w, h);

    
    //if (canvasOverwrite){
        newCanvas.crop(0, 0, 512, 512);
        newCanvas.resize(canvas->getRectangle().getWidth(), canvas->getRectangle().getHeight());
        canvas->setFromPixels(newCanvas.getPixels());
    //}
}

//--------------------------------------------------------------
void Favorites::buttonEvent(FaveButtonEvent &e) {
    ofImage newCanvas;
    main.load(e.settings.path);
    main.crop(512, 0, 512, 512);




    float aspect = main.getWidth() / main.getHeight();
    float w, h;
    if (float(ofGetWidth()) / ofGetHeight() > aspect) {
        h = ofGetHeight() - 20;
        w = int(float(h) * aspect);
    } else {
        w = ofGetWidth() - 20;
        h = int(float(w) / aspect);
    }
    main.resize(w, h);




    if (canvasOverwrite){
        newCanvas.setFromPixels(e.settings.imgIn);
        newCanvas.resize(canvas->getRectangle().getWidth(), canvas->getRectangle().getHeight());
        canvas->setFromPixels(newCanvas);
    }
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

//--------------------------------------------------------------
string Favorites::convertDrawerName(string oldDrawerName) {
    // very ugly hack to recover non-standard characters :/
    string newStr = "";
    bool isU, isO, isA, isu, iso, isa, is314;
    isU = false; isO = false; isA = false; isu = false; iso = false; isa = false; is314 = false;
    for (char & c : oldDrawerName) {
        string keyStr = ofToString(c);
        if (keyStr == "\210") {
            if      (is314 && isU) {newStr = newStr.substr(0, newStr.length()-2); keyStr = "Ü";}
            else if (is314 && isu) {newStr = newStr.substr(0, newStr.length()-2); keyStr = "ü";}
            if      (is314 && isA) {newStr = newStr.substr(0, newStr.length()-2); keyStr = "Ä";}
            else if (is314 && isa) {newStr = newStr.substr(0, newStr.length()-2); keyStr = "ä";}
            if      (is314 && isO) {newStr = newStr.substr(0, newStr.length()-2); keyStr = "Ö";}
            else if (is314 && iso) {newStr = newStr.substr(0, newStr.length()-2); keyStr = "ö";}
            isU = false; isO = false; isA = false; isu = false; iso = false; isa = false; is314 = false;
        }
        else if (keyStr == "u") {
            isU = false; isO = false; isA = false; isu = true; iso = false; isa = false; is314 = false;
        }
        else if (keyStr == "U") {
            isU = true; isO = false; isA = false; isu = false; iso = false; isa = false; is314 = false;
        }
        else if (keyStr == "o") {
            isU = false; isO = false; isA = false; isu = false; iso = true; isa = false; is314 = false;
        }
        else if (keyStr == "O") {
            isU = false; isO = true; isA = false; isu = false; iso = false; isa = false; is314 = false;
        }
        else if (keyStr == "a") {
            isU = false; isO = false; isA = false; isu = false; iso = false; isa = true; is314 = false;
        }
        else if (keyStr == "A") {
            isU = false; isO = false; isA = true; isu = false; iso = false; isa = false; is314 = false;
        }
        else if (keyStr == "\314") {
            is314 = true;
        }
        newStr+=keyStr;
    }
    return newStr;
}
