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
    sendInput();
}

void ofApp::sendInput()
{
	// host
    //std::string uri = "https://httpbin.org/post";
    std::string uri = "http://localhost:23100/infer";
    
    // encode the image Base64
    std::string encodedImage = ofxIO::Base64Encoding::encode(ofBufferFromFile("input2.png"));

    // data for form request
    std::multimap<std::string, std::string> formFields =
    {
        { "data", encodedImage }
    };

    // create request
    ofxHTTP::Client client;
    ofxHTTP::PostRequest request(uri);
    request.addFormFields(formFields)
;
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
            img.setFromPixels(pixels);
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


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
    if (img.isAllocated()) {
	    img.draw(50, 50);
	}
}
