/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Developer
 *
 * Created on October 18, 2016, 9:12 PM
 */

#include <cstdlib>
//#include "../IO/IORpi.h"
#include <thread>
#include <functional>
#include <iostream>
#include "CameraThread.h"

/*
 * 
 */
int main(int argc, char** argv) {
    auto cameraThreadObj = CameraThread::Instance();
    std::cout << "Starting program" << std::endl;
    cameraThreadObj->start();
    // Testing: Activate the camera to take an image.
//    CameraThread::Settings settings = cameraThreadObj.getSettings();
//    settings.state = CameraThread::Settings::IMAGE;
//    cameraThreadObj.setSettings(settings);
//    std::this_thread::yield();
    //cameraThreadObj.takePicture();
    cameraThreadObj->setVideoOn(true);
    std::this_thread::sleep_for(std::chrono::seconds(20));
    cameraThreadObj->setVideoOn(false);
    auto filenames = cameraThreadObj->getCompletedFilenames();
    cameraThreadObj->stop();
    for (auto&& filename : filenames) {
        std::cout << filename << std::endl;
    }
    return 0;
}
