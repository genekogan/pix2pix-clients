#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(100);
    
    //host = "http://d6e64b8d.ngrok.io/infer";
    host = "http://7a942e8a.ngrok.io/infer";
    
    width = 1024;
    height = 512;
    debug = true;
    isReady = true;
    
    // setup drawing canvas
    canvas.setup(10, 10, width, height, 100, false);
    canvas.setBackground(ofColor(0));
    canvas.addUndoOption("undo");
    canvas.addLineOption("roads", ofColor::white, 1, 10);
    canvas.addSlider("size", 0, 1);

    // init images
    input.allocate(width, height, OF_IMAGE_COLOR);
    output.allocate(width, height, OF_IMAGE_COLOR);
    
    // setup Runway client
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup(host);
    runway.start();
    
    img.load("/Users/gene/Downloads/02830_young-girl-seated-in-a-meadow-1865_0_input_label.jpg");
    canvas.clearCanvas();
}

//--------------------------------------------------------------
void ofApp::update(){
    //canvas.getCanvas().readToPixels(input);
    canvas.getCanvas().readToPixels(input);
    
    if (toSend) {
        input.save("hello.jpg");
        input.load("hello.jpg");
        runway.send(input.getPixels());
        toSend = false;
    }

    
    ofPixels processedPixels;
    while (runway.tryReceive(processedPixels)) {
        outputTex.loadData(processedPixels);
    }
    
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    canvas.draw();
    canvas.drawGui();

//
//    ofSetColor(255);
    outputTex.draw(ofGetWidth()-1024, ofGetHeight()-512);
	
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' '){
        toSend = true;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    canvas.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    canvas.mouseDragged(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    canvas.mousePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    canvas.mouseReleased(x, y);
}

