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

    //cameraThreadObj->setVideoOn(true);
    //std::this_thread::sleep_for(std::chrono::seconds(20));
    //cameraThreadObj->setVideoOn(false);
    cameraThreadObj->takePicture();
    cameraThreadObj->stop();
    auto filenames = cameraThreadObj->getCompletedFilenames();
    for (auto&& filename : filenames) {
        std::cout << filename << std::endl;
    }
    return 0;
}
