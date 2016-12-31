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
#include "CameraThread.h"

/*
 * 
 */
int main(int argc, char** argv) {
    CameraThread cameraThreadObj;
    cameraThreadObj();
    std::thread cameraThread(std::ref(cameraThreadObj));

    return 0;
}
