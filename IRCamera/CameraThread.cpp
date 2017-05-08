/* 
 * File:   cameraThread.h
 * Author: Weimen Li
 *
 * Created on October 18, 2016, 9:35 PM
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
#include <thread>
#include <boost/filesystem.hpp>
#include <stdexcept>
#include "Utilities.h"

// Set the instance pointer to nullptr to define it.
CameraThread* CameraThread::m_pInstance = nullptr;
//constexpr CameraThread::Settings defaultSettings;

CameraThread* CameraThread::Instance() {
    if (m_pInstance == nullptr) {
        m_pInstance = new CameraThread();
    }
    return m_pInstance;
}

void CameraThread::start() {
    setSettings(settings);
    thread = std::thread(&CameraThread::threadFunc, std::ref(*this));
    //    thread = std::thread(&CameraThread::threadFunc, this);
}

void CameraThread::stop() {
    {std::lock_guard<std::mutex> lock(settingsMutex);
    terminateNow = true;
    settingsChanged = true;
    stopCamera();
    settingscv.notify_one();
    }
    if (thread.joinable()) {
        thread.join();
    }
}

bool CameraThread::set_directory(std::string directory_path) {
    using namespace boost::filesystem;
        path p{directory_path};
        if (exists(p) && !is_directory(p)) {
            std::ostringstream ss;
            ss << directory_path << " exists, but is not a directory.";
            throw new std::invalid_argument(ss.str());
            return false;
        } else {
            if (!exists(p)) {
                create_directory(p);
            }
            std::lock_guard<std::mutex> lock(settingsMutex);
            file_directory = p.string();
            return true;
        }
//    }
}

CameraThread::CameraThread() :
settings(), videoOn(false), numPicturesToTake(0), terminateNow(false), file_directory("ircamera") {
    using namespace boost::filesystem;
    path p{file_directory};
    if (!exists(p)) {
        create_directory(p);
    }
}

bool CameraThread::getVideoOn() {
    std::lock_guard<std::mutex> lock(settingsMutex); // May not be needed because videoOn is written by the thread.
    return videoOn;
}

void CameraThread::setVideoOn(bool isOn) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    if (videoOn && !isOn) {
        stopCamera();
    }
    videoOn = isOn;
    settingsChanged = true;
    settingscv.notify_one();
}

void CameraThread::takePicture() {
    std::unique_lock<std::mutex> lock(settingsMutex);

    // Notify the camera thread to take a picture.
    numPicturesToTake += 1;
    settingsChanged = true;
    settingscv.notify_one();
    while (numPicturesToTake != 0) picture_taken_cv.wait(lock);

}

void CameraThread::waitForCompletedFile() {
    std::unique_lock<std::mutex> lock(settingsMutex);
    while (completedFileNames.empty()) picture_taken_cv.wait(lock);
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

void CameraThread::stopCamera() {
    std::clog << "Termination signal detected while omxcam active, stopping omxcam." << std::endl;
    if (omxcam_video_stop()) {
        omxcam_perror();
        omxcam_errno errorNum = omxcam_last_error();
        if (errorNum != omxcam_errno::OMXCAM_ERROR_CAMERA_STOPPING)
            throw std::runtime_error(omxcam_strerror(errorNum));
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
                // std::function<void(omxcam_buffer_t)> cb = std::bind(&CameraThread::onData, this, std::placeholders::_1);
                int duration_copy; // Copy of the duration to capture video.
                {
                    std::lock_guard<std::mutex> lock(settingsMutex);
                    filename = file_directory + '/' + Utilities::getCurrentTime() + ".h264"; // Filename of image output.
                    videoSettings.on_data = Instance()->onDataVideo;
                    videoSettings.camera.width = settings.width;
                    videoSettings.camera.height = settings.height;
                    videoSettings.camera.framerate = settings.fps;
                    duration_copy = settings.duration;
                }
                file.open(filename);
                if (file.fail()) {
                    throw std::runtime_error("CameraThread.cpp: File << " + filename + " could not be opened.");
                    std::lock_guard<std::mutex> lock(settingsMutex);
                    videoOn = false;
                    state = OFF;
                    continue;
                }
                // Start the camera, which blocks this thread.
                omxcam_video_start(&videoSettings, 1000 * duration_copy);
                // Capture is terminated in the following lines.
                file.close();
                {
                    std::lock_guard<std::mutex> lock(settingsMutex);
                    completedFileNames.push_back(filename);
                    videoOn = false;
                }
                // Inform any waiting thread that video capture is complete.
                picture_taken_cv.notify_one();
                state = OFF;
                break;
            }

            case CAMERA:
            {
                std::clog << "CameraThread.cpp: in CAMERA" << std::endl;
                omxcam_still_init(&stillSettings);

                {
                    std::lock_guard<std::mutex> lock(settingsMutex);
                    filename = file_directory + '/' + Utilities::getCurrentTime() + ".jpeg"; // Filename of image output.
                    stillSettings.on_data = Instance()->onDataImage;
                    stillSettings.camera.width = settings.width;
                    stillSettings.camera.height = settings.height;
                }

                // Open a file to write.
                file.open(filename);
                if (file.fail()) {
                    std::ostringstream ss;
                    ss << "CameraThread.cpp: " << filename << " could not be opened";
                    throw std::runtime_error(ss.str());
                    std::lock_guard<std::mutex> lock(settingsMutex);
                    numPicturesToTake = 0;
                    state = OFF;
                    continue;
                }
                omxcam_still_start(&stillSettings);
                file.close();
                completedFileNames.push_back(filename);
                {
                    std::lock_guard<std::mutex> lock(settingsMutex);
                    numPicturesToTake--;
                }
                picture_taken_cv.notify_one();
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

std::string CameraThread::popCompletedFilename() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    if (!completedFileNames.empty()) {
        std::string returnVal = completedFileNames.back();
        completedFileNames.pop_back();
        return returnVal;
    } else {
        return std::string();
    }
}
