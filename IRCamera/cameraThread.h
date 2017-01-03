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
#include <condition_variable>
#include <atomic>

class CameraThread {
public:
    CameraThread();
    CameraThread(const CameraThread&) = delete; // No copies allowed.
    virtual ~CameraThread();
private:
    // Functor operator allows class to be used where void function() would be used.
    // This method is called when CameraThread is passed as an argument to something accepting a function pointer.
    void threadFunc();
    
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
        /// Frames Per Second (FPS) to capture video at.
        float fps;
    };
    
    /// Default settings used when an instance of CameraThread is created.
    static constexpr Settings defaultSettings{
            640U, 480U, 
            true, 
            127, 127, 127, 127,
            0, 0, 0,
            19.0
    };
    
private:
    std::chrono::duration<int32_t, std::micro> fps_to_stdchrono;

    
public:
    Settings getSettings();
    void setSettings(Settings const &settings);
    /// Get whether the camera is currently taking a video.
    bool getVideoOn(void);
    /// Set whether the camera is currently taking a video.
    void setVideoOn(bool isOn);
    /// Take a picture with the camera. Works even during video capture.
    void takePicture();
    std::vector<std::string> getCompletedFilenames();
    
    // Camera settings which may be accessed from the outside.
private:
    mutable std::mutex settingsMutex;
    std::condition_variable settingscv;
    std::atomic<bool> settingsChanged;
    Settings settings;
    bool videoOn;
    unsigned int numPicturesToTake;
    std::vector<std::string> completedFileNames;
    std::atomic<bool> terminateNow;
    std::thread thread;
    
    // Helper functions
private:
    static std::string getCurrentTime();
    void writePictureFrame(cv::Mat const &frame);
    
    
};



#endif /* CAMERATHREAD_H */

