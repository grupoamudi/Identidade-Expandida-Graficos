#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
private:
	// Contains the meshes loaded from the text file.
	vector<ofVboMesh> meshes;
	// Contains light sources, defined ramdomly.
	vector<ofLight> lights;

	// Points to the text file itself.
	ifstream txtFile;
	bool fileLoading = false;

	// Keeps track of app state.
	enum AppState {
		AppIdle,
		AppReading,
		AppDrawing
	} appState;

	public:
		void setup(); // Runs once
		void update(); // Runs immediately before draw()
		void draw(); // Runs once every frame


		// Since this work is non-interactive, we don't need
		//  to actually implement those functions.
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
