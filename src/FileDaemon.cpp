#include "FileDaemon.h"

FileDaemon :: FileDaemon(shared_ptr<FingerMesh> *ref, string fileName) : meshRef(ref), fileName(fileName) {
    daemonThread = std::thread(&FileDaemon::daemon, this);
    daemonThread.detach();
}

FileDaemon :: ~FileDaemon() {
    isDestroying = true;
    // Sometimes the thread may destroy immediately after
    //  the previous instruction, in which case we'll crash
    //  upon exit when joining the dead thread. Because of
    //  this, we have to first check for joinability.
    if (daemonThread.joinable()) {
        daemonThread.join();
    }
}

bool FileDaemon :: isFileReady() {
    mut.lock();
    mut.unlock();
    return state == FILE_LOADED ? true : false;
}

void FileDaemon :: daemon() {
    while (isDestroying == false) {
        switch (state) {
            case FILE_NONE: {
                ifstream file(fileName);
                if (file.good()) {
                    state = FILE_LOADING;
                    cout << "File " << fileName << " found! Loading...";
                    mut.lock();
                    *meshRef = make_shared<FingerMesh>(fileName);
                    state = FILE_LOADED;
                    mut.unlock();
                    cout << " loaded." << endl;
                }
            }  break;
            case FILE_LOADING: {
            }  break;
            case FILE_LOADED: {
                ifstream file(fileName);
                if (!file.good()) {
                    mut.lock();
                    state = FILE_NONE;
                    meshRef->reset();
                    mut.unlock();
                    cout << "File " << fileName << " deleted!" << endl;
                }
            }  break;
        }
    
        // We don't want the thread to hog up an entire CPU
        //  core, so we only check for the file 5x a second.
        ofSleepMillis(200);
    }
}
