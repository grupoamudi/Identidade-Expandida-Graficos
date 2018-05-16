#include"ofMain.h"

#pragma once
class FingerMesh : public std::vector<ofMesh> {
public:
	// We're explicitly deleting the default constructor
	//  because we only want FingerMesh to be initializable
	//  with a vertex list.
	FingerMesh() = delete;
	FingerMesh(const vector<float> vertList);

	const void draw();
	void setHeight(const int curve, const float height);
	void setHeight(const iterator begin, const iterator end);
	void setOffset(const int curve, const float height);
	void setOffset(const iterator begin, const iterator end);


	~FingerMesh();
};

