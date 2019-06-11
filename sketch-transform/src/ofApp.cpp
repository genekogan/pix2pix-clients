#include "ofApp.h"


//    sea -> water
//    bush -> leaves, plant
//    dirt -> sand
// flower 118 straw 162 bridge 94 fence 112, 39 pavement, 135 mud, 133 moss 


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1920, 1080);
    
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(100);

    cWidth = 820;//1800;
    cHeight = 820;
    width = 512; //1024
    height = 512;
    mode = 1;
    toSend = false;
    bFullscreen1 = false;
    bFullscreen2 = false;

    // setup drawing canvas
    canvas.setup(120, 130, cWidth, cHeight);
    canvas.setBackground(ofColor(255));
    canvas.clear();
    input.allocate(width, height, OF_IMAGE_COLOR);
    
    // create gui
    panelLeft.setup(20, 130, 80, true, &canvas);
    panelTop.setup(120, 30, 85, false, &canvas);
    panelTop.addSlider("size", 0, 1);
    panelTop.addUndoOption("undo", "undo.png");
    panelTop.addUndoOption("redo", "redo.png");
    panelTop.addClearOption("clear", "clear.png");

    // get lookup table of buttons     
    string path = "master_settings.json";
    ofFile file(path);
    if(file.exists()) {
        ofJson json = ofLoadJson(path);
        for (auto & label : json["sketch_transform_labels"]) {
            string name = label["name"];
            string iconPath = label["icon"];
            int index = label["index"];
            int r = label["color"][0];
            int g = label["color"][1];
            int b = label["color"][2];
            panelLeft.addShapeOption(name, ofColor(r, g, b), 1, 300, iconPath);
        }
    } else {
        ofLog() << "Can't find lookup file ";
    }

    // click first button & divert clearing
    panelLeft.getElement(0)->buttonClicked();
    ofAddListener(ofxCanvasButtonEvent::events, this, &ofApp::canvasPanelEvent);

    // setup Runway client
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup("http://localhost:8651");
    runway.start();
    
    // nav buttons
    font.load("verdana.ttf", 36);
    goToFaves.setName("Faves!");
    goToFaves.setFont(&font);
    goToFaves.resize(180, 60);
    goToFaves.setPosition(ofGetWidth()-220, 5);
    goToDraw.setName("Draw!");
    goToDraw.setFont(&font);
    goToDraw.resize(180, 60);
    goToDraw.setPosition(ofGetWidth()-220, 5);
    saveFave.setName("Save!");
    saveFave.setFont(&font);
    saveFave.resize(180, 60);
    saveFave.setPosition(ofGetWidth() - 420, 5);
    ofAddListener(goToDraw.clickEvent, this, &ofApp::goToDrawScreen);
    ofAddListener(goToFaves.clickEvent, this, &ofApp::goToFavesScreen);
    ofAddListener(saveFave.clickEvent, this, &ofApp::saveFavorite);
    
    // favorites interface
    //faves.setup(320, 160, 24, 90);
    
    // templates interface
    loadTemplates();
    if (templates.size() > 0) {
        canvas.setFromImage(templates[0]->getName());
    }
}

void ofApp::canvasPanelEvent(ofxCanvasButtonEvent &e) {
    if (e.settings.isLine == NULL && (e.settings.color == ofColor::black)) {
        canvas.toClear = false;
        toClearCanvas = true;
    }
}



