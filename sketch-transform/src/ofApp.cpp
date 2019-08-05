#include "ofApp.h"


//  sea -> water
//  bush -> leaves, plant
//  dirt -> sand
// flower 118 straw 162 bridge 94 fence 112, 39 pavement, 135 mud, 133 moss 


// can only save after canvas has changed a bit



//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(100);

    cX = 130;
    cY = 135;
    cWidth = 900;//1800;
    cHeight = 900;
    width = 512; //1024
    height = 512;
    mode = PRESENTATION;
    toSend = false;
    bFullscreen1 = false;
    bFullscreen2 = false;

    // setup drawing canvas
    canvas.setup(cX, cY, cWidth, cHeight);
    canvas.setBackground(ofColor(255));
    canvas.setMaxHistory(64);
    canvas.clear();
    input.allocate(width, height, OF_IMAGE_COLOR);

    // create gui
    panelLeft.setup("categories", 20, 130, 90, 5, true, &canvas);
    panelLeft.setGuiBgDraw(false);
    panelTop.setup("drawOptions", 120, 20, 95, 10, false, &canvas);
    panelTop.setGuiBgDraw(false);
    panelTop.addBrushSizeOption("small", 20, "paintbrush-thin.png");
    panelTop.addBrushSizeOption("medium", 60, "paintbrush-medium.png");
    panelTop.addBrushSizeOption("large", 120, "paintbrush-thick.png");
    panelTop.addUndoOption("undo", "undo.png");
    panelTop.addRedoOption("redo", "redo.png");
    panelTop.addClearOption("clear", "clear.png");
    
    // get lookup table of buttons     
    string path = "master_settings.json";
    ofFile file(path);
    if (file.exists()) {
        ofJson json = ofLoadJson(path);
        for (auto & label : json["sketch_transform_labels"]) {
            string name = label["name"];
            string iconPath = label["icon"];
            int index = label["index"];
            int r = label["color"][0];
            int g = label["color"][1];
            int b = label["color"][2];
            ofColor clr(r, g, b);
            ofxCanvasGuiElement * button = panelLeft.addShapeOption(name, clr, NULL, NULL, iconPath);
            //button->setActiveBackgroundColor(clr);
        }
        
        autoMeander = json["stress_test"] == 1;
    }
    else {
        ofLog() << "Can't find lookup file ";
    }
    
    // styling for buttons
    for (auto b : panelLeft.getElements()) {
        b->setBackgroundGradient(55);
        b->setCornerRounded(6);
    }
    for (auto b : panelTop.getElements()) {
        b->setBackgroundGradient(85);
        b->setCornerRounded(6);
    }

    // click first button & divert clearing
    panelLeft.getElement(0)->buttonClicked();
    panelTop.getElement(autoMeander ? 2 : 0)->buttonClicked();
    ofAddListener(ofxCanvasButtonEvent::events, this, &ofApp::canvasPanelEvent);
    
    // setup Runway client
#ifndef TEST_MODE
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup("http://localhost:8651");
    runway.start();
