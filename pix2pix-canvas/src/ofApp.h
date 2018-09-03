#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCanvas.h"
#include "ofxMSATensorFlow.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	ofxPanel panel;


	ofxCanvas canvas;

	void load_model(string model_dir);
	void load_model_index(int index);


	msa::tf::SimpleModel model;

    // a bunch of properties of the models
    // ideally should read from disk and vary with the model
    // but trying to keep the code minimal so hardcoding them since they're the same for all models
    const int input_shape[2] = {512, 512}; // dimensions {height, width} for input image
    const int output_shape[2] = {512, 512}; // dimensions {height, width} for output image
    const ofVec2f input_range = {-1, 1}; // range of values {min, max} that model expects for input
    const ofVec2f output_range = {-1, 1}; // range of values {min, max} that model outputs
    const string input_op_name = "generator/generator_inputs"; // name of op to feed input to
    const string output_op_name = "generator/generator_outputs"; // name of op to fetch output from



    // model file management
    ofDirectory models_dir;    // data/models folder which contains subfolders for each model
    int cur_model_index = 0; // which model (i.e. folder) we're currently using



    ofFloatImage img_in, img_out;
    ofPixels pix_in;


/*
    // fbo for drawing into (will be fed to model)
    ofFbo fbo;

    // images in and out of model
    // preallocating these to save allocating them every frame
    ofFloatImage img_in; // input to the model (read from fbo)
    ofFloatImage img_out; // output from the model



    // color management for drawing
    vector<ofColor> colors; // contains color palette to be used for drawing (loaded from data/models/XYZ/palette.txt)
    const int palette_draw_size = 50;
    int draw_color_index = 0;
    ofColor draw_color;


    // other vars
    bool do_auto_run = true;    // auto run every frame
    int draw_mode = 0;     // draw vs boxes
    int draw_radius = 10;
    ofVec2f mousePressPos;
*/
};
