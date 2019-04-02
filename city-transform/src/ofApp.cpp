#include "ofApp.h"


void ofApp::setup() {
    ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(0);

    //host = "http://d6e64b8d.ngrok.io/infer";
    //host = "http://ba92328c.ngrok.io/infer";

    width = 1024;
    height = 512;
    debug = true;
    srcMode = 0;
    numTrackingColors = 3;
    isReady = true;

    // setup input
    if (srcMode==0) {
        cam.setDeviceID(1);
        cam.setup(640, 480);
        srcWidth = 640;
        srcHeight = 480;
    }
    else if (srcMode==1) {
        src.load("test3.png");
        src.resize(width, height);
        srcWidth = width;
        srcHeight = height;
    }
    else if (srcMode==2) {
        video.load("/Users/gene/Documents/futurium_test2.mp4");
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();
        srcWidth = video.getWidth();
        srcHeight = video.getHeight();
    }

    // setup cv
    sandbox.setup(srcWidth, srcHeight);
    sandbox.setDebugPosition(0, 0);
    sandbox.setTrackColor(0, ofColor(120, 0, 0));
    sandbox.setTrackColor(1, ofColor(0, 150, 0));
    sandbox.setTrackColor(2, ofColor(0, 0, 100));
    sandbox.setTrackColor(3, ofColor(255, 255, 255));
    sandbox.setTrackColor(4, ofColor(0, 0, 0));
    sandbox.setOutColor(0, ofColor(255, 0, 0));
    sandbox.setOutColor(1, ofColor(10, 255, 10));
    sandbox.setOutColor(2, ofColor(0, 0, 255));
    sandbox.setOutColor(3, ofColor(255, 255, 255));
    sandbox.setOutColor(4, ofColor(0, 0, 0));
    sandbox.loadSettings();

    // init images
    input.allocate(width, height, OF_IMAGE_COLOR);
    output.allocate(width, height, OF_IMAGE_COLOR);

    // setup Runway client
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup("http://localhost:9000");
    runway.start();
}

//--------------------------------------------------------------
void ofApp::exit() {
    sandbox.saveSettings();
}

//--------------------------------------------------------------
void ofApp::sendToRunway() {
    ofxRunwayBundle bundle;
    bundle.address = "convert";
    bundle.images["image"] = input.getPixels();
    runway.send(bundle);
}

//--------------------------------------------------------------
void ofApp::update(){
    sandbox.setThreshold(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255));

    if (srcMode==0) {
        cam.update();
        if(cam.isFrameNew()) {
            src.setFromPixels(cam.getPixels());
            updateSandbox();
        }
    }
    else if (srcMode==2) {
        video.update();
        if(video.isFrameNew()) {
            src.setFromPixels(video.getPixels());
            updateSandbox();
        }
    }
    else if (srcMode == 0) {
        updateSandbox();
    }

    if (sandbox.isMotionTripped()) {
        sendToRunway();
    }

    ofxRunwayBundle bundleToReceive;
    while (runway.tryReceive(bundleToReceive)) {
        ofPixels processedPixels = bundleToReceive.images["output"];
        outputTex.loadData(processedPixels);
    }

    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::updateSandbox(){
    ofPixels inputPixels;
    sandbox.update(src);
    sandbox.getFbo().readToPixels(inputPixels);
    input.setFromPixels(inputPixels);
    input.resize(width, height);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(ofColor::white, ofColor::black);

    /*
    if (outputTex.isAllocated()) {
        outputTex.draw(0, 0+height);
    }
    if (input.isAllocated()){
        input.draw(width, 0);
        src.draw(0, 0);
    }*/

    //src.draw(0, 0);
    sandbox.drawDebug();

    if (outputTex.isAllocated()) {
      outputTex.draw(50+width, 50+height);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    sandbox.keyEvent(key);

    if (key==' '){
        debug = !debug;
    }
}
