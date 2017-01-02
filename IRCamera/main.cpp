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
    CameraThread cameraThreadObj;
    std::thread cameraThread(std::ref(cameraThreadObj));
    std::cout << "Starting program" << std::endl;
    // Testing: Activate the camera to take an image.
    CameraThread::Settings settings = cameraThreadObj.getSettings();
    settings.state = CameraThread::Settings::IMAGE;
    cameraThreadObj.setSettings(settings);
    std::this_thread::yield();
    cameraThreadObj.terminate();
    cameraThread.join();
    auto filenames = cameraThreadObj.getCompletedFilenames();
    for (auto&& filename : filenames) {
        std::cout << filename << std::endl;
    }
    return 0;
}
