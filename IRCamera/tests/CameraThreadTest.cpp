/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CameraThreadTest.cpp
 * Author: Developer
 *
 * Created on December 31, 2016, 5:57 PM
 */

#include <stdlib.h>
#include <iostream>

#include <cstdlib>
#include <thread>
#include <functional>
#include "CameraThread.h"

/*
 * Simple C++ Test Suite
 */

void imgCaptureTest1() {
    std::cout << "CameraThreadTest imgCaptureTest1" << std::endl;
    CameraThread cameraThreadObj;
    cameraThreadObj();
    std::thread cameraThread(std::ref(cameraThreadObj));
    
    // Testing: Activate the camera to take an image.
    CameraThread::Settings settings = cameraThreadObj.getSettings();
    settings.state = CameraThread::Settings::IMAGE;
    cameraThreadObj.setSettings(settings);
}

void vidCaptureTest1() {
    std::cout << "CameraThreadTest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (CameraThreadTest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% CameraThreadTest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (CameraThreadTest)" << std::endl;
    imgCaptureTest1();
    std::cout << "%TEST_FINISHED% time=0 test1 (CameraThreadTest)" << std::endl;

//    std::cout << "%TEST_STARTED% test2 (CameraThreadTest)\n" << std::endl;
//    vidCaptureTest1();
//    std::cout << "%TEST_FINISHED% time=0 test2 (CameraThreadTest)" << std::endl;
//
//    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