#endif
    
    // nav buttons
    font.load("verdana.ttf", 36);
    
    goToFaves.setName("Faves");
    goToFaves.setFont(&font);
    goToFaves.setBackgroundColor(ofColor::lightGray);
    goToFaves.setBackgroundGradient(50);
    goToFaves.setCornerRounded(5);
    goToFaves.loadIcon("favorites.png");
    goToFaves.resize(105, 95);
    goToFaves.setPosition(ofGetWidth()-125, 20);
    goToFaves.disable();
    
    goToDraw.setName("Draw");
    goToDraw.setFont(&font);
    goToDraw.setBackgroundColor(ofColor::lightGray);
    goToDraw.setBackgroundGradient(50);
    goToDraw.setCornerRounded(5);
    goToDraw.loadIcon("paintbrush.png");
    goToDraw.resize(105, 95);
    goToDraw.setPosition(ofGetWidth()-125, 20);
    goToDraw.disable();
    
    saveFave.setName("Save");
    saveFave.setFont(&font);
    saveFave.setBackgroundColor(ofColor::lightGray);
    saveFave.setBackgroundGradient(50);
    saveFave.setCornerRounded(5);
    saveFave.loadIcon("favorites_add.png");
    saveFave.resize(105, 95);
    saveFave.setPosition(900, 20);

    randomFave.setName("Rand");
    randomFave.setFont(&font);
    randomFave.setBackgroundColor(ofColor::lightGray);
    randomFave.setBackgroundGradient(50);
    randomFave.setCornerRounded(5);
    randomFave.loadIcon("random.png");
    randomFave.resize(105, 95);
    randomFave.setPosition(740, 20);
    
    ofAddListener(goToDraw.clickEvent, this, &ofApp::goToDrawScreen);
    ofAddListener(goToFaves.clickEvent, this, &ofApp::goToFavesScreen);
    ofAddListener(saveFave.clickEvent, this, &ofApp::saveFavoritePrompt);
    ofAddListener(randomFave.clickEvent, this, &ofApp::selectRandomFavorite);
    ofAddListener(saveFaveTimer.event, this, &ofApp::timerUpEvent);
    
    
    // favorites interface
    //faves.setup(320, 160, 24, 90);
    faves.setCanvasReference(&canvas);
    
    // setup keyboard
    keyboard.setup(LAYOUT_GERMAN);
    keyboard.setCharacterLimit(36);
    keyboard.setInputPosition(360, 200, 1100, 86);
    keyboard.setPosition(380, 330, 84, 84, 30, 10);
    keyboard.loadInputFont("verdana.ttf", 46);
    keyboard.clearInput();
    keyboard.setVisible(false);
    ofAddListener(keyboard.bSave.clickEvent, this, &ofApp::keyboardSaveEvent);
    //ofAddListener(keyboard.bCancel.clickEvent, this, &ofApp::keyboardCancelEvent);

    // templates interface
    loadTemplates();
    if (templates.size() > 0) {
        canvas.setFromImage(templates[0]->getName());
    }
}

//--------------------------------------------------------------
void ofApp::timerUpEvent() {
    saveFave.enable();
}

//--------------------------------------------------------------
void ofApp::setupMain(){
	ofSetBackgroundColor(0);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
    fullscreenCheck1();

    if (autoMeander) {
        meander();
    }

    canvas.getCanvas().readToPixels(input);
    input.update();

    if (canvas.isFrameNew()) {
        toSend = true;
    }

    if (toSend && !runway.getBusy()) {
        sendToRunway();
        toSend = false;
    }

    receiveFromRunway();

    if (toClearCanvas && templates.size() > 0) {
        canvas.setFromImage(templates[0]->getName());
        toClearCanvas = false;
    }

}

//--------------------------------------------------------------
void ofApp::sendToRunway(){
    ofxRunwayBundle bundle;
    ofImage outputImage;
    outputImage.setFromPixels(input.getPixels());
    outputImage.resize(width, height);
    bundle.address = "convert";
    bundle.images["image"] = outputImage.getPixels();
    runway.send(bundle);
}

//--------------------------------------------------------------
void ofApp::receiveFromRunway(){
    ofxRunwayBundle bundleToReceive;
    while (runway.tryReceive(bundleToReceive)) {
        ofPixels processedPixels = bundleToReceive.images["output"];
        output.loadData(processedPixels);
    }
}

//--------------------------------------------------------------
void ofApp::setModel(string model_name, int which_epoch) {
    ofxRunwayBundle bundle;
    bundle.address = "setup";
    bundle.strings["model_name"] = model_name;
    bundle.ints["which_epoch"] = which_epoch;
    runway.send(bundle);
    toSend = true;
}


//--------------------------------------------------------------
void ofApp::selectRandomFavorite() {
    faves.selectRandom();
}

//--------------------------------------------------------------
void ofApp::saveFavoritePrompt() {
    keyboard.setVisible(true);
}

//--------------------------------------------------------------
void ofApp::saveFavorite(string name) {
    //saveTemplate();
    ofFbo fbo;
    fbo.allocate(2 * width, height);
    fbo.begin();
    ofClear(0, 0);
    ofSetColor(ofColor::white);
    canvas.getCanvas().draw(0, 0, width, height);
#ifdef TEST_MODE
    ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
    ofDrawRectangle(width, 0, width, height);
    ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
    ofDrawRectangle(width+75, 75, width-150, height-150);
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ofGetTimestampString(), width + width/2, height/2);
#else
    output.draw(width, 0);
