#include "ofApp.h"


using namespace cv;
using namespace ofxCv;



//--------------------------------------------------------------
void ofApp::setupMain(){
	ofSetBackgroundColor(0);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::drawMain(ofEventArgs & args){
    //drawPresent();
    ofBackground(0);
    faves.drawPresent();
}

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetWindowShape(1920, 1080);
    
    //ofSetFullscreen(true);
    ofSetFrameRate(60);
    
    //ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(100);

    width = 1024;
    height = 512;
    mode = 0;
    ready = true;
    toSend = false;

    // setup drawing canvas
    canvas.setup(10, 10, width, height, 100, false);
    canvas.setBackground(ofColor(255));
    canvas.addUndoOption("undo");
//    canvas.addLineOption("roads", ofColor::black, 1, 10);

    canvas.addShapeOption("white", ofColor(255, 255, 255), 1, 300);
    canvas.addShapeOption("red", ofColor(200, 0, 0), 1, 300);
    canvas.addShapeOption("organge", ofColor(200, 200, 0), 1, 300);
    canvas.addShapeOption("teal", ofColor(0, 200, 200), 1, 300);
    canvas.addShapeOption("purple", ofColor(200, 0, 200), 1, 300);
    canvas.addShapeOption("green", ofColor(0, 200, 0), 1, 300);
    canvas.addShapeOption("blue", ofColor(0, 0, 200), 1, 300);
    canvas.addSlider("size", 0, 1);
    canvas.clear();
    //img.load("/home/bzion/projects/pix2pixHD/checkpoints/renais_025_025_ngfndf16/web/images/epoch006_input_label.jpg");
    //canvas.setFromPixels(img.getPixels());

    // init images
    input.allocate(width, height, OF_IMAGE_COLOR);
    // output.allocate(width, height, OF_IMAGE_COLOR);

    // setup Runway client
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup("http://localhost:8001");
    runway.start();

    // setup gui
    epoch.addListener(this, &ofApp::epochChanged);
    gui.setup();
    gui.setName("sketch-transform");
    gui.add(epoch.set("epoch", 1, 1, 12));
    gui.setPosition(10, 720);
    
    
    
    
    ofDirectory dir("/Users/gene/Learn/MLJS/ml5-examples/p5js/");
    int n = dir.listDir();
    
    for (int i=0; i<n; i++) {
         if (dir.getFile(i).isDirectory()){
             string checkpoint = dir.getFile(i).getFileName();
             ofParameter<bool> cp;
             cp.addListener(this, &ofApp::checkpointSelected);
             cp.set(checkpoint, false);
             checkpoints.push_back(cp);
             gui.add(cp);
         }
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    canvas.getCanvas().readToPixels(input);
    input.update();

    if (canvas.isFrameNew()) {
        //toSend = true;
    }

    if (toSend && ready) {
        sendToRunway();
        toSend = false;
    }

    receiveFromRunway();
}

//--------------------------------------------------------------
void ofApp::sendToRunway(){
    ofxRunwayBundle bundle;
    bundle.address = "convert";
    bundle.images["image"] = input.getPixels();
    runway.send(bundle);
    ready = false;
}

//--------------------------------------------------------------
void ofApp::receiveFromRunway(){
    ofxRunwayBundle bundleToReceive;
    while (runway.tryReceive(bundleToReceive)) {
        ofPixels processedPixels = bundleToReceive.images["output"];
        //output.setFromPixels(processedPixels);
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
void ofApp::epochChanged(int & e) {
    setModel("sk_landscape_0060_64", int(10 * epoch));
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
        drawPresent();
    }
    else if (mode == 2) {
        drawFavorites();
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
}

//--------------------------------------------------------------
void ofApp::drawPresent(){
    ofBackground(0);

    // int w = output.getWidth();
    // int h = output.getHeight();
    // if (float(w)/h > float(ofGetWidth()/ofGetHeight())) {

    // }

    int w = ofGetWidth() - 20;
    int h = int(float(w) / (output.getWidth() / output.getHeight()));

    if (output.isAllocated()) {
        output.draw(10, 10, w, h);
    }
}

//--------------------------------------------------------------
void ofApp::drawFavorites(){
    ofBackground(0);
    
    //faves.drawPresent();
    faves.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ') {
        //mode = (mode + 1) % 3;
        mode = 2-mode;
    }    
    else if (key == 'f') {
        saveFavorite();
    }
    
    else if (key == OF_KEY_LEFT) {
        faves.prevPage();
    }
    else if (key == OF_KEY_RIGHT) {
        faves.nextPage();
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (mode == 0) {
        canvas.mouseMoved(x, y);
    } else if (mode == 2) {
        faves.mouseMoved(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (mode == 0) {
        canvas.mouseDragged(x, y);
    } else if (mode == 2) {
        faves.mouseDragged(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (mode == 0) {
        canvas.mousePressed(x, y);
    } else if (mode == 2) {
        faves.mousePressed(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (mode == 0) {
        canvas.mouseReleased(x, y);
    } else if (mode == 2) {
        faves.mouseReleased(x, y);
    }
}
