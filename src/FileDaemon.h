#pragma once

#include "ofMain.h"
#include "FingerMesh.h"
#include <future>

class FileDaemon {
protected:
    enum {
        FILE_NONE,
        FILE_LOADING,
        FILE_LOADED
    } state = FILE_NONE;
    
    shared_ptr<FingerMesh> *meshRef;
    thread daemonThread;
    bool isDestroying = false;
    string fileName;
    void daemon();
    
public:
    mutex mut, meshMut;
    FileDaemon(shared_ptr<FingerMesh> *ref, string fileName);
    ~FileDaemon();
    bool isFileReady();
};
