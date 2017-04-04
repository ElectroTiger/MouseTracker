/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Function called from thread which opens the camera, records from it, and 

#include "CameraThread.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <exception>
#include "Utilities.h"

// Set the instance pointer to nullptr to define it.
CameraThread* CameraThread::m_pInstance = nullptr;
//constexpr CameraThread::Settings defaultSettings;

CameraThread* CameraThread::Instance() {
    if (!std::is_null_pointer<decltype(m_pInstance)>::value) {
        m_pInstance = new CameraThread();
    }
    return m_pInstance;
}

void CameraThread::start() {
    setSettings(settings);
    thread = std::thread(&CameraThread::threadFunc, std::ref(*this));
}

void CameraThread::stop() {
    //std::lock_guard<std::mutex> lock(settingsMutex);
    terminateNow = true;
    //settingscv.notify_one();
    if (thread.joinable()) {
        thread.join();
    }
}

CameraThread::CameraThread() :
settings(), videoOn(false), numPicturesToTake(0), terminateNow(false) {

}

bool CameraThread::getVideoOn() {
    std::lock_guard<std::mutex> lock(settingsMutex); // May not be needed because videoOn is written by the thread.
    return videoOn;
}

void CameraThread::setVideoOn(bool isOn) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    videoOn = isOn;
    settingsChanged = true;
    settingscv.notify_one();
}

void CameraThread::takePicture() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    numPicturesToTake += 1;
    settingsChanged = true;
    settingscv.notify_one();
}

// Callback function used when a frame is received.

void CameraThread::onDataVideo(omxcam_buffer_t buffer) {
    auto obj = CameraThread::Instance();
    // Write the data to the file output.
    obj->file.write(reinterpret_cast<char*> (buffer.data), buffer.length);
    // Terminate of failure condition is detected.
    if (obj->file.fail()) {
        std::cerr << "Video frame write failure.";
        obj->terminateNow = true;
    }
    if (obj->terminateNow || !(obj->videoOn)) {
        if (omxcam_video_stop()) {
            omxcam_perror();
            throw std::runtime_error(omxcam_strerror(omxcam_last_error()));
        }
    }
}

// Callback function used when an image is received.
void CameraThread::onDataImage(omxcam_buffer_t buffer) {
    // Retrieve the instance of the camerathread singleton.
    auto obj = CameraThread::Instance();
    obj->file.write(reinterpret_cast<char*> (buffer.data), buffer.length);
    // Log if failure condition is detected.
    if (obj->file.fail()) {
        std::cerr << "Camera frame write failure.";
    }
    
}


void CameraThread::threadFunc() {
    /* Check if the camera is attached. */

    /* Initialize the camera*/
    omxcam_video_settings_t videoSettings;
    omxcam_still_settings_t stillSettings;
    std::string filename;

    enum ThreadState {
        OFF, VIDEO, CAMERA
    } state = OFF;
    bool terminate = false;

    while (!terminate) {
        switch (state) {
            case OFF:
            {
                std::unique_lock<std::mutex> lock(settingsMutex); // Unique lock is necessary because std::condition_variable requires it as input.
                if (terminateNow) {
                    terminate = true;
                    continue;
                }
                // Wait on a condition variable.
                while (!settingsChanged) {
                    settingscv.wait(lock);
                }
                settingsChanged = false;

                // Change the settings if need be.

                if (numPicturesToTake > 0) {
                    state = CAMERA;
                } else if (videoOn) {
                    state = VIDEO;
                } else {
                    state = OFF;
                }

                break;
            }

            case VIDEO:
            {
                // Open the camera and throw an exception on failure.
                std::clog << "CameraThread.cpp: in VIDEO" << std::endl;
                omxcam_video_init(&videoSettings);
                filename = Utilities::getCurrentTime() + ".h264"; // Filename of video output.
                // std::function<void(omxcam_buffer_t)> cb = std::bind(&CameraThread::onData, this, std::placeholders::_1);
                videoSettings.on_data = Instance()->onDataVideo;
                videoSettings.camera.width = 1920;
                videoSettings.camera.height = 1080;
                videoSettings.camera.framerate = 30;

                file.open(filename);
                if (file.fail()) {
                    throw std::runtime_error("File could not be opened.");
                    state = OFF;
                    continue;
                }
                // Start the camera, which blocks this thread.
                omxcam_video_start(&videoSettings, 1000 * 60 * 60);
                // Capture is terminated in the following lines.
                file.close();
                completedFileNames.push_back(filename);
                state = OFF;
                break;
            }

            case CAMERA:
            {
                std::clog << "CameraThread.cpp: in CAMERA" << std::endl;
                omxcam_still_init(&stillSettings);
                filename = Utilities::getCurrentTime() + ".jpeg"; // Filename of image output.
                stillSettings.on_data = Instance()->onDataImage;
                stillSettings.camera.width = 1920;
                stillSettings.camera.height = 1080;
                
                // Open a file to write.
                file.open(filename);
                if (file.fail()) {
                    throw std::runtime_error("File could not be opened.");
                    state = OFF;
                    continue;
                }
                omxcam_still_start(&stillSettings);
                file.close();
                completedFileNames.push_back(filename);
                state = OFF;
                break;
            }
        }
    }
}

CameraThread::Settings CameraThread::getSettings() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return settings;
}

void CameraThread::setSettings(Settings const &newSettings) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    settings = newSettings;
    fps_to_stdchrono = std::chrono::microseconds(static_cast<int32_t> (1000000 / settings.fps));
    settingsChanged = true;
    settingscv.notify_one();
}

std::vector<std::string> CameraThread::getCompletedFilenames() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return completedFileNames;
}