#endif
    fbo.end();
    faves.add(&fbo.getTexture(), name);
}

//--------------------------------------------------------------
void ofApp::saveTemplate() {
    string newPath = "templates/template"+ofGetTimestampString()+".png";
    input.save(newPath);
}

//--------------------------------------------------------------
void ofApp::loadTemplates() {
    int ctw = 365;
    int cth = 365;
    int ctmargin = 50;
    int ctx0 = canvas.getRectangle().getX() + canvas.getRectangle().getWidth() + ctmargin;
    int cty0 = canvas.getRectangle().getY() + ctmargin;
    templates.clear();
    ofDirectory dir("templates");
    int n = min(4, (int) dir.listDir());
    for (int i=0; i<n; i++) {
        if (!dir.getFile(i).isDirectory()){
            int ctx, cty;
            if      (i==0) {ctx = ctx0; cty = cty0;}
            else if (i==1) {ctx = ctx0 + ctw + ctmargin; cty = cty0;}
            else if (i==2) {ctx = ctx0 + ctw + ctmargin; cty = cty0 + cth + ctmargin;}
            else if (i==3) {ctx = ctx0; cty = cty0 + cth + ctmargin;}
            TemplateThumbnail *ctemplate = new TemplateThumbnail();
            ctemplate->setup(dir.getFile(i).getAbsolutePath(), ctx, cty, ctw, cth);
            ctemplate->loadIcon(dir.getFile(i).getAbsolutePath());
            ofAddListener(ctemplate->event, this, &ofApp::templateClicked);
            templates.push_back(ctemplate);
        }
    }
}

//--------------------------------------------------------------
void ofApp::templateClicked(string & templatePath) {
    canvas.setFromImage(templatePath);
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (mode == PRESENTATION) {
#ifdef DEBUG_VIEW
        drawDebug();
#else
        drawUserView();
#endif
    }
    else if (mode == FAVORITES) {
        drawFavorites();
    }
}

//--------------------------------------------------------------
void ofApp::drawMain(ofEventArgs & args){
    fullscreenCheck2();
    ofBackgroundGradient(ofColor(100), ofColor(0));
    if (mode == PRESENTATION) {
        drawPresent();
    }
    else if (mode == FAVORITES) {
        faves.drawPresent();
    }
}

//--------------------------------------------------------------
void ofApp::drawDebug(){
    ofBackgroundGradient(ofColor(100), ofColor(0));

    canvas.draw();
    panelTop.draw();
    panelLeft.draw();

    if (output.isAllocated()) {
        int x = canvas.getRectangle().getX() + canvas.getRectangle().getWidth() + 32;
        int y = canvas.getRectangle().getY();
        output.draw(x, y, canvas.getRectangle().getWidth(), canvas.getRectangle().getHeight());    
    }

    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 12);
    
    keyboard.drawAll();
    goToFaves.draw();
    saveFave.draw();
    randomFave.draw();
}

//--------------------------------------------------------------
void ofApp::drawUserView(){
    ofBackgroundGradient(ofColor(100), ofColor(0));
    canvas.draw();
    panelTop.draw();
    panelLeft.draw();
    drawTemplates();
    keyboard.drawAll();
    goToFaves.draw();
    saveFave.draw();
    randomFave.draw();
}

//--------------------------------------------------------------
void ofApp::drawPresent(){
    ofBackgroundGradient(ofColor(100), ofColor(0));
    
    if (output.isAllocated()) {
        int w = int(0.5 * ofGetWidth() - 20);
        int h = int(float(w) / (output.getWidth() / output.getHeight()));
        int y = int(0.5 * (ofGetHeight() - h));
        output.draw(10, y, w, h);
    }
}

//--------------------------------------------------------------
void ofApp::drawFavorites(){
    ofBackgroundGradient(ofColor(100), ofColor(0));
    faves.draw();
    goToDraw.draw();
}

//--------------------------------------------------------------
void ofApp::drawTemplates() {
    for (auto t : templates) {
        t->draw();
    }
}

//--------------------------------------------------------------
void ofApp::meander() {
    int mx = cX + int(ofNoise(20, 0.01*ofGetFrameNum()) * cWidth);
    int my = cY + int(ofNoise(10, 0.01*ofGetFrameNum()) * cHeight);
    canvas.mouseDragged(mx, my);
    if (ofRandom(1) < 0.005) {
        panelLeft.selectRandomOption();
    }
}

