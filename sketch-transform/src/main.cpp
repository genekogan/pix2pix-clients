#include "ofMain.h"
#include "ofApp.h"



//========================================================================
int main( ){

	int monitor_presentation_id = 1;
	int monitor_touchscreen_id = 2;
	int monitor_presentation_order = 1;
	int monitor_touchscreen_order = 2;
	int resolution_px = 1920;
	int resolution_py = 1080;
	int resolution_tx = 1920;
	int resolution_ty = 1080;
	int position_px = 0;
	int position_tx = 0;

	string path = "master_settings.json";
	ofFile file(path);
	if (!file.exists()) {
		ofLog() << "ERROR: Can't find lookup file, don't know camera_id, using default 0";
	} 
	else {
		ofJson json = ofLoadJson(path);
		monitor_presentation_id = json["monitor_id"]["sketch_transform"];
		monitor_touchscreen_id = json["monitor_id"]["touchscreen"];

		for (int i=0; i<4; i++) {
			if (json["monitor_order"][i] == monitor_presentation_id) {
				monitor_presentation_order = i;
			}
			if (json["monitor_order"][i] == monitor_touchscreen_id) {
				monitor_touchscreen_order = i;
			}
		}
cout << "GOT ORDERS " << monitor_presentation_order << " and t " << monitor_touchscreen_order << endl;

		resolution_px = json["monitor_resolution"][ofToString(monitor_presentation_id)][0];
		resolution_py = json["monitor_resolution"][ofToString(monitor_presentation_id)][1];
		resolution_tx = json["monitor_resolution"][ofToString(monitor_touchscreen_id)][0];
		resolution_ty = json["monitor_resolution"][ofToString(monitor_touchscreen_id)][1];

		for (int i=0; i<monitor_presentation_order; i++) {
			int px = json["monitor_resolution"][ofToString(json["monitor_order"][i])][0];
			cout << "ADD Px = "<< px << " to " << "position_x" << endl;
			position_px += px;
		}
		for (int i=0; i<monitor_touchscreen_order; i++) {
			int tx = json["monitor_resolution"][ofToString(json["monitor_order"][i])][0];
			cout << "ADD Tx = "<< tx << " to " << "position_t" << endl;
			position_tx += tx;
		}

		ofLog() << "Touchscreen monitor: id "<<monitor_touchscreen_id<<", resolution [" << resolution_tx << ", " <<resolution_ty << "], x-position " << position_tx; 
		ofLog() << "Presentation monitor: id "<<monitor_presentation_id<<", resolution [" << resolution_px << ", " <<resolution_py << "], x-position " << position_px; 
	}

	ofGLFWWindowSettings settingsT;
	settingsT.setSize(resolution_tx, resolution_ty);
	settingsT.setPosition(ofVec2f(position_tx, 0));
	settingsT.resizable = true;
	//settingsT.windowMode = OF_GAME_MODE;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settingsT);


#ifdef PRESENT
	ofGLFWWindowSettings settingsP;
	settingsP.setSize(resolution_px, resolution_py);
	settingsP.setPosition(ofVec2f(position_px, 0));
	settingsP.resizable = true;
	//settingsP.windowMode = OF_GAME_MODE;
	settingsP.shareContextWith = mainWindow;	
    shared_ptr<ofAppBaseWindow> presentWindow = ofCreateWindow(settingsP);
    presentWindow->setVerticalSync(false);
#endif
	
    shared_ptr<ofApp> mainApp(new ofApp);
	mainApp->setupMain();
	
#ifdef PRESENT
    ofAddListener(presentWindow->events().draw, mainApp.get(), &ofApp::drawMain);
#endif

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
