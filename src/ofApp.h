#pragma once

#include "ofMain.h"
#include "FingerMesh.h"
#include "FileDaemon.h"
#include <fstream>
#include <memory>
#include <future>

class ofApp : public ofBaseApp {
private:
    shared_ptr<FingerMesh> mesh;
    unique_ptr<FileDaemon> daemon;
    ofLight light;
    ofShader shader;
    ofImage palette;
    bool statsEnabled = false;
    bool dummyFile = false;
    uint64_t lastFrameTime;
    uint64_t startupTime;
    int mouseX = 512;
    int mouseY = 384;

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
		
};
