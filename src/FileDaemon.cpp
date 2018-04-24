#include "FileDaemon.h"

FileDaemon :: FileDaemon(shared_ptr<FingerMesh> *ref, string fileName) : meshRef(ref), fileName(fileName) {
    daemonThread = std::thread(&FileDaemon::daemon, this);
    daemonThread.detach();
}

FileDaemon :: ~FileDaemon() {
    // This assures we can only fiddle with "isDestroying"
    //  while the daemon() is sleeping.
    lock_guard<mutex> lock(mut);
    isDestroying = true;
}

bool FileDaemon :: isFileReady() {
    return state == FILE_LOADED ? true : false;
}

void FileDaemon :: daemon() {
    // I have a problem: what if the object is destroyed
    //  as *this* comparison is running? EXC_BAD_ACCESS.
    //while (isDestroying == false) {
    
    // That's why this piece of code is needed, though
    //  admittedly more convoluted.
    while (1) {
        {
            lock_guard<mutex> lock(mut);
            if (isDestroying == true) {
                return;
            }
            switch (state) {
                case FILE_NONE: {
                    ifstream file(fileName);
                    if (file.good()) {
                        state = FILE_LOADING;
                        cout << "File " << fileName << " found! Loading...";
                        *meshRef = make_shared<FingerMesh>(fileName);
                        state = FILE_LOADED;
                        cout << " loaded." << endl;
                    }
                }  break;
                case FILE_LOADING: {
                }  break;
                case FILE_LOADED: {
                    ifstream file(fileName);
                    if (!file.good()) {
                        state = FILE_NONE;
                        meshRef->reset();
                        cout << "File " << fileName << " deleted!" << endl;
                    }
                }  break;
            }
        }
        // We don't want the thread to hog up an entire CPU
        //  core, so we only check for the file 5x a second.
        ofSleepMillis(200);
    }
}
