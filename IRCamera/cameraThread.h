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
#include <raspicam/raspicam.h>
#include <thread>
#include <mutex>
#include <fstream>

class CameraThread {
public:
    CameraThread();
    virtual ~CameraThread();
    // Functor operator allows class to be used where void function() would be used.
    void operator()(void);
    
// Accessors to change camera settings
public:
    enum Mode {DAY, NIGHT};
    /// Set the camera mode to be optimized for either daytime vision or night vision.
    void setMode(Mode mode);
    enum Resolution {r240P, r480P, r960P};
    /// Set the camera resolution.
    void setResolution(Resolution res);
    /// Set whether the camera is on.
    void setIsOn(bool b);
    /// Get whether the camera is on.
    bool getIsOn();
private:
    raspicam::RaspiCam raspicam;
    std::ofstream ofstream;
    // Camera settings which may be accessed from the outside.
private:
    std::mutex settingsMutex;
    raspicam::RASPICAM_FORMAT format;
    uint16_t width;
    uint16_t height;
    // Helper functions
private:
    static std::string getCurrentTime();
    
    
    
};



#endif /* CAMERATHREAD_H */

