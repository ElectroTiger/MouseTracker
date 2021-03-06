/* 
 * File:   cameraThread.h
 * Author: Weimen Li
 *
 * Created on October 18, 2016, 9:35 PM
 */

#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <condition_variable>
#include <atomic>
#include <string>
#include "../raspberrypi_omxcam/include/omxcam.h"

class CameraThread {
public:
    static CameraThread* Instance();
    // Start the camerathread.
    void start();
    // Stop the cameraThread.
    void stop();
    // Set the camera thread directory
    // TODO: Replace string implementation with a library implementation.
    bool set_directory(std::string directory_path);
private:
    CameraThread();
    CameraThread(const CameraThread&) = delete; // No copies allowed.
    // Functor operator allows class to be used where void function() would be used.
    // This method is called when CameraThread is passed as an argument to something accepting a function pointer.
    void threadFunc();
    
    static CameraThread* m_pInstance;
    
public: 
    // Structure which encapsulates camera settings.
    struct Settings {
        /// Capture width in pixels.
        uint16_t width;
        /// Capture height in pixels
        uint16_t height;
        /// Frames Per Second (FPS) to capture video at.
        float fps;
        /// Duration to capture video for, in seconds.
        int duration;
        
        Settings() : width(640U), height(480U), 
            fps(25.0), duration(60) {}
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
    /// Take a picture with the camera.
    void takePicture();
    /// Sleeps the calling thread until cameraThread has produced a file.
    void waitForCompletedFile();
    /// Get a copy of the vector containing the completed file names.
    std::vector<std::string> getCompletedFilenames();
    /// Remove the most recent completed file from the stack.
    /// @returns empty string if the queue is empty.
    std::string popCompletedFilename();

    
    // Camera settings which may be accessed from the outside.
private:
    mutable std::mutex settingsMutex;
    std::condition_variable settingscv, picture_taken_cv;
    bool settingsChanged;
    Settings settings;
    bool videoOn;
    unsigned int numPicturesToTake;
    std::vector<std::string> completedFileNames;
    std::atomic<bool> terminateNow;
    std::thread thread;
    std::string file_directory;
    
    // Helper functions
private:
    static void onDataVideo(omxcam_buffer_t buffer);
    static void onDataImage(omxcam_buffer_t buffer);
    static void stopCamera();
    std::ofstream file;
};
#endif /* CAMERATHREAD_H */

