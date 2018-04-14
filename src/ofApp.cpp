#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
    ofEnableDepthTest();
	ofEnableLighting();
    
    light.setPointLight();
    light.setAmbientColor(0.2f);
    light.setDiffuseColor(0.6f);
    light.setPosition(512, 0, 300);
    light.enable();
    
    m = FingerMesh("Testmesh.dae");
    
    //shader.load("Forward.vert", "Forward.frag");
    //shader.setUniform3f("lightPos", 0, 0, 300);
}

//--------------------------------------------------------------
void ofApp::update(){
    const float phase = M_PI * (ofGetSystemTime() % 2048) / 1024.0f;
    const float offset = M_PI * 0.15f;
    for (int x = 0; x < m.size(); x++) {
        m.setHeight(x, 1.0f + 3.0f * powf((1.0f + cosf(phase - offset * x)) * 0.5f, 1.5));
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	static int x = 0;
    
    if (statsEnabled) {
        int numPolys = accumulate(m.begin(), m.end(), 0, [](int acc, ofMesh mesh) { return acc + mesh.getIndices().size() - 6 ;});
        string strPolys = "Num. Polys in scene: " + to_string(numPolys) + "\n";
        strPolys += "FPS: " + to_string(ofGetFrameRate()) + "\n";
        strPolys += "GPU load: ";
        strPolys += to_string(lastFrameTime * .006f) + " %";
        
        ofDrawBitmapStringHighlight(strPolys, 10, 20);
    }
    
    ofPushMatrix();
    ofTranslate(512 + float(10) * sinf(x / float(30.0)), 384);
    ofScale(25.0f, 25.0f);
    ofRotate(60, 1, 0, 0);
    //ofRotate(x * 0.1f, 0, 0, 1);
    uint64_t timeBegin = ofGetSystemTimeMicros();
    m.draw();
    lastFrameTime = ofGetSystemTimeMicros() - timeBegin;
    ofPopMatrix();
    
	x++;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        statsEnabled ^= 1;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    //light.setPosition(x, y, 300);
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
