#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.width = 1024;
    settings.height = 768;
    settings.windowMode = OF_WINDOW; // later OF_FULLSCREEN
    ofCreateWindow(settings);
    
	ofRunApp(new ofApp());

}
