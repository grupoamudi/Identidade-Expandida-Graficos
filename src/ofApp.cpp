#include "ofApp.h"
//#define DEBUG_MODE

//--------------------------------------------------------------
void ofApp :: setup() {
    ofSetupScreenPerspective(ofGetWidth(), ofGetHeight(), 16.0f, 0.1f, 2000.0f);
	ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofDisableArbTex();
    
    // Amazingly, OF doesn't turn face culling on by default.
    // Realizing this unearthed a lot of problems with my
    //  custom geometry code.
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    
    palette = ofImage("Palette.png");
    shader.load("FingerShader.vert", "FingerShader.frag");
    startupTime = ofGetSystemTime();
    
#ifdef DEBUG_MODE
    mesh = make_shared<FingerMesh>();
#else
    daemon = make_unique<FileDaemon>(&this->mesh, "currentFingerprint.obj");
#endif
}

//--------------------------------------------------------------
void ofApp :: update() { }

//--------------------------------------------------------------
void ofApp :: draw() {
	static int x = 0;
    
    if (statsEnabled) {
#ifndef DEBUG_MODE
        if (daemon && daemon->isFileReady()) {
#endif
            const int numPolys = mesh ? accumulate(mesh->begin(), mesh->end(), 0, [](int acc, ofMesh mesh) { return acc + mesh.getIndices().size() / 3 ;}) : 0;
            string strPolys = "Num. Polys in scene: " + to_string(numPolys) + "\n";
            strPolys += "FPS: " + to_string(ofGetFrameRate()) + "\n";
            strPolys += "GPU load: ";
            strPolys += to_string(lastFrameTime * .006f) + " %";
            
            ofDrawBitmapStringHighlight(strPolys, 10, 20);
#ifndef DEBUG_MODE
        }
        else {
            ofDrawBitmapStringHighlight("No model currently loaded.", 10, 20);
        }
#endif
    }
    
    //ofRotate(x * 0.1f, 0, 0, 1);
    const uint64_t timeBegin = ofGetSystemTimeMicros();
    
    // We have to first check if the mesh is ready before rendering,
    //  or we might stumble upon cases where this thread is acessing
    //  an incomplete mesh object, therefore crashing the program.
#ifndef DEBUG_MODE
    if (daemon && daemon->isFileReady()){
#endif
        if (mesh) {
            ofPushMatrix();
            ofTranslate(ofGetWidth() * 0.5f /*+ float(10) * sinf(x / float(30.0))*/, ofGetHeight() * 0.2f, -1.8f * ofGetHeight());
            //ofTranslate(.0f, .0f, -ofGetHeight());
            
            ofRotate(180.0f, 0, 0, 1);
            ofRotate(-45.0f, 1, 0, 0);
            //ofRotate((mouseX - 512) * 0.3f, 0, 1, 0);
            //ofRotate((mouseY - 384) * 0.3f, 1, 0, 0);
            
            
            ofScale(1.5f * ofGetHeight() / mesh->maxElement, 1.5f * ofGetHeight() / mesh->maxElement, 0.1f * ofGetHeight());
            
            //ofScale(35.0f, 35.0f);
            
            shader.begin();
            shader.setUniform3f("lightPosition", lightPos.x, lightPos.y, lightPos.z);
            shader.setUniform1f("ambient", .1f);
            shader.setUniform1f("diffuse", .9f);
            shader.setUniform1f("noisePower", 0.3f);
            shader.setUniformTexture("palette", palette.getTexture(), 0);
            shader.setUniform1f("time", (ofGetSystemTime() % 4096) / 2048.0f);
            
            // Great for debugging!
            //ofIcoSpherePrimitive(10.0, 0).draw();
            
            const float phase = M_PI * (ofGetSystemTime() % 4096) / 2048.0f;
            const float offset = M_PI * 0.15f;
            
            const auto timeElapsed = ofGetSystemTime() - mesh->creationTime;
            
            mesh->draw(/*[&shader = shader, phase, offset, timeElapsed, &mesh = mesh] (int x) {
                const float wave = (1.0f + 3.0f * powf((1.0f + cosf(phase - offset * x)) * 0.5f, 1.5));
                const float height = (1.0  + tanh((timeElapsed * .0005f) - (x + 1) * 10.0f / mesh->size())) * 0.5;
                shader.setUniform1f("offset", wave * height);
                        }*/[](int a){}, timeElapsed * mesh->size() / 1000);
            
            shader.end();
            ofPopMatrix();
        }
#ifndef DEBUG_MODE
    }
#endif
    //glFlush();
    lastFrameTime = ofGetSystemTimeMicros() - timeBegin;
    
    
	x++;
}

//--------------------------------------------------------------
void ofApp :: keyPressed(int key) {
    if (key == 's') {
        statsEnabled ^= 1;
    }
    if (key == 'f') {
        /*dummyFile ^= 1;
        if (dummyFile) {
            ofstream dummy("TestFile.obj", ios::trunc);
            dummy << "v 0 0\nv 1 0\nv 1 1\nv 1 0\nf 0 1 2 3 0";
        }
        else {
            remove("TestFile.obj");
        }*/
        for(auto x = 0; x < mesh->size(); x++) {
            std::string saveString = "MeshDump " + to_string(x);
            saveString += ".ply";
            mesh->at(x).save(saveString);
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
