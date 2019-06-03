#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(100);

    cWidth = 900;//1800;
    cHeight = 900;
    width = 512; //1024
    height = 512;
    mode = 0;
    ready = true;
    toSend = false;
    bFullscreen1 = false;
    bFullscreen2 = false;

    // setup drawing canvas
    canvas.setup(10, 10, cWidth, cHeight, 75, false);
    canvas.setBackground(ofColor(255));
    canvas.addUndoOption("undo", "undo.png");

    canvas.addShapeOption("ground", ofColor(60, 120, 60), 1, 300, "ground.png");
    canvas.addShapeOption("bush", ofColor(30, 190, 180), 1, 300, "plant.png");
    canvas.addShapeOption("grass", ofColor(255, 255, 255), 1, 300, "grass.png");
    canvas.addShapeOption("tree", ofColor(0, 0, 255), 1, 300, "tree.png");
    canvas.addShapeOption("plant-other", ofColor(0, 255, 0), 1, 300, "plant.png");
    //canvas.addShapeOption("house", ofColor(120, 30, 30), 1, 300, "house.png");
    canvas.addShapeOption("sky-other", ofColor(40, 60, 255), 1, 300, "sky.png");
    canvas.addShapeOption("clouds", ofColor(40, 40, 40), 1, 300, "cloud.png");
    canvas.addShapeOption("mountain", ofColor(200, 200, 200), 1, 300, "mountain.png");
    canvas.addShapeOption("rock", ofColor(150, 150, 150), 1, 300, "rock.png");
    //canvas.addShapeOption("river", ofColor(0, 60, 180), 1, 300, "river.png");
    canvas.addShapeOption("sea", ofColor(0, 0, 100), 1, 300, "sea.png");
    canvas.addShapeOption("water-other", ofColor(0, 0, 185), 1, 300, "sea.png");

    canvas.addSlider("size", 0, 1);
    canvas.clear();

    // init images
    input.allocate(width, height, OF_IMAGE_COLOR);
    // output.allocate(width, height, OF_IMAGE_COLOR);

    // setup Runway client
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup("http://localhost:8651");
    runway.start();
    
    // setup gui
    // bCkpt_ngf1_30.addListener(this, &ofApp::selectCheckpoint_ngf1_30);
    // bCkpt_ngf1_60.addListener(this, &ofApp::selectCheckpoint_ngf1_60);
    // bCkpt_ngf9_60.addListener(this, &ofApp::selectCheckpoint_ngf9_60);

    bCkpt_landscape_sk_curated_50_10.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_50_10);
    bCkpt_landscape_sk_curated_50_40.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_50_40);
    bCkpt_landscape_sk_curated_50_80.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_50_80);
    bCkpt_landscape_sk_curated_segcolored_10.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_segcolored_10);
    bCkpt_landscape_sk_curated_segcolored_40.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_segcolored_40);
    bCkpt_landscape_sk_curated_segcolored_70.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_segcolored_70);
    bCkpt_landscape_sk_curated_small_250_50.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_small_250_50);
    bCkpt_landscape_sk_curated_small_250_150.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_small_250_150);
    bCkpt_landscape_sk_curated_small_250_300.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_small_250_300);
    bCkpt_landscape_sk_curated_small_250_400.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_small_250_400);
    bCkpt_landscape_sk_curated_small_250_500.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_small_250_500);
    bCkpt_landscape_sk_curated_transfer_50_10.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_transfer_50_10);
    bCkpt_landscape_sk_curated_transfer_50_40.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_transfer_50_40);
    bCkpt_landscape_sk_curated_transfer_50_100.addListener(this, &ofApp::selectCheckpoint_landscape_sk_curated_transfer_50_100);

    gui.setup();
    gui.setName("sketch-transform");
    gui.setPosition(10, 720);
    gui.add(bCkpt_landscape_sk_curated_50_10.setup("bCkpt_landscape_sk_curated_50_10"));
    gui.add(bCkpt_landscape_sk_curated_50_40.setup("bCkpt_landscape_sk_curated_50_40"));
    gui.add(bCkpt_landscape_sk_curated_50_80.setup("bCkpt_landscape_sk_curated_50_80"));
    gui.add(bCkpt_landscape_sk_curated_segcolored_10.setup("bCkpt_landscape_sk_curated_segcolored_10"));
    gui.add(bCkpt_landscape_sk_curated_segcolored_40.setup("bCkpt_landscape_sk_curated_segcolored_40"));
    gui.add(bCkpt_landscape_sk_curated_segcolored_70.setup("bCkpt_landscape_sk_curated_segcolored_70"));
    gui.add(bCkpt_landscape_sk_curated_small_250_50.setup("bCkpt_landscape_sk_curated_small_250_50"));
    gui.add(bCkpt_landscape_sk_curated_small_250_150.setup("bCkpt_landscape_sk_curated_small_250_150"));
    gui.add(bCkpt_landscape_sk_curated_small_250_300.setup("bCkpt_landscape_sk_curated_small_250_300"));
    gui.add(bCkpt_landscape_sk_curated_small_250_400.setup("bCkpt_landscape_sk_curated_small_250_400"));
    gui.add(bCkpt_landscape_sk_curated_small_250_500.setup("bCkpt_landscape_sk_curated_small_250_500"));
    gui.add(bCkpt_landscape_sk_curated_transfer_50_10.setup("bCkpt_landscape_sk_curated_transfer_50_10"));
    gui.add(bCkpt_landscape_sk_curated_transfer_50_40.setup("bCkpt_landscape_sk_curated_transfer_50_40"));
    gui.add(bCkpt_landscape_sk_curated_transfer_50_100.setup("bCkpt_landscape_sk_curated_transfer_50_100"));
    
    // nav buttons
    font.load("verdana.ttf", 36);
    goToFaves.setString("Faves!");
    goToFaves.setFont(&font);
    goToFaves.resize(180, 60);
    goToFaves.setPosition(ofGetWidth()-220, 5);
    goToDraw.setString("Draw!");
    goToDraw.setFont(&font);
    goToDraw.resize(180, 60);
    goToDraw.setPosition(ofGetWidth()-220, 5);
    saveFave.setString("Save!");
    saveFave.setFont(&font);
    saveFave.resize(180, 60);
    saveFave.setPosition(ofGetWidth() - 420, 5);
    ofAddListener(goToDraw.clickEvent, this, &ofApp::goToDrawScreen);
    ofAddListener(goToFaves.clickEvent, this, &ofApp::goToFavesScreen);
    ofAddListener(saveFave.clickEvent, this, &ofApp::saveFavorite);
    
    // favorites interface
    //faves.setup(320, 160, 24, 90);
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

    if (toSend && ready) {
        sendToRunway();
        toSend = false;
        toSendManual = false;
    }

    receiveFromRunway();
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
    ready = false;
}

