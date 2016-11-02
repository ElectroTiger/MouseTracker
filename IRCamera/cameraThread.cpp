/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Function called from thread which opens the camera, records from it, and 

#include "cameraThread.h"
#include <raspicam/raspicam.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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

CameraThread::CameraThread() {

}

void CameraThread::operator()() {
    /* Check if the camera is attached. */

    /* Open a new file with the name given by the getCurrentTime() function. */
    std::ofstream videoFile;
    videoFile.open(getCurrentTime());
    /* Initialize the camera*/

    while (true) {

    }

}

CameraThread::~CameraThread() {

}

/// Set the camera mode to be optimized for either daytime vision or night vision.

void CameraThread::setMode(Mode mode) {
    // Only act if the mode actually changes.
    if (mode != this->mode) {

        // Change the setting and stop recording.
        settingsMutex.lock();
        this->mode = mode;
        CameraThread::stopRecording();

        // Act based on the input.
        switch (mode) {
            case DAY:
                raspicam.setFormat(raspicam::RASPICAM_FORMAT::RASPICAM_FORMAT_YUV420);
                break;
            case NIGHT:
                raspicam.setFormat(raspicam::RASPICAM_FORMAT::RASPICAM_FORMAT_GRAY);
                break;
        }

        // Start recording and release the lock.
        CameraThread::startRecording();
        settingsMutex.unlock();
    }
}
/// Set the camera resolution.

void CameraThread::setResolution(CameraThread::Resolution res) {
    // Only act if the mode actually changes.
    if (res != this->res) {

        // Change the setting and stop recording.
        settingsMutex.lock();
        this->res = res;
        CameraThread::stopRecording();

        // Act based on the input.
        switch (res) {
            case r240P:
                raspicam.setWidth(320);
                raspicam.setHeight(240);
                break;
            case r480P:
                raspicam.setWidth(640);
                raspicam.setHeight(480);
                break;
            case r960P:
                raspicam.setWidth(1280);
                raspicam.setHeight(960);
                break;
        }

        // Start recording and release the lock.
        CameraThread::startRecording();
        settingsMutex.unlock();
    }
}
/// Set whether the camera is on.

void CameraThread::setIsOn(bool b) {
    if (this->isOn != b) {
        settingsMutex.lock();
        this->isOn = b;
        if (b == true) {
            startRecording();
        } else {
            stopRecording();
        }
        settingsMutex.unlock();
    }
}
/// Get whether the camera is on.

bool CameraThread::getIsOn() {
    settingsMutex.lock();
    bool returnVal = isOn;
    settingsMutex.unlock();
    return returnVal;
}
/// Get a the filenames of the videofiles that have completed recording.

std::vector<std::string> CameraThread::getCompletedFilenames() {
    return completedFileNames;
}