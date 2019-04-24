#include "ofMain.h"
#include "ofApp.h"


#define PRESENT

//========================================================================
int main( ){
//	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
//	ofRunApp(new ofApp());


	ofGLFWWindowSettings settings;
    settings.setSize(1920, 1080);
	settings.setPosition(ofVec2f(3840*2, 0));
	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
//	mainWindow->setFullscreen(true);

    settings.setSize(2560, 1440);
//    settings.setSize(1440, 810);
	settings.setPosition(ofVec2f(3840,0));
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
