#include "ofApp.h"


void ofApp::setup() {
    autoMode = false;

	int monitor_presentation_id = 1;
	int monitor_touchscreen_id = 2;
    int monitor_presentation_order = 1;
	int monitor_touchscreen_order = 2;
	int resolution_px = 1920;
	int resolution_py = 1080;
	int resolution_tx = 1920;
	int resolution_ty = 1080;
	int position_px = 0;
	int position_tx = 0;
    int camera_id = -1;
	int monitor_calibration_id = 2;
    cameraHardwareName = "";
	string path = "master_settings.json";
	ofFile file(path);
	if(!file.exists()) {
        ofLog() << "ERROR: Can't find lookup file, don't know camera_id, using default 0";
		ofLog() << "ERROR: Can't find lookup file, don't know touchscreen monitor id, using default 2";
	} 
    else {
		ofJson json = ofLoadJson(path);
        monitor_presentation_id = json["monitor_id"]["city_transform"];
		monitor_touchscreen_id = json["monitor_id"]["touchscreen"];
		for (int i=0; i<4; i++) {
			if (json["monitor_order"][i] == monitor_presentation_id) {
				monitor_presentation_order = i;
			}
			if (json["monitor_order"][i] == monitor_touchscreen_id) {
				monitor_touchscreen_order = i;
			}
		}
		resolution_px = json["monitor_resolution"][ofToString(monitor_presentation_id)][0];
		resolution_py = json["monitor_resolution"][ofToString(monitor_presentation_id)][1];
		resolution_tx = json["monitor_resolution"][ofToString(monitor_touchscreen_id)][0];
		resolution_ty = json["monitor_resolution"][ofToString(monitor_touchscreen_id)][1];
		for (int i=0; i<monitor_presentation_order; i++) {
			int px = json["monitor_resolution"][ofToString(json["monitor_order"][i])][0];
			position_px += px;
		}
		for (int i=0; i<monitor_touchscreen_order; i++) {
			int tx = json["monitor_resolution"][ofToString(json["monitor_order"][i])][0];
			position_tx += tx;
		}
		ofLog() << "Touchscreen monitor: id "<<monitor_touchscreen_id<<", resolution [" << resolution_tx << ", " <<resolution_ty << "], x-position " << position_tx; 
		ofLog() << "Presentation monitor: id "<<monitor_presentation_id<<", resolution [" << resolution_px << ", " <<resolution_py << "], x-position " << position_px; 
        cameraHardwareName = "/dev/video"+ofToString(json["camera_id"]["city_transform"]);
        ofLog() << "camera requested: " << cameraHardwareName << endl;
        autoMode = json["stress_test"] == 1;
	}
    
#ifdef CALIBRATION_MODE
    ofSetWindowPosition(position_tx, 0);
    ofSetWindowShape(resolution_tx, resolution_ty);
#else
    ofSetWindowPosition(position_px, 0);
    ofSetWindowShape(resolution_px, resolution_py);
#endif
    ofSetVerticalSync(true);

    //ofSetFullscreen(true);
    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    ofBackground(0);

    width = 1024;//1024;
    height = 512;//512;
    
    srcMode = 0;
#ifdef CALIBRATION_MODE
    debug = true;
#else
    //debug = false;
    debug = true;
    ofAddListener(initialDebug.event, this, &ofApp::timerUpEvent);
    initialDebug.start(20);
#endif
    bFullscreen2 = false;

    // setup input
    if (srcMode==0) {
        auto devices = cam.listDevices();
        for(int d=0; d<devices.size(); d++) {
            if (devices[d].hardwareName == cameraHardwareName) {
                camera_id = d;
            }
        }
        cam.setDeviceID(camera_id);
        cam.setup(1280, 720);
        srcWidth = 1280;
        srcHeight = 720;

        // disable autofocus
        string cmd = "v4l2-ctl -d "+cameraHardwareName+" --set-ctrl=focus_auto=0";
        ofSystem(cmd);
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

    // initlaize camera focus
    focusAbsolute = sandbox.getCameraFocus();
    focusAbsolute -= (focusAbsolute % 5);
    string cmd = "v4l2-ctl -d "+cameraHardwareName+" --set-ctrl=focus_absolute="+ofToString(focusAbsolute);
    ofLog() << "Initial focus: " << cmd;
    ofSystem(cmd);

    // buttons
    font.load("verdana.ttf", 18);
    mappingButton.setup("Mapping", 15, 430, 185, 50);
    mappingButton.setFont(&font);
    saveButton.setup("Save", 15, 620, 185, 50);
    saveButton.setFont(&font);
    exitButton.setup("Exit", 15, 800, 185, 50);
    exitButton.setFont(&font);
    //defaultButton.setup("Default settings", 15, 730, 185, 50);
    //defaultButton.setFont(&font);
    ofAddListener(saveButton.clickEvent, this, &ofApp::saveButtonClicked);
    ofAddListener(exitButton.clickEvent, this, &ofApp::exitButtonClicked);
    //ofAddListener(defaultButton.clickEvent, this, &ofApp::defaultButtonClicked);
    ofAddListener(mappingButton.clickEvent, this, &ofApp::mappingButtonClicked);

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
void ofApp::saveButtonClicked() {
    save();
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
void ofApp::mappingButtonClicked() {
    sandbox.setMapping(!sandbox.getMapping());
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
void ofApp::save() {
    //#ifdef CALIBRATION_MODE
    string sand1 = "../../../city-transform-calibration/bin/data/sandboxSettings.xml";
    string sand2 = "../../../city-transform-calibration/bin/data/settings.xml";
    sandbox.saveSettings(sand1, sand2);
    //#endif
}

//--------------------------------------------------------------
void ofApp::exit() {
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

    // update camera focus if needed (wait 30 frames)
    if (ofGetFrameNum() % 30 == 0){
        int focusAbsoluteNext = sandbox.getCameraFocus();
        focusAbsoluteNext -= (focusAbsoluteNext%5);
        if (focusAbsoluteNext != focusAbsolute) {
            string cmd = "v4l2-ctl -d "+cameraHardwareName+" --set-ctrl=focus_absolute="+ofToString(focusAbsolute);
            ofLog() << "Run: "<< cmd;
            ofSystem(cmd);
            focusAbsolute = focusAbsoluteNext;
        } 
    }

    //sandbox.setThreshold(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255));

    if (srcMode==0) {
        if (!runway.getBusy() || debug) {
            cam.update();
            if(cam.isFrameNew()) {
                src.setFromPixels(cam.getPixels());
                updateSandbox();
            }
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
//    checkFullscreen();

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
    mappingButton.draw();
    saveButton.draw();
    exitButton.draw();
    //defaultButton.draw();
    if (outputTex.isAllocated()) {
        outputTex.draw(5, ofGetHeight()-5-outputTex.getHeight());
    }

    if (initialDebug.getActive()) {
        ofSetColor(ofColor::red);
        font.drawString("Will go to main app in "+ofToString(int(20-initialDebug.getElapsedTime()))+" seconds...", 700, 24);
        ofSetColor(ofColor::white);
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
        #ifndef CALIBRATION_MODE
        debug = !debug;
        #endif
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (!debug) return;
    sandbox.mouseMoved(x, y);
    saveButton.mouseMoved(x, y);
    exitButton.mouseMoved(x, y);
    //defaultButton.mouseMoved(x, y);
    mappingButton.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (!debug) return;
    sandbox.mouseDragged(x, y);
    saveButton.mouseDragged(x, y);
    exitButton.mouseDragged(x, y);
    //defaultButton.mouseDragged(x, y);
    mappingButton.mouseDragged(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (!debug) return;
    sandbox.mousePressed(x, y);
    saveButton.mousePressed(x, y);
    exitButton.mousePressed(x, y);
    //defaultButton.mousePressed(x, y);
    mappingButton.mousePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (!debug) return;
    sandbox.mouseReleased(x, y);
    saveButton.mouseReleased(x, y);
    exitButton.mouseReleased(x, y);
    //defaultButton.mouseReleased(x, y);
    mappingButton.mouseReleased(x, y);
}

//--------------------------------------------------------------
void ofApp::timerUpEvent() {
    debug = false;
}
