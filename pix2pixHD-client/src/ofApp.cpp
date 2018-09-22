#include "ofApp.h"


void ofApp::setup() {
    ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(0);

    //host = "http://d6e64b8d.ngrok.io/infer";
    host = "YOUR_HOST";
    width = 1024;
    height = 512;
    debug = true;
    srcMode = 2;
    numTrackingColors = 3;
    isReady = true;

    // setup cv
    sandbox.setup(width, height, numTrackingColors);
    sandbox.setFilterColor(0, ofColor(41, 83, 74));
    sandbox.setFilterColor(1, ofColor(223, 171, 37));
    sandbox.setFilterColor(2, ofColor(199, 194, 196));
    sandbox.setThreshold(220);

    // setup input
    if (srcMode==0) {
        cam.setup(640, 480);
    }
    else if (srcMode==1) {
        src.load("test3.png");
        src.resize(width, height);
    }
    else if (srcMode==2) {
        video.load("/Users/gene/Documents/futurium_test2.mp4");
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();
    }

    // init images
    inputPixels.allocate(width, height, OF_IMAGE_COLOR);
    input.allocate(width, height, OF_IMAGE_COLOR);
    output.allocate(width, height, OF_IMAGE_COLOR);
    
    // setup Runway client
    ofLog::setChannel(std::make_shared<ofxIO::ThreadsafeConsoleLoggerChannel>());
    runway.setup(host);
    runway.start();
    ofAddListener(ofxRunway::runwayEvent, this, &ofApp::receivedFromRunway);
}

void ofApp::update(){
    if (srcMode==1) {
        cam.update();
        if(cam.isFrameNew()) {
            src.setFromPixels(cam.getPixels());
            src.resize(width, height);
            updateSandbox();
        }
    }
    else if (srcMode==2) {
        video.update();
        if(video.isFrameNew()) {
            src.setFromPixels(video.getPixels());
            src.resize(width, height);
            updateSandbox();
        }
    }
    else if (srcMode == 0) {
        updateSandbox();
    }
}

void ofApp::updateSandbox(){
    sandbox.update(src);
    sandbox.getFbo().readToPixels(inputPixels);
    input.setFromPixels(inputPixels);
    input.resize(width, height);
    //ofLog() << img_in.getImageType();
    
    if (isReady) {
        runway.send(input);
        runway.start();
        isReady = false;
    }
    
    //sendInput();
}


void ofApp::receivedFromRunway() {
    ofLog() << "received from runway";
    
    ofPixels p2;
    ofLoadImage(p2, ofBuffer(runway.getOutputBuffer().getCharPtr(), runway.getOutputBuffer().size()));
    output.setFromPixels(p2);  // <-- CRASHING!
    
    isReady = true;
}

void ofApp::draw() {
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
    if (output.isAllocated()) {
        output.draw(50, 50+512);
    }
    if (input.isAllocated()){
        input.draw(50, 50);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    int x = ofGetMouseX();
    int y = ofGetMouseY();
    
    unsigned char * pixels = src.getPixels().getData();
    int index = 3 * (y * src.getWidth() + x);
    int r = pixels[index];
    int g = pixels[index+1];
    int b = pixels[index+2];
    
    if (key=='1') {
        sandbox.setFilterColor(0, ofColor(r, g, b));
    }
    else if (key=='2') {
        sandbox.setFilterColor(1, ofColor(r, g, b));
    }
    else if (key=='3') {
        sandbox.setFilterColor(2, ofColor(r, g, b));
    }

    else if (key==' '){
        debug = !debug;
    }
}

/*
void ofApp::sendInput() {
    
    ofSaveImage(img_in.getPixels(), buffer_in, OF_IMAGE_FORMAT_JPEG);
    multimap<string, string> formFields = {{ "data", ofxIO::Base64Encoding::encode(buffer_in) }};
    
    // create request
    ofxHTTP::Client client;
    ofxHTTP::PostRequest request(host);
    request.addFormFields(formFields);
    
    try
    {
        ofLog() << "execute request";
        auto response = client.execute(request);
        
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            // A successful response.
            //ofLogNotice("ofApp::setup") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
            
            ofLogNotice("ofApp::setup") << "Content Begin";
            //std::cout << response->stream() << std::endl;
            ofLogNotice("ofApp::setup") << "Content End";
            
            // decode Base64 buffer
            ofxIO::ByteBuffer encodedBuffer(response->buffer());
            ofxIO::ByteBuffer decodedBuffer;
            ofxIO::Base64Encoding encoding;
            encoding.decode(encodedBuffer, decodedBuffer);
            
            // Now save the buffer back to disk.
            ofxIO::ByteBufferUtils::saveToFile(decodedBuffer, "decoded_input.png");
            
            ofPixels pixels;
            ofLoadImage(pixels, ofBuffer(decodedBuffer.getCharPtr(), decodedBuffer.size()));
            img_out.setFromPixels(pixels);
        }
        else
        {
            //ofLogError("ofApp::setup") << response->getStatus() << " " << response->getReason();
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << exc.displayText();
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::setup") << exc.what();
    }
}
*/
