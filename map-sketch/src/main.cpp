#include "ofMain.h"
#include "ofApp.h"



//========================================================================
int main( ){
//	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
//	ofRunApp(new ofApp());

	int monitor_presentation_id = 1;
	int monitor_touchscreen_id = 2;
	string path = "master_settings.json";
	ofFile file(path);
	if (!file.exists()) {
		ofLog() << "ERROR: Can't find lookup file, don't know camera_id, using default 0";
	} else {
		ofJson json = ofLoadJson(path);
		monitor_presentation_id = json["monitor_id"]["sketch_transform_presentation"];
		monitor_touchscreen_id = json["monitor_id"]["touchscreen"];
	}

	ofGLFWWindowSettings settings;
	settings.setSize(1920, 1080);
	settings.setPosition(ofVec2f(monitor_touchscreen_id * 3840, 0));
	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
//	mainWindow->setFullscreen(true);

	settings.setSize(2560, 1440);
//    settings.setSize(1440, 810);
#ifdef PRESENT
	settings.setPosition(ofVec2f(monitor_presentation_id * 3840,0));
#endif
	settings.resizable = true;
	settings.shareContextWith = mainWindow;	
	
#ifdef PRESENT
    shared_ptr<ofAppBaseWindow> presentWindow = ofCreateWindow(settings);
    presentWindow->setVerticalSync(false);
  //  presentWindow->setFullscreen(true);
#endif
	
    shared_ptr<ofApp> mainApp(new ofApp);
	mainApp->setupMain();
	
#ifdef PRESENT
    ofAddListener(presentWindow->events().draw,mainApp.get(),&ofApp::drawMain);
#endif

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
