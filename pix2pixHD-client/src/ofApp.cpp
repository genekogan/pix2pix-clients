//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


void ofApp::setup()
{
    ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(0);

    width = 1024;
    height = 512;


    //sendInput();
    img.load("inputtest.jpg");

    debug = true;

    // setup cv
    sandbox.setup(width, height, numTrackingColors);

    if (srcMode==0) {
        cam.setup(640, 480);
    } else if (srcMode==1) {
        src.load("test3.png");
        src.resize(width, height);
    } else if (srcMode==2) {
        video.load("/home/gene/of_v0.10.0_linux64gcc5_release/apps/pix2pix/pix2pix-cv/bin/data/futurium_test2.mp4");
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();
    }

    // defaults
    sandbox.setFilterColor(0, ofColor(41, 83, 74));
    sandbox.setFilterColor(1, ofColor(223, 171, 37));
    sandbox.setFilterColor(2, ofColor(199, 194, 196));
    sandbox.setThreshold(220);

    // init images
    fbo.allocate(width, height, GL_RGB);
    pix_in.allocate(width, height, OF_IMAGE_COLOR);
    img_in.allocate(width, height, OF_IMAGE_COLOR);
    img_out.allocate(width, height, OF_IMAGE_COLOR);

    
}

void ofApp::sendInput()
{
	// host
    //std::string uri = "https://httpbin.org/post";
    std::string uri = "http://localhost:23100/infer";
    
    // encode the image Base64
    //string encodedImage = ofxIO::Base64Encoding::encode(ofBufferFromFile("00000010.png"));
    

    ofBuffer img_in_buffer;
    ofSaveImage(img_in.getPixels(), img_in_buffer, OF_IMAGE_FORMAT_JPEG);
    string encodedImage = ofxIO::Base64Encoding::encode(img_in_buffer);





    // data for form request
    std::multimap<std::string, std::string> formFields =
    {
        { "data", encodedImage }
    };

    // create request
    ofxHTTP::Client client;
    ofxHTTP::PostRequest request(uri);
    request.addFormFields(formFields);

    try
    {
        ofLog() << "execute request";
        auto response = client.execute(request);
        
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            // A successful response.
            ofLogNotice("ofApp::setup") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";

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
            ofLogError("ofApp::setup") << response->getStatus() << " " << response->getReason();
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


void ofApp::update(){

    
    if (srcMode==1) {
        cam.update();
        if(cam.isFrameNew()) {
            src.setFromPixels(cam.getPixels());
            src.resize(width, height);
        }
    }
    else if (srcMode==2) {
        video.update();
        if(video.isFrameNew()) {
            src.setFromPixels(video.getPixels());
            src.resize(width, height);
        }
    }

    //float thresh = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255);
    //sandbox.setThreshold(thresh);
    

    sandbox.update(src);

    
    

    sandbox.getFbo().readToPixels(pix_in);
    img_in.setFromPixels(pix_in);
    img_in.resize(width, height);
    ofLog() << img_in.getImageType();


    sendInput();

}

void ofApp::draw2()
{

    ofBackground(0);
    
    ofSetColor(255);

    if (debug) {
        src.draw(0, 0);
        sandbox.draw(width, 0);
        sandbox.drawDebug(0, height);
        img_out.draw(2*width, 0, width, height);
    }
    else {
        img_out.draw(0, 0, width*2, height*2);
        img_in.draw(ofGetWidth()-256, 0, 0, 256, 256);
    }
}

void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
    if (img_out.isAllocated()) {
	    img_out.draw(50, 50+512);
	}
    if (img_in.isAllocated()){
        img_in.draw(50, 50);
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
        cout << "1 = " << r<< " " << g << " " << b << endl;
    }
    else if (key=='2') {
        sandbox.setFilterColor(1, ofColor(r, g, b));
        cout << "2 = " << r<< " " << g << " " << b << endl;
    }
    else if (key=='3') {
        sandbox.setFilterColor(2, ofColor(r, g, b));
        cout << "3 = " << r<< " " << g << " " << b << endl;
    }

    else if (key==' '){
        debug = !debug;
    }
}
