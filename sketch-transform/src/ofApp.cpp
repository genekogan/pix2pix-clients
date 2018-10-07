#include "ofApp.h"


using namespace cv;
using namespace ofxCv;


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(100);
    
    //host = "http://d6e64b8d.ngrok.io/infer";
    host = "http://7a942e8a.ngrok.io/infer";
    
    width = 512;
    height = 256;
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
    //runway.setup(host);
    //runway.start();
    
    img.load("/Users/gene/Downloads/02830_young-girl-seated-in-a-meadow-1865_0_input_label.jpg");
    canvas.clearCanvas();
    
    
    inputGray.allocate(width, height, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    canvas.getCanvas().readToPixels(input);
    convertColor(input, inputGray, CV_RGB2GRAY);
    threshold(inputGray, 100);
    input.update();
    inputGray.update();
    
    
    if (toSend) {
        runway.send(input.getPixels());
        toSend = false;
    }

    
    ofPixels processedPixels;
    while (runway.tryReceive(processedPixels)) {
        outputTex.loadData(processedPixels);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    canvas.draw();
    canvas.drawGui();

    
    if (input.isAllocated()){
        input.draw(width + 85, 0);
    }
    if (inputGray.isAllocated()) {
        inputGray.draw(width+95, height+5);
    }


    ofSetColor(255);
    if (outputTex.isAllocated()) {
        //outputTex.draw(ofGetWidth()-1024, ofGetHeight()-512);
    }
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), ofGetWidth()-80, 12);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' '){
        //toSend = true;
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