//--------------------------------------------------------------
void ofApp::receiveFromRunway(){
    ofxRunwayBundle bundleToReceive;
    while (runway.tryReceive(bundleToReceive)) {
        ofPixels processedPixels = bundleToReceive.images["output"];
        output.loadData(processedPixels);
        ready = true;
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
    
    ofBackground(0);
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
    ofBackground(100);

    canvas.draw();
    canvas.drawGui();

    if (output.isAllocated()) {
        output.draw(ofGetWidth()-1024-5, ofGetHeight()-512-5);
    }

    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 12);
    gui.draw();
    
    goToFaves.draw();
    saveFave.draw();
}

//--------------------------------------------------------------
void ofApp::drawUserView(){
    ofBackground(100);
    
    canvas.draw();
    canvas.drawGui();
    
    goToFaves.draw();
    saveFave.draw();
}

//--------------------------------------------------------------
void ofApp::drawPresent(){
    ofBackground(0);
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
/*
    else if (key == 'f') {
        saveFavorite();
    }
    else if (key == OF_KEY_LEFT) {
        faves.prevPage();
    }
    else if (key == OF_KEY_RIGHT) {
        faves.nextPage();
    }

    if (key==' '){
        toSendManual = true;
    }
*/
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (mode == 0 || mode == 1) {
        canvas.mouseMoved(x, y);
        goToFaves.mouseMoved(x, y);
        saveFave.mouseMoved(x, y);
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
    } else if (mode == 2) {
        faves.mouseReleased(x, y);
        goToDraw.mouseReleased(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::fullscreenCheck1(){
    if(!bFullscreen1){
        float t = ofGetElapsedTimef();
        if (t > 1) {
            ofSetFullscreen(true);
            bFullscreen1 = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::fullscreenCheck2(){
    if(!bFullscreen2){
        float t = ofGetElapsedTimef();
        if (t > 1) {
            ofSetFullscreen(true);
            bFullscreen2 = true;
        }
    }
}