//--------------------------------------------------------------
void ofApp::canvasPanelEvent(ofxCanvasButtonEvent &e) {
    if (e.settings.isLine == NULL && (e.settings.color == ofColor::black)) {
        canvas.toClear = false;
        toClearCanvas = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyboardCancelEvent() {
    keyboard.clearInput();
    keyboard.setVisible(false);
}

//--------------------------------------------------------------
void ofApp::keyboardSaveEvent() {
    string name = keyboard.getInput();
    saveFavorite(name);
    keyboard.clearInput();
    keyboard.setVisible(false);
    keyboard.setMessageString("Saved favorite!", 2);
    saveFave.disable();
    saveFaveTimer.start(10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    if (key=='t') {
//        saveTemplate();
//    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (mode == PRESENTATION) {
        if (keyboard.getVisible()) {
            keyboard.mouseMoved(x, y);
            return;
        }
        canvas.mouseMoved(x, y);
        goToFaves.mouseMoved(x, y);
        saveFave.mouseMoved(x, y);
        randomFave.mouseMoved(x, y);
        mouseMovedTemplates(x, y);
    } else if (mode == FAVORITES) {
        faves.mouseMoved(x, y);
        goToDraw.mouseMoved(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (mode == PRESENTATION) {
        if (keyboard.getVisible()) {
            keyboard.mouseDragged(x, y);
            return;
        }
        canvas.mouseDragged(x, y);
        goToFaves.mouseDragged(x, y);
        saveFave.mouseDragged(x, y);
        randomFave.mouseDragged(x, y);
        mouseDraggedTemplates(x, y);
    } else if (mode == FAVORITES) {
        faves.mouseDragged(x, y);
        goToDraw.mouseDragged(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (mode == PRESENTATION) {
        if (keyboard.getVisible()) {
            keyboard.mousePressed(x, y);
            return;
        }
        canvas.mousePressed(x, y);
        goToFaves.mousePressed(x, y);
        saveFave.mousePressed(x, y);
        randomFave.mousePressed(x, y);
        mousePressedTemplates(x, y);
    }
    else if (mode == FAVORITES) {
        faves.mousePressed(x, y);
        goToDraw.mousePressed(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (mode == PRESENTATION) {
        if (keyboard.getVisible()) {
            bool inside = keyboard.mouseReleased(x, y);
            if (!inside) {
                keyboardCancelEvent();
            }
            return;
        }
        canvas.mouseReleased(x, y);
        goToFaves.mouseReleased(x, y);
        saveFave.mouseReleased(x, y);
        randomFave.mouseReleased(x, y);
        mouseReleasedTemplates(x, y);
    }
    else if (mode == FAVORITES) {
        faves.mouseReleased(x, y);
        goToDraw.mouseReleased(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMovedTemplates(int mx, int my) {
    if (mode == PRESENTATION) {
        for (auto t : templates) {
            t->mouseMoved(mx, my);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressedTemplates(int mx, int my) {
    if (mode == PRESENTATION) {
        for (auto t : templates) {
            t->mousePressed(mx, my);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseDraggedTemplates(int mx, int my) {
    if (mode == PRESENTATION) {
        for (auto t : templates) {
            t->mouseDragged(mx, my);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleasedTemplates(int mx, int my) {
    if (mode == PRESENTATION) {
        for (auto t : templates) {
            t->mouseReleased(mx, my);
        }
    }
}

//--------------------------------------------------------------
void ofApp::fullscreenCheck1(){
    if(!bFullscreen1){
        float t = ofGetElapsedTimef();
        if (t > 4) {
            ofSetFullscreen(true);
            bFullscreen1 = true;
        }
    }
    if (bFullscreen1 && bFullscreen2 && !faves.getIsSetup()) {
        faves.setup(384, 384, 50, 90);
        goToFaves.enable();
        goToDraw.enable();
    }
}

//--------------------------------------------------------------
void ofApp::fullscreenCheck2(){
    if(!bFullscreen2){
        float t = ofGetElapsedTimef();
        if (t > 4) {
            ofSetFullscreen(true);
            bFullscreen2 = true;
        }
    }
}
