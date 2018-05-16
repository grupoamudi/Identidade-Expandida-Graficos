#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// Capping Framerate to 60, else we'll waste CPU time
	ofSetFrameRate(60);
	// VSync ON to avoid tearing
	ofSetVerticalSync(true);
	// If we can afford, MSAA is also on
	ofEnableSmoothing();
	
	// Activating important parts of GL pipeline
	ofEnableDepthTest();
	ofEnableLighting();

	lights.push_back(ofLight());
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	static int x = 0;
	lights[0].setPosition(512 + float(200) * sinf(x / float(473.0)), 0, 20);
	lights[0].enable();

	ofDrawSphere(512 + float(30) * sinf(x / float(30.0)), 384, -5, 200);
	x++;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
