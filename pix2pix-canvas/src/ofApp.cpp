#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(1600, 900);
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
    load_model_index(2); // load first model

    // setup drawing canvas
	canvas.setup(10, 10, input_shape[1], input_shape[0], 100, false);
    canvas.addUndoOption("undo");
    canvas.addShapeOption("water", ofColor::blue, 10, 100);
    canvas.addShapeOption("park", ofColor::green, 10, 100);
    canvas.addShapeOption("buildings", ofColor::red, 5, 50);
    canvas.addLineOption("roads", ofColor::black, 1, 10);
    canvas.addSlider("size", 0, 1);
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

    canvas.getCanvas().readToPixels(pix_in);
    img_in.setFromPixels(pix_in);

    img_in.load("inp1.png");
/*
    if (mouseX < 400) {
	    img_in.load("inp1.png");
	} else if (mouseX < 800) {
	    img_in.load("inp2.png");
	} else if (mouseX < 1200) {
		img_in.load("inp3.png");
	} else  {
		img_in.load("inp4.png");
	}
*/
	model.run_image_to_image(img_in, img_out, input_range, output_range);
}

//--------------------------------------------------------------
void ofApp::draw(){
    canvas.draw();
    canvas.drawGui();


	ofSetColor(255);
    img_in.draw(1600-200, 0, 200, 200);
	img_out.draw(720, 100);
	
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
