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

    width = 1024;
    height = 512;
    debug = true;
    ready = true;
    toSend = false;

    // setup drawing canvas
    canvas.setup(10, 10, width, height, 100, false);
    canvas.setBackground(ofColor(255));
    canvas.addUndoOption("undo");
    canvas.addLineOption("roads", ofColor::black, 1, 10);
    canvas.addSlider("size", 0, 1);

    // init images
    input.allocate(width, height, OF_IMAGE_COLOR);
    output.allocate(width, height, OF_IMAGE_COLOR);

    // setup Runway client
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup("http://localhost:9000");
    runway.start();

    img.load("/home/bzion/projects/pix2pixHD/checkpoints/renais_025_025_ngfndf16/web/images/epoch006_input_label.jpg");
    canvas.clear();
    canvas.setFromPixels(img.getPixels());

//    inputGray.allocate(width, height, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void ofApp::update(){

    canvas.getCanvas().readToPixels(input);
    //convertColor(input, inputGray, CV_RGB2GRAY);
    //threshold(inputGray, 100);
    input.update();
    //inputGray.update();


    if (toSend) {
        ofxRunwayBundle bundle;
        bundle.address = "convert";
        bundle.images["image"] = input.getPixels();
        runway.send(bundle);
        toSend = false;
        ready = false;
        cout << "IT GOT SENT " << endl;
    }

    ofxRunwayBundle bundleToReceive;
    while (runway.tryReceive(bundleToReceive)) {
        ofPixels processedPixels = bundleToReceive.images["output"];
        outputTex.loadData(processedPixels);
        cout << 11111 << endl;
        ready = true;
    }

    ofSetWindowTitle(ofToString(ofGetFrameRate()));
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
        outputTex.draw(ofGetWidth()-1024, ofGetHeight()-512);
    }
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), ofGetWidth()-80, 12);

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
