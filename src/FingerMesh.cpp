#include "FingerMesh.h"

#define DEBUG_WITH_COLORS

// The default constructor makes concentric rings whose
//  shape vaguely resembles a fingerprint. The idea is
//  to have some kind of mesh ready so we can test the
//  animation and lighting functions.

FingerMesh::FingerMesh() {
    // We want to make concentric rings around the meshes,
    //  so we can do this in three steps:
    // 1. Generate discs + squash in X axis
    // 2. Generate normals
    // 2.1  and re-normalize
    // 3. Stitch faces together with indexes
    // and after that, we can finally render it.
    
    const float thickness = 1.0f;
    
    for (auto r = 2; r < 15; r += 3) {
        const int nSlices = 15 * r;
        const float step = 2 * M_PI / nSlices;
        
        vector<ofVec3f> vertices;
        vector<ofVec3f> normals;
        
        // First, we generate a slightly flattened ring
        //  in triangle-strip order, with normals.
        for (int s = 0; s != nSlices; s++) {
            const float sSin = sinf(step * s);
            const float sCos = 0.7 * cosf(step * s);
            const float r1 = r - thickness;
            const float r2 = r + thickness;
            vertices.push_back(ofVec3f(r1 * sCos, r1 * sSin, .0f));
            vertices.push_back(ofVec3f(r2 * sCos, r2 * sSin, .0f));
            normals.push_back(ofVec3f(-sCos, -sSin, -0.2f));
            normals.push_back(ofVec3f(sCos, sSin, -0.2f));
        }
        
        const unsigned int vertsPerRing = (unsigned int) vertices.size();
        
        // Second, we duplicate the lower vertices to
        //  make a heightened ring. We also get the
        //  normals ready for lighting.
        auto vs = vertices.size();
        vertices.resize(vs * 2);
        auto vb = vertices.begin();
        copy(vb, vb + vs, vb + vs);
        // Raising the second batch of vertices.
        for(auto x = vertsPerRing; x < vertices.size(); x++) {
            vertices[x].z = 1.0f;
        }
        
        auto ns = normals.size();
        normals.resize(ns * 2);
        auto nb = normals.begin();
        copy(nb, nb + ns, nb + ns);
        // Flipping the second set of normals in XY plane...
        for(auto x = vertsPerRing; x < normals.size(); x++) {
            normals[x].z = -normals[x].z;
        }
        // ... and then, normalizing all normals!
        for(ofVec3f &n : normals) {
            n.normalize();
        }
        
        // For a subtle color effect, we can optionally
        //  dye each vertex with a random color.
        /*
        vector<ofFloatColor> colors(vertices.size());
        for (ofFloatColor &c : colors) {
            c.set(ofRandom(0.75f, 1.0f));
        }*/
        
#ifdef DEBUG_WITH_COLORS
        vector<ofFloatColor> colors(vertices.size());
        for (auto x = 0; x < colors.size(); x++) {
            ofVec3f n = (normals[x] * 0.5f) + 0.5f;
            colors[x].set(n.x, n.y, n.z);
        }
#endif
        
        // Now, we just have to stitch the vertices
        //  together to make the rings, and later
        //  outer edges, using indices.
        vector<ofIndexType> indices((vertsPerRing + 2) * 4 + 6);
        
        // We stitch together all the four parts, using
        //  the technique of degenerated triangles to
        //  reduce the number of GL calls.
        // <idea> If we need to redude the number of calls
        //  even further, perhaps we can concatenate the
        //  meshes together using the ofMesh::append()
        //  method.
        
        const size_t lowerOffset = 0;
        const size_t upperOffset = vertsPerRing + 4;
        const size_t innerOffset = 2 * vertsPerRing + 8;
        const size_t outerOffset = 3 * vertsPerRing + 12;
        
        // Lower plane
        for (auto x = 0; x < vertsPerRing; x++) {
            indices[x] = x;
        }
        indices[vertsPerRing] = indices[0];
        indices[vertsPerRing + 1] = indices[1];
        
        // Upper plane
        for (auto x = 0; x < vertsPerRing; x++) {
            indices[upperOffset + x] = vertsPerRing + x;
        }
        indices[upperOffset + vertsPerRing] = indices[upperOffset];
        indices[upperOffset + vertsPerRing + 1] = indices[upperOffset + 1];
        
        // Inner ring
        for (auto x = 0; x < vertsPerRing / 2; x++) {
            indices[innerOffset + 2 * x] = 2 * x;
            indices[innerOffset + 2 * x + 1] = vertsPerRing + 2 * x;
        }
        indices[innerOffset + vertsPerRing] = indices[innerOffset];
        indices[innerOffset + vertsPerRing + 1] = indices[innerOffset + 1];
        
        // Outer ring
        for (auto x = 0; x < vertsPerRing / 2; x++) {
            indices[outerOffset + 2 * x] = 2 * x + 1;
            indices[outerOffset + 2 * x + 1] = vertsPerRing + 2 * x + 1;
        }
            
        indices[outerOffset + vertsPerRing] = indices[outerOffset];
        indices[outerOffset + vertsPerRing + 1] = indices[outerOffset + 1];
        
        // Now, to join the seams with degenerate triangles!
        indices[upperOffset - 2] = indices[upperOffset - 3];
        indices[upperOffset - 1] = indices[upperOffset];
        
        indices[innerOffset - 2] = indices[innerOffset - 3];
        indices[innerOffset - 1] = indices[innerOffset];
        
        indices[outerOffset - 2] = indices[outerOffset - 3];
        indices[outerOffset - 1] = indices[outerOffset];
        
        
        // And we're ready to build our mesh.
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        mesh.addVertices(vertices);
#ifdef DEBUG_WITH_COLORS
        mesh.addColors(colors);
#else
        mesh.addNormals(normals);
        //mesh.addColors(colors);
#endif
        mesh.addIndices(indices);
        
        this->push_back(mesh);
    }
}

