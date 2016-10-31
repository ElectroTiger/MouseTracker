/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Function called from thread which opens the camera, records from it, and 

#include "cameraThread.h"
#include "raspicam/raspicam.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>

static std::string getCurrentTime(void) {
    auto currentTime = std::chrono::system_clock::now();
    auto currentTime_c = std::localtime(std::chrono::system_clock::to_time_t(currentTime));
    // Use ISO 8601 format.
    std::string fileName;
    fileName
            << std::setw(4) << currentTime_c->tm_year + 1900 << '-'
            << std::setw(2) << currentTime_c->tm_mon + 1 << '-'
            << std::setw(2) << currentTime_c->tm_mday << 'T'
            << std::setw(2) << currentTime_c->tm_hour << ':'
            << std::setw(2) << currentTime_c->tm_min << ':'
            << std::setw(2) << currentTime_c->tm_sec + 1
            ;
    return fileName;
    
}

CameraThread::CameraThread() {
    
}

void CameraThread::operator ()() {
    /* Check if the camera is attached. */
    
    /* Open a new file. */
    ofstream videoFile;
    videoFile.open(getTimeString());
    /* Initialize the camera*/
    
    while(true) {
        
    }
    
}
CameraThread::~CameraThread() {

}
