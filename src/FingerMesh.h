#include "ofMain.h"
#include <fstream>

// Windows apparently doesn't ship with M_PI, so we
//  have to define it ourselves.
#ifndef M_PI
	#define M_PI 3.141592f
#endif

#pragma once
class FingerMesh : public ofMesh {
public:
    uint64_t creationTime;
    float maxElement;
	// Initializes with a default, oval mesh for test purposes.
    FingerMesh();
	// Loads from a file, which should be default behavior.
	FingerMesh(string filePath);
    
    virtual ~FingerMesh() {};
    
	void setHeight(const size_t index, const float height);
	void setHeight(const vector<float> heights);
    void drawWithNormalColors() const;
    
protected:
    void calculateMaxElement();
    void flattenMeshVec(vector<ofMesh> const &meshVec);
};

