#include "ofApp.h"


void ofApp::setup() {

    autoMode = false;
    int camera_id = 0;
	int monitor_calibration_id = 2;
	string path = "master_settings.json";
	ofFile file(path);
	if(!file.exists()) {
        ofLog() << "ERROR: Can't find lookup file, don't know camera_id, using default 0";
		ofLog() << "ERROR: Can't find lookup file, don't know touchscreen monitor id, using default 2";
	} else {
		ofJson json = ofLoadJson(path);
		monitor_calibration_id = json["monitor_id"]["touchscreen"];
        camera_id = json["camera_id"]["city_transform"];
        autoMode = json["stress_test"] == 1;
	}

#ifdef CALIBRATION_MODE
    ofSetWindowPosition(3840 * monitor_calibration_id, 0);
#else
    ofSetWindowPosition(0, 0);
#endif
    ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);

    //ofSetFullscreen(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    ofBackground(0);

    width = 768;//1024;
    height = 384;//512;
    srcMode = 0;
#ifdef CALIBRATION_MODE
    debug = true;
#else
    debug = false;
#endif
    bFullscreen2 = false;

    // setup input
    if (srcMode==0) {
        cam.setDeviceID(camera_id);
        cam.setup(640, 480);
        srcWidth = 640;
        srcHeight = 480;
    }
    else if (srcMode==1) {
        src.load("test.png");
        src.resize(width, height);
        srcWidth = width;
        srcHeight = height;
    }
    else if (srcMode==2) {
        video.load("test.mp4");
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();
        srcWidth = video.getWidth();
        srcHeight = video.getHeight();
    }

    // setup cv
    sandbox.setup(srcWidth, srcHeight);
    sandbox.setEllipseSize(36);
    sandbox.setDebugPosition(5, 10);
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

    font.load("verdana.ttf", 18);
    exitButton.setup("Save & Exit", 15, 600, 200, 50);
    exitButton.setFont(&font);
    defaultButton.setup("Default settings", 15, 700, 200, 50);
    defaultButton.setFont(&font);
    ofAddListener(exitButton.clickEvent, this, &ofApp::exitButtonClicked);
    ofAddListener(defaultButton.clickEvent, this, &ofApp::defaultButtonClicked);

    // init images
    input.allocate(width, height, OF_IMAGE_COLOR);
    output.allocate(width, height, OF_IMAGE_COLOR);

    // setup Runway client
#ifndef CALIBRATION_MODE
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup("http://localhost:8759");
    runway.start();
#endif
}

//--------------------------------------------------------------
void ofApp::exitButtonClicked() {
    exit();
}

//--------------------------------------------------------------
void ofApp::defaultButtonClicked() {
    sandbox.loadSettings("sandboxSettings_default.xml", "settings_default.xml");
}

//--------------------------------------------------------------
void ofApp::checkFullscreen(){
    if(!bFullscreen2){
        float t = ofGetElapsedTimef();
        if (t > 3) {
            ofSetFullscreen(false);
            ofSetFullscreen(true);
            bFullscreen2 = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
#ifdef CALIBRATION_MODE
    sandbox.saveSettings();
#endif
    ofExit();
}

//--------------------------------------------------------------
void ofApp::sendToRunway() {
    if (!runway.isRunning()) {
        return;
    }
    ofxRunwayBundle bundle;
    bundle.address = "convert";
    bundle.images["image"] = input.getPixels();
    runway.send(bundle);
}

//--------------------------------------------------------------
void ofApp::update(){

    //sandbox.setThreshold(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255));

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

    if (sandbox.isMotionTripped() || (autoMode && !runway.getBusy())) {
        sendToRunway();
    }

    ofxRunwayBundle bundleToReceive;
    while (runway.tryReceive(bundleToReceive)) {
        ofPixels processedPixels = bundleToReceive.images["output"];
        outputTex.loadData(processedPixels);
    }

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
    checkFullscreen();

    if (debug) {
        drawDebug();
    } else {
        drawPresent();
    }
}

//--------------------------------------------------------------
void ofApp::drawDebug(){
    ofBackgroundGradient(ofColor(0), ofColor(100));

    sandbox.drawDebug();
    exitButton.draw();
    defaultButton.draw();
    if (outputTex.isAllocated()) {
        outputTex.draw(64 + 2*width, 50);
    }
}

//--------------------------------------------------------------
void ofApp::drawPresent(){
    ofBackgroundGradient(ofColor(0), ofColor(100));
    int margin = 30;
    int w = ofGetWidth() - 2 * margin;
    int h = int(float(w) / (outputTex.getWidth() / outputTex.getHeight()));
    int x = margin;
    int y = int(0.5 * (ofGetHeight() - h));

    if (outputTex.isAllocated()) {
        outputTex.draw(x, y, w, h);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    sandbox.keyEvent(key);
    if (key==' '){
        debug = !debug;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (!debug) return;
    sandbox.mouseMoved(x, y);
    exitButton.mouseMoved(x, y);
    defaultButton.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (!debug) return;
    sandbox.mouseDragged(x, y);
    exitButton.mouseDragged(x, y);
    defaultButton.mouseDragged(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (!debug) return;
    sandbox.mousePressed(x, y);
    exitButton.mousePressed(x, y);
    defaultButton.mousePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (!debug) return;
    sandbox.mouseReleased(x, y);
    exitButton.mouseReleased(x, y);
    defaultButton.mouseReleased(x, y);
}