//--------------------------------------------------------------
void ofApp::setupMain(){
	ofSetBackgroundColor(0);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
    fullscreenCheck1();

    canvas.getCanvas().readToPixels(input);
    input.update();

    if (canvas.isFrameNew()) {
        toSend = true;
    }

    if (toSend && !runway.getBusy()) {
        sendToRunway();
        toSend = false;
        toSendManual = false;
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
void ofApp::saveFavorite() {
    //saveTemplate();
#ifdef TEST_MODE
    ofFbo fbo;
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0, 0);
    ofBackground(ofRandom(255), ofRandom(255), ofRandom(255));
    ofDrawBitmapString(ofGetTimestampString(), width/2, height/2);
    fbo.end();
    faves.add(&fbo.getTexture());
#endif
#ifndef TEST_MODE
    faves.add(&output);
#endif
}

//--------------------------------------------------------------
void ofApp::saveTemplate() {
    string newPath = "templates/template"+ofGetTimestampString()+".png";
    input.save(newPath);
}

//--------------------------------------------------------------
void ofApp::loadTemplates() {
    int ctw = 375;
    int cth = 375;
    int ctmargin = 64;
    int ctx0 = canvas.getRectangle().getX() + canvas.getRectangle().getWidth() + 80;
    int cty0 = canvas.getRectangle().getY() + 10;
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
    if (mode == 0) {
        drawDebug();
    }
    else if (mode == 1) {
        drawUserView();
    }
    else if (mode == 2) {
        drawFavorites();
    }
}

//--------------------------------------------------------------
void ofApp::drawMain(ofEventArgs & args){
    fullscreenCheck2();
    
    ofBackgroundGradient(ofColor(100), ofColor(0));
    if (mode == 0) {
        drawPresent();
    }
    else if (mode == 1) {
        drawPresent();
    }
    else if (mode == 2) {
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
    
    goToFaves.draw();
    saveFave.draw();
}

//--------------------------------------------------------------
void ofApp::drawUserView(){
    ofBackgroundGradient(ofColor(100), ofColor(0));
    
    canvas.draw();
    panelTop.draw();
    panelLeft.draw();
    drawTemplates();
    
    goToFaves.draw();
    saveFave.draw();
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
    ofBackground(0);
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
void ofApp::keyPressed(int key){
    if (key=='1') {
        mode = 0;
    }
    if (key=='2') {
        mode = 1;
    }
    if (key=='3') {
        mode = 2;
    }
    
    if (key=='t') {
        saveTemplate();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (mode == 0 || mode == 1) {
        canvas.mouseMoved(x, y);
        goToFaves.mouseMoved(x, y);
        saveFave.mouseMoved(x, y);
        mouseMovedTemplates(x, y);
    } else if (mode == 2) {
        faves.mouseMoved(x, y);
        goToDraw.mouseMoved(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (mode == 0 || mode == 1) {
        canvas.mouseDragged(x, y);
        goToFaves.mouseDragged(x, y);
        saveFave.mouseDragged(x, y);
        mouseDraggedTemplates(x, y);
    } else if (mode == 2) {
        faves.mouseDragged(x, y);
        goToDraw.mouseDragged(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (mode == 0 || mode == 1) {
        canvas.mousePressed(x, y);
        goToFaves.mousePressed(x, y);
        saveFave.mousePressed(x, y);
        mousePressedTemplates(x, y);
    } else if (mode == 2) {
        faves.mousePressed(x, y);
        goToDraw.mousePressed(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (mode == 0 || mode == 1) {
        canvas.mouseReleased(x, y);
        goToFaves.mouseReleased(x, y);
        saveFave.mouseReleased(x, y);
        mouseReleasedTemplates(x, y);
    } else if (mode == 2) {
        faves.mouseReleased(x, y);
        goToDraw.mouseReleased(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMovedTemplates(int mx, int my) {
    for (auto t : templates) {
        t->mouseMoved(mx, my);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressedTemplates(int mx, int my) {
    for (auto t : templates) {
        t->mousePressed(mx, my);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDraggedTemplates(int mx, int my) {
    for (auto t : templates) {
        t->mouseDragged(mx, my);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleasedTemplates(int mx, int my) {
    for (auto t : templates) {
        t->mouseReleased(mx, my);
    }
}

//--------------------------------------------------------------
void ofApp::fullscreenCheck1(){
    if(!bFullscreen1){
        float t = ofGetElapsedTimef();
        if (t > 3) {
            ofSetFullscreen(true);
            bFullscreen1 = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::fullscreenCheck2(){
    if(!bFullscreen2){
        float t = ofGetElapsedTimef();
        if (t > 3) {
            ofSetFullscreen(true);
            bFullscreen2 = true;
        }
    }
}
