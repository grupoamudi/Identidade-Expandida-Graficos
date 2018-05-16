#include "ofApp.h"

//--------------------------------------------------------------
void ofApp :: setup() {
	ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofDisableArbTex();
    
    /*light.setPointLight();
    light.setAmbientColor(0.2f);
    light.setDiffuseColor(0.6f);
    light.setPosition(512, 0, 300);
    light.enable();*/
    
    palette = ofImage("Palette.png");
    
    shader.load("FingerShader.vert", "FingerShader.frag");
    
    startupTime = ofGetSystemTime();
    
    //daemon = make_unique<FileDaemon>(&this->mesh, "TestObj.obj");
    mesh = make_shared<FingerMesh>();
}

//--------------------------------------------------------------
void ofApp :: update() { }

//--------------------------------------------------------------
void ofApp :: draw() {
	static int x = 0;
    
    if (statsEnabled) {
        //if (daemon && daemon->isFileReady()) {
            const int numPolys = mesh ? accumulate(mesh->begin(), mesh->end(), 0, [](int acc, ofMesh mesh) { return acc + mesh.getIndices().size() - 6 ;}) : 0;
            string strPolys = "Num. Polys in scene: " + to_string(numPolys) + "\n";
            strPolys += "FPS: " + to_string(ofGetFrameRate()) + "\n";
            strPolys += "GPU load: ";
            strPolys += to_string(lastFrameTime * .006f) + " %";
            
            ofDrawBitmapStringHighlight(strPolys, 10, 20);
        //}
        //else {
        //    ofDrawBitmapStringHighlight("No model currently loaded.", 10, 20);
        //}
    }
    
    ofPushMatrix();
    ofTranslate(512 + float(10) * sinf(x / float(30.0)), 484);
    ofScale(0.7f * ofGetWindowHeight() / mesh->maxElement, 0.7f * ofGetWindowHeight() / mesh->maxElement);
    //ofScale(35.0f, 35.0f);
    ofRotate(mouseY * (1.0/6.0f), 1, 0, 0);
    ofRotate((mouseX - 512) * (1.0/6.0f), 0, 1, 0);
    //ofRotate(x * 0.1f, 0, 0, 1);
    const uint64_t timeBegin = ofGetSystemTimeMicros();
    
    // We have to first check if the mesh is ready before rendering,
    //  or we might stumble upon cases where this thread is acessing
    //  an incomplete mesh object, therefore crashing the program.
    //if (daemon && daemon->isFileReady()){
        if (mesh) {
            shader.begin();
            shader.setUniform3f("lightPosition", lightPos.x, lightPos.y, lightPos.z);
            shader.setUniform1f("ambient", .1f);
            shader.setUniform1f("diffuse", .9f);
            shader.setUniform1f("noisePower", 0.3f);
            shader.setUniformTexture("palette", palette.getTexture(), 0);
            shader.setUniform1f("time", (ofGetSystemTime() % 2048) / 512.0f);
            
            // Great for debugging!
            //ofIcoSpherePrimitive(10.0, 0).draw();
            
            const float phase = M_PI * (ofGetSystemTime() % 2048) / 1024.0f;
            const float offset = M_PI * 0.15f;
            
            const auto timeElapsed = ofGetSystemTime() - mesh->creationTime;
            
            mesh->draw([&shader = shader, phase, offset, timeElapsed] (int x) {
                const float wave = (1.0f + 3.0f * powf((1.0f + cosf(phase - offset * x)) * 0.5f, 1.5)) * (1.0  + tanh((timeElapsed * .0005f) - (x + 1) * 1.5f)) * 0.5;
                shader.setUniform1f("offset", wave);
            }, timeElapsed / 500);
            
            shader.end();
        }
    //}
    //glFlush();
    lastFrameTime = ofGetSystemTimeMicros() - timeBegin;
    ofPopMatrix();
    
	x++;
}

//--------------------------------------------------------------
void ofApp :: keyPressed(int key) {
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
void ofApp :: keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp :: mouseMoved(int x, int y ) {
    //light.setPosition(x, y, 300);
    
    lightPos.x = x;
    lightPos.y = y;
    lightPos.z = -30;
    
    //mouseX = x;
    //mouseY = y;
}

//--------------------------------------------------------------
void ofApp :: mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp :: mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp :: mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp :: mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp :: mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp :: windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp :: gotMessage(ofMessage msg) {
    
}

//--------------------------------------------------------------
void ofApp :: dragEvent(ofDragInfo dragInfo) { 

}
