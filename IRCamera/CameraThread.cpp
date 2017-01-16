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

// Set the instance pointer to nullptr to define it.
CameraThread* CameraThread::m_pInstance = nullptr;

std::string CameraThread::getCurrentTime(void) {
    auto currentTime = std::chrono::system_clock::now();
    auto currentTime_time_t = std::chrono::system_clock::to_time_t(currentTime);
    auto currentTime_c = std::localtime(&currentTime_time_t);
    // Use ISO 8601 format.
    std::ostringstream fileName;
    fileName
            << std::setw(4) << currentTime_c->tm_year + 1900 << '-'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_mon + 1 << '-'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_mday << 'T'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_hour << '.'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_min << '.'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_sec + 1
            ;
    return fileName.str();

}

CameraThread* CameraThread::Instance() {
    if (!m_pInstance) {
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
settings(defaultSettings), videoOn(false), numPicturesToTake(0), terminateNow(false) {

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

void CameraThread::onData(omxcam_buffer_t buffer) {
    auto obj = CameraThread::Instance();
    // Write the data to the file output.
    obj->file.write(reinterpret_cast<char*> (buffer.data), buffer.length);
    // Terminate of failure condition is detected.
    if (obj->file.fail()) {
        obj->terminateNow = true;
    }
    if (obj->terminateNow || !(obj->videoOn)) {
        if (omxcam_video_stop()) {
            omxcam_perror();
            throw std::runtime_error(omxcam_strerror(omxcam_last_error()));
        }
    }
}

void CameraThread::threadFunc() {
    /* Check if the camera is attached. */

    /* Initialize the camera*/
    omxcam_video_settings_t settings;
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
                std::clog << "CameraThread.cpp: in VIDEO";
                omxcam_video_init(&settings);
                filename = getCurrentTime() + ".h264"; // Filename of video output.
                // std::function<void(omxcam_buffer_t)> cb = std::bind(&CameraThread::onData, this, std::placeholders::_1);
                settings.on_data = Instance()->onData;
                settings.camera.width = 1920;
                settings.camera.height = 1080;
                settings.camera.framerate = 30;

                file.open(filename);
                if (file.fail()) {
                    throw std::runtime_error("File could not be opened.");
                    state = OFF;
                    continue;
                }
                // Start the camera, which blocks this thread.
                omxcam_video_start(&settings, 1000 * 60 * 60);
                // Capture is terminated in the following lines.
                file.close();
                completedFileNames.push_back(filename);
                state = OFF;
                break;
            }

            case CAMERA:
            {
                //                // Open the camera
                //                if (!camera.open()) {
                //                    throw std::runtime_error("Camera could not be opened.");
                //                    state = OFF;
                //                    continue;
                //                }
                //
                //                // Grab the frame.
                //                if (!camera.grab()) {
                //                    throw std::runtime_error("Frame grab failed. Writing captured video.");
                //                    state = OFF;
                //                    continue;
                //                }
                //                camera.retrieve(frame);
                //                // Convert the frame to RGB color since it seems to be taken in BGR color.
                //                cv::Mat frameTemp = frame;
                //                cv::cvtColor(frameTemp, frame, CV_BGR2RGB);
                //
                //                writePictureFrame(frame);
                //                completedFileNames.push_back(filename);
                //
                //                // Release the camera.
                //                camera.release();
                //                numPicturesToTake--;
                //                state = OFF;
                //                break;
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