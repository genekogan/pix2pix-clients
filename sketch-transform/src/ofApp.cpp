#include "ofApp.h"


using namespace cv;
using namespace ofxCv;



//--------------------------------------------------------------
void ofApp::setupMain(){
	ofSetBackgroundColor(0);
}

//--------------------------------------------------------------
void ofApp::drawMain(ofEventArgs & args){
    drawPresent();
}

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetWindowShape(1920, 1080);
    ofSetFullscreen(true);
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
    gui.setPosition(10, 920);
}

//--------------------------------------------------------------
void ofApp::update(){    
    canvas.getCanvas().readToPixels(input);
    input.update();

    if (canvas.isFrameNew()) {
        toSend = true;
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
    ofImage fave;
    output.readToPixels(fave);
    favorites.push_back(fave);    
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

    int w = ofGetWidth()-20;
    int h = int(float(w) / (output.getWidth() / output.getHeight()));

    if (output.isAllocated()) {
        output.draw(10, 10, w, h);
    }
}

//--------------------------------------------------------------
void ofApp::drawFavorites(){
    ofBackground(0);
    
    for (int f=0; f<favorites.size(); f++) {
        float x = 10 + (f%3) * 370;
        float y = 10 + int(f/3) * 190;
        favorites[f].draw(x, y, 360, 180);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ') {
        mode = (mode + 1) % 3;
    }    
    else if (key == 'f') {
        saveFavorite();
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
