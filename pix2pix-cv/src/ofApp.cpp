#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(1920, 1080);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(0);

    // scan models dir
    models_dir.listDir("models");
    if(models_dir.size()==0) {
        ofLogError() << "Couldn't find models folder." << msa::tf::missing_data_error();
        assert(false);
        ofExit(1);
    }
    models_dir.sort();
    load_model_index(0); // load first model

    
    width = input_shape[1];
    height = input_shape[0];

    debug = true;

    // setup cv
    sandbox.setup(width, height, numTrackingColors);

    if (srcMode==0) {
        cam.setup(640, 480);
    } else if (srcMode==1) {
        src.load("test3.png");
        src.resize(width, height);
    } else if (srcMode==2) {
        video.load("futurium_test2.mp4");
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();
    }

    // defaults
    sandbox.setFilterColor(0, ofColor(41, 83, 74));
    sandbox.setFilterColor(1, ofColor(223, 171, 37));
    sandbox.setFilterColor(2, ofColor(199, 194, 196));
    sandbox.setThreshold(220);
}


//--------------------------------------------------------------
// Load graph (model trained in and exported from python) by folder NAME, and initialise session
void ofApp::load_model(string model_dir) {
    ofLogVerbose() << "loading model " << model_dir;

    // init the model
    // note that it expects arrays for input op names and output op names, so just use {}
    model.setup(ofFilePath::join(model_dir, "graph_frz.pb"), {input_op_name}, {output_op_name});
    if(! model.is_loaded()) {
        ofLogError() << "Model init error." << msa::tf::missing_data_error();
        assert(false);
        ofExit(1);
    }

    // init tensor for input. shape should be: {batch size, image height, image width, number of channels}
    model.init_inputs(tensorflow::DT_FLOAT, {1, input_shape[0], input_shape[1], 3});

    // initialize images
    ofLogVerbose() << "allocating fbo and images " << input_shape;
    fbo.allocate(input_shape[1], input_shape[0], GL_RGB);
	pix_in.allocate(input_shape[1], input_shape[0], OF_IMAGE_COLOR);
    img_in.allocate(input_shape[1], input_shape[0], OF_IMAGE_COLOR);
    img_out.allocate(output_shape[1], output_shape[0], OF_IMAGE_COLOR);


/*
    // allocate fbo and images with correct dimensions, and no alpha channel
    fbo.allocate(input_shape[1], input_shape[0], GL_RGB);
    img_in.allocate(input_shape[1], input_shape[0], OF_IMAGE_COLOR);
    img_out.allocate(output_shape[1], output_shape[0], OF_IMAGE_COLOR);

    // load test image
    ofLogVerbose() << "loading test image";
    ofImage img;
    img.load(ofFilePath::join(model_dir, "test_image.png"));
    if(img.isAllocated()) {
        fbo.begin();
        ofSetColor(255);
        img.draw(0, 0, fbo.getWidth(), fbo.getHeight());
        fbo.end();

    } else {
        ofLogError() << "Test image not found";
    }

    // load color palette for drawing
    ofLogVerbose() << "loading color palette";
    colors.clear();
    ofBuffer buf;
    buf = ofBufferFromFile(ofFilePath::join(model_dir, "/palette.txt"));
    if(buf.size()>0) {
        for(const auto& line : buf.getLines()) {
            ofLogVerbose() << line;
            if(line.size() == 6) // if valid hex code
                colors.push_back(ofColor::fromHex(ofHexToInt(line)));
        }
        draw_color_index = 0;
        if(colors.size() > 0) draw_color = colors[0];
    } else {
        ofLogError() << "Palette info not found";
    }

    // load default brush info
    ofLogVerbose() << "loading default brush info";
    buf = ofBufferFromFile(ofFilePath::join(model_dir, "/default_brush.txt"));
    if(buf.size()>0) {
        auto str_info = buf.getFirstLine();
        ofLogVerbose() << str_info;
        auto str_infos = ofSplitString(str_info, " ", true, true);
        if(str_infos[0]=="draw") draw_mode = 0;
        else if(str_infos[0]=="box") draw_mode = 1;
        else ofLogError() << "Unknown draw mode: " << str_infos[0];

        draw_radius = ofToInt(str_infos[1]);

    } else {
        ofLogError() << "Default brush info not found";
    }
*/

}

//--------------------------------------------------------------
void ofApp::load_model_index(int index) {
    cur_model_index = ofClamp(index, 0, models_dir.size()-1);
    load_model(models_dir.getPath(cur_model_index));
}


//--------------------------------------------------------------
void ofApp::update(){

	
    if (srcMode==1) {
        cam.update();
        if(cam.isFrameNew()) {
            src.setFromPixels(cam.getPixels());
            src.resize(input_shape[1], input_shape[0]);
        }
    }
    else if (srcMode==2) {
        video.update();
        if(video.isFrameNew()) {
            src.setFromPixels(video.getPixels());
            src.resize(input_shape[1], input_shape[0]);
        }
    }

    //float thresh = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255);
    //sandbox.setThreshold(thresh);
    

    sandbox.update(src);

	
    

    sandbox.getFbo().readToPixels(pix_in);
    img_in.setFromPixels(pix_in);
    img_in.resize(input_shape[1], input_shape[0]);
    ofLog() << img_in.getImageType();
	model.run_image_to_image(img_in, img_out, input_range, output_range);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
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

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
