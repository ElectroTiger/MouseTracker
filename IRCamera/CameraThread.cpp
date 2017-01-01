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

void CameraThread::startRecording() {

}

void CameraThread::stopRecording() {

}

CameraThread::CameraThread() :
settings (defaultSettings) {

}

CameraThread::~CameraThread() {

}

void CameraThread::operator()() {
    /* Check if the camera is attached. */

    /* Initialize the camera*/
    
    enum ThreadState {OFF, OPENVIDEO, VIDEO, CLOSEVIDEO, CAMERA} state = OFF;
    while (true) {
        std::lock_guard<std::mutex> lock(settingsMutex);
        
        cv::VideoWriter writer;
        cv::Mat frame;
        switch(state) {
            case OFF: {
                // Release the camera and writer in case we arrived here due to an exception.
                if (camera.isOpened()) {
                    camera.release();
                }
                if (writer.isOpened()) {
                    writer.release();
                }
                // Wait on a condition variable.
                
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
                std::string filename = getCurrentTime() + ".avi"; // Filename of video output.
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
                // Loop to grab frames and write them.
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
                std::string filename = getCurrentTime() + ".avi";
                imwrite(filename, frame); // This may throw an exception.
                
                // Release the camera.
                camera.release();
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

void CameraThread::setSettings(Settings newSettings) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    // Terminate the current recording or capture if it is still running.
    if (settings.state != CameraThread::Settings::OFF) {
        
    }
}

std::vector<std::string> CameraThread::getCompletedFilenames() {
    return completedFileNames;
}