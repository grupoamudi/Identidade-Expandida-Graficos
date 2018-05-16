#include "ofMain.h"
#include <fstream>

// Windows apparently doesn't ship with M_PI, so we
//  have to define it ourselves.
#ifndef M_PI
	#define M_PI 3.141592f
#endif

#pragma once
class FingerMesh : public vector<ofVboMesh> {
public:
    uint64_t creationTime;
    float maxElement;
	// Initializes with a default, oval mesh for test purposes.
    FingerMesh();
	// Loads from a file, which should be default behavior.
	FingerMesh(string filePath);
    
	void setHeight(const size_t index, const float height);
	void setHeight(const vector<float> heights);
    
    void draw() const;
    void draw(function<void(int)> fun, size_t segments) const;
    void drawWithNormalColors() const;
    
    // Since we don't ever use the new() operator, the default
    //  destructor is enough for us.
    
protected:
    void calculateMaxElement();
};

