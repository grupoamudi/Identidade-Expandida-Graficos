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
    
    //shader.load("Forward.vert", "Forward.frag");
    //shader.setUniform3f("lightPos", 0, 0, 300);
    startupTime = ofGetSystemTime();
    
    daemon = make_unique<FileDaemon>(&this->mesh, "TestObj.obj");
    //mesh = make_shared<FingerMesh>();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Animation part
    if (mesh) {
        const float phase = M_PI * (ofGetSystemTime() % 2048) / 1024.0f;
        const float offset = M_PI * 0.15f;
        for (int x = 0; x < mesh->size(); x++) {
            mesh->setHeight(x, 1.0f + 3.0f * powf((1.0f + cosf(phase - offset * x)) * 0.5f, 1.5));
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	static int x = 0;
    
    if (statsEnabled) {
        if (daemon->isFileReady()) {
            int numPolys = mesh ? accumulate(mesh->begin(), mesh->end(), 0, [](int acc, ofMesh mesh) { return acc + mesh.getIndices().size() - 6 ;}) : 0;
            string strPolys = "Num. Polys in scene: " + to_string(numPolys) + "\n";
            strPolys += "FPS: " + to_string(ofGetFrameRate()) + "\n";
            strPolys += "GPU load: ";
            strPolys += to_string(lastFrameTime * .006f) + " %";
            
            ofDrawBitmapStringHighlight(strPolys, 10, 20);
        }
        else {
            ofDrawBitmapStringHighlight("No model currently loaded.", 10, 20);
        }
    }
    
    ofPushMatrix();
    ofTranslate(512 + float(10) * sinf(x / float(30.0)), 384);
    ofScale(5.0f, 5.0f);
    ofRotate(mouseY * (1.0/6.0f), 1, 0, 0);
    ofRotate(mouseX * (1.0/6.0f), 0, 1, 0);
    //ofRotate(x * 0.1f, 0, 0, 1);
    uint64_t timeBegin = ofGetSystemTimeMicros();
    
    // We have to first check if the mesh is ready before rendering,
    //  or we might stumble upon cases where this thread is acessing
    //  an incomplete mesh object, therefore crashing the program.
    if (daemon->isFileReady()){
        if (mesh) {
            mesh->draw();
        }
    }
    lastFrameTime = ofGetSystemTimeMicros() - timeBegin;
    ofPopMatrix();
    
	x++;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        statsEnabled ^= 1;
    }
    if (key == 'f') {
        dummyFile ^= 1;
        if (dummyFile) {
            ofstream dummy("TestFile.obj", ios::trunc);
            dummy << "v 0 0\nv 1 0\nv 1 1\nv 1 0\nf 0 1 2 3 0";
        }
        else {
            remove("TestFile.obj");
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    //light.setPosition(x, y, 300);
    mouseX = x;
    mouseY = y;
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
