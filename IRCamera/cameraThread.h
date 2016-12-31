/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cameraThread.h
 * Author: Developer
 *
 * Created on October 18, 2016, 9:35 PM
 */

#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <raspicam/raspicam_cv.h>

class CameraThread {
public:
    CameraThread();
    CameraThread(const CameraThread&) = delete; // No copies allowed.
    //CameraThread& operator=(const CameraThread&) = delete; // No self assignment.
    
    virtual ~CameraThread();
    // Functor operator allows class to be used where void function() would be used.
    // This method is called when CameraThread is passed as an argument to something accepting a function pointer.
    void operator()();
    
// Accessors to change camera settings
    
public: 
    // Structure which encapsulates camera settings.
    struct Settings {
        uint16_t width;
        uint16_t height;
        enum State {OFF, VIDEO, IMAGE} state;
    };
    
public:
    Settings getSettings();
    void setSettings(Settings settings);
    std::vector<std::string> getCompletedFilenames();
    
private:
    std::ofstream ofstream;
    raspicam::RaspiCam_Cv camera;
    
    // Camera settings which may be accessed from the outside.
private:
    mutable std::mutex settingsMutex;
    std::vector<std::string> completedFileNames;
    Settings settings;
    
    // Helper functions
private:
    static std::string getCurrentTime();
    void startRecording();
    void stopRecording();
    
    
    
};



#endif /* CAMERATHREAD_H */