FingerMesh::FingerMesh(string filePath) {
    // If the file doesn't exist yet, the constructor
    //  will throw an exception and the object won't
    //  be initialized. The caller needs to be aware
    //  of this.
    ifstream file(filePath);

    if (file.fail()) {
        cout << strerror(errno) << endl;
        throw "Something has gone wrong with the file!\n";
    }
    
    // First, we read the vertices and store them into
    //  a single vector, to later assembly the meshes.
    vector<ofVec3f> vertices;
    while (!file.eof()) {
        char line[256];
        do {
            file.getline(line, 256);
        } while((line[0] != 'v' || line[1] != ' ') && !file.eof());
		if (!file.eof()) {
			stringstream sLine(line);
			char c;
			float x, y;
			sLine >> c >> x >> y;
			vertices.push_back(ofVec3f(x, y, .0f));
		}
    }
    
    // We then rewind the file, now looking for
    //  polygons / faces, representing different
    //  finger curves.
    file.clear();
    file.seekg(0);

    
    while (!file.eof()) {
        char line[256];
        do {
            file.getline(line, 256);
        } while((line[0] != 'f' || line[1] != ' ') && !file.eof());
		if (!file.eof()) {
			stringstream sLine(line);
			char c;
			sLine >> c;
			// There's an additional challenge with faces:
			//  we don't know how many vertices make up
			//  one, so we have to read it until the end.
			vector<int> indices;
			do {
				int i;
				sLine >> i;
				indices.push_back(i);
			} while (!sLine.eof());
            
			// Now, we use ofPath's facilities to join
			//  the complex polygon and tesselate it
			//  for us.
			ofPath path;
			for (int i : indices) {
				path.lineTo(vertices[i].x, .0f, vertices[i].y);
			}
			path.tessellate();
			this->push_back(path.getTessellation());
		}
    }
    
    // Finally, we can find the center of all vertices
    //  and re-center the mesh group.
    ofVec3f accum(.0f, .0f, .0f);
    int nVerts = 0;
    for (ofMesh m : *this) {
        for (ofVec3f v : m.getVertices()) {
            accum += v;
            nVerts++;
        }
    }
    
    accum /= float(nVerts);
    
    for (ofMesh m : *this) {
        for (ofVec3f v : m.getVertices()) {
            v -= accum;
        }
    }
}

void FingerMesh::draw() {
    for (ofMesh mesh : *this) {
        //mesh.drawWireframe();
        mesh.draw();
        
        // The code below draws normals as colored lines,
        //  but it's rather slow. Use for debugging only.
//        auto verts = mesh.getVertices();
//        auto norms = mesh.getNormals();
//        vector<ofVec3f> normals(verts.size() * 2);
//        vector<ofFloatColor> normalColors(norms.size() * 2);
//        for (auto x = 0; x < verts.size(); x++) {
//            normals[2 * x] = verts[x];
//            ofVec3f vPlusN = verts[x] + norms[x];
//            normals[2 * x + 1] = vPlusN;
//            ofVec3f nNorm = (norms[x] * 0.5f) + 0.5f;
//            ofFloatColor nColor = ofFloatColor(nNorm.x, nNorm.y, nNorm.z, 1.0f);
//            normalColors[2 * x] = nColor;
//            normalColors[2 * x + 1] = normalColors[2 * x];
//        }
//
//        ofMesh normalView;
//        normalView.setMode(OF_PRIMITIVE_LINES);
//        normalView.addVertices(normals);
//        normalView.addColors(normalColors);
//
//        normalView.draw();
    }
}

void FingerMesh::setHeight(const size_t index, const float height) {
    auto &vertices = this->at(index).getVertices();
    for (auto x = vertices.size() / 2; x < vertices.size(); x++) {
        vertices[x].z = height;
    }
}

void FingerMesh::setHeight(const vector<float> heights) {
    for (auto y = 0; y < heights.size() && y < this->size(); y++) {
        vector<ofVec3f> vertices = this->at(y).getVertices();
        for (auto x = vertices.size() / 2; x < vertices.size(); x++) {
            vertices[x].z = heights[y];
        }
    }
}