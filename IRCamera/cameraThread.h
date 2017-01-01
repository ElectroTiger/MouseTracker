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
    
    virtual ~CameraThread();
    // Functor operator allows class to be used where void function() would be used.
    // This method is called when CameraThread is passed as an argument to something accepting a function pointer.
    void operator()();
    
public: 
    // Structure which encapsulates camera settings.
    struct Settings {
        /// Capture width in pixels.
        uint16_t width;
        /// Capture height in pixels
        uint16_t height;
        /// Capture format as grayscale or color.
        bool isColor;
        /// Brightness between 0 and 255
        uint8_t brightness;
        /// Contrast between 0 and 255
        uint8_t contrast;
        /// Saturation between 0 and 255
        uint8_t saturation;
        /// Gain (iso) between 0 and 255.
        uint8_t gain;
        /// Shutter speed between 1 and 255, 0 for auto.
        uint8_t exposure;
        /// White Balance Red between 1 and 255, 0 for auto.
        uint8_t whiteBalanceRed;
        /// White Balance Blue between 1 and 255, 0 for auto.
        uint8_t whiteBalanceBlue;
        /// Camera capture state as off, video, or image.
        enum State {OFF, VIDEO, IMAGE} state;
    };
    
    /// Default settings used when an instance of CameraThread is created.
    static constexpr Settings defaultSettings{
            640U, 480U, 
            true, 
            127, 127, 127, 
            0, 0, 0, 
            Settings::OFF
    };

    
public:
    Settings getSettings();
    void setSettings(Settings settings);
    std::vector<std::string> getCompletedFilenames();
    
private:
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

