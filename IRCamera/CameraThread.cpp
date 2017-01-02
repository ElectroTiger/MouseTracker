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
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

std::string CameraThread::getCurrentTime(void) {
    auto currentTime = std::chrono::system_clock::now();
    auto currentTime_time_t = std::chrono::system_clock::to_time_t(currentTime);
    auto currentTime_c = std::localtime(&currentTime_time_t);
    // Use ISO 8601 format.
    std::ostringstream fileName;
    fileName
            << std::setw(4) << currentTime_c->tm_year + 1900 << '-'
            << std::setw(2) << currentTime_c->tm_mon + 1 << '-'
            << std::setw(2) << currentTime_c->tm_mday << 'T'
            << std::setw(2) << currentTime_c->tm_hour << ':'
            << std::setw(2) << currentTime_c->tm_min << ':'
            << std::setw(2) << currentTime_c->tm_sec + 1
            ;
    return fileName.str();

}

CameraThread::CameraThread() :
settings (defaultSettings), terminateNow (ATOMIC_FLAG_INIT) {
    
}

CameraThread::~CameraThread() {
    terminate();
}

void CameraThread::start() {
    
}

void CameraThread::terminate() {
    //std::lock_guard<std::mutex> lock(settingsMutex);
    terminateNow.test_and_set(std::memory_order_release);
    //settingscv.notify_one();
}

void CameraThread::operator()() {
    /* Check if the camera is attached. */

    /* Initialize the camera*/
    
    cv::VideoWriter writer;
    cv::Mat frame;
    std::string filename;
    enum ThreadState {OFF, OPENVIDEO, VIDEO, CLOSEVIDEO, CAMERA, TERMINATE} state = OFF;
    bool terminate = false;
    while (!terminate) {
        // Unique lock is necessary because std::condition_variable requires it as input.
        std::unique_lock<std::mutex> lock(settingsMutex); 
        switch(state) {
            case OFF: {
                // Release the camera and writer in case we arrived here due to an exception.
                if (camera.isOpened()) {
                    camera.release();
                }
                if (writer.isOpened()) {
                    writer.release();
                }
                if(terminateNow.test_and_set(std::memory_order_consume)) {
                    state = TERMINATE;
                    continue;
                }
                // Wait on a condition variable.
                settingscv.wait(lock);
                switch(settings.state) {
                    case Settings::OFF:
                        state = OFF;
                        break;
                    case Settings::VIDEO:
                        state = OPENVIDEO;
                        break;
                    case Settings::IMAGE:
                        state = CAMERA;
                        break;
                }
                break;
            }
                
            case OPENVIDEO: {
                // Open the camera and throw an exception on failure.
                if(!camera.open()) {
                    throw std::runtime_error("Camera could not be opened.");
                    state = OFF;
                    continue;
                }
                
                // Prepare settings for the video writer.
                filename = getCurrentTime() + ".avi"; // Filename of video output.
                int codec = CV_FOURCC('M', 'J', 'P', 'G'); // Codec
                double fps = 24.0;
                cv::Size size((int) camera.get(CV_CAP_PROP_FRAME_WIDTH),   
                  (int) camera.get(CV_CAP_PROP_FRAME_HEIGHT));
                
                // Open the video writer and throw an exception on failure.
                if(!writer.open(filename, codec, fps, size, settings.isColor)) {
                    throw std::runtime_error("Video file could not be opened.");
                    state = OFF;
                    continue;
                }
                
                state = VIDEO;
                break;
            }
                
            case VIDEO: {

                // Stop capturing video if told to.
                if(terminateNow.test_and_set(std::memory_order_consume) || settings.state != Settings::VIDEO) {
                    state = CLOSEVIDEO;
                }
                
                // Acquire a frame and write it.
                if(!camera.grab()) {
                    throw std::runtime_error("Frame grab failed. Writing captured video.");
                    state = CLOSEVIDEO;
                    continue;
                }
                camera.retrieve(frame);
                writer.write(frame);
                
                break;
            }
                
            case CLOSEVIDEO: {
                camera.release();
                writer.release();
                completedFileNames.push_back(filename);
                state = OFF;
                break;
            }
                
            case CAMERA: {
                // Open the camera
                if(!camera.open()) {
                    throw std::runtime_error("Camera could not be opened.");
                    state = OFF;
                    continue;
                }
                
                // Grab the frame.
                if(!camera.grab()) {
                    throw std::runtime_error("Frame grab failed. Writing captured video.");
                    state = OFF;
                    continue;
                }
                camera.retrieve(frame);
                
                // Write the image.
                filename = getCurrentTime() + ".png";
                try {
                imwrite(filename, frame); // This may throw an exception.
                } catch(std::exception e) {
                    std::cerr << e.what() << std::endl;
                    throw;
                }
                completedFileNames.push_back(filename);
                
                // Release the camera.
                camera.release();
                state = OFF;
                break;
            }
            
            case TERMINATE: {
                terminate = true;
                break;
            }
        }
    }
}

CameraThread::Settings CameraThread::getSettings() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return settings;
}

void CameraThread::setSettings(Settings newSettings) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    settings = newSettings;
    settingscv.notify_one();
}

std::vector<std::string> CameraThread::getCompletedFilenames() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return completedFileNames;
}