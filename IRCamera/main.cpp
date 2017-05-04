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
#include "Utilities.h"
#include <string>
#include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;

/*
 * 
 */
int main(int argc, char** argv) {

    const fs::path workingDirectory {"."};
    if (fs::exists(workingDirectory) && !fs::is_directory(workingDirectory)) {
        fs::remove(workingDirectory);
    }
    
    if(!fs::exists(workingDirectory)) {
        fs::create_directory(workingDirectory);
    }

    auto cameraThreadObj = CameraThread::Instance();
    cameraThreadObj->set_directory(workingDirectory.c_str());
    std::cout << "Starting program" << std::endl;
    cameraThreadObj->start();

    //    cameraThreadObj->setVideoOn(true);
    //    std::this_thread::sleep_for(std::chrono::seconds(60));
    //    cameraThreadObj->setVideoOn(false);
    //    cameraThreadObj->takePicture();
    //    cameraThreadObj->stop();
    //    auto filenames = cameraThreadObj->getCompletedFilenames();
    //    for (auto&& filename : filenames) {
    //        std::cout << filename << std::endl;
    //    }

    // Start video capture.
    if (!cameraThreadObj->getVideoOn()) {
        cameraThreadObj->setVideoOn(true);
    }

    // Check for the presence of .h264 files in the working directory and convert to .mp4 file if they exist.
    fs::recursive_directory_iterator it(workingDirectory);
    fs::recursive_directory_iterator endit;
    const std::string ext = ".h264";
    while (it != endit) {
        if (fs::is_regular_file(*it) && it->path().extension() == ext) {
            std::clog << "Existing .h264 " << (*it).path().generic_string() << " found, converting to .mp4" << std::endl;
            Utilities::h264_to_avi(*it, cameraThreadObj->getSettings().fps);
            fs::remove(*it);
        }
        it++;
    }

    for (int i = 0; i < 1; i++) {
        // Wait for video capture to complete.
        cameraThreadObj->waitForCompletedFile();
        // Start video capture again. 
        if (!cameraThreadObj->getVideoOn()) {
            cameraThreadObj->setVideoOn(true);
        }
        // Convert the completed file.
        std::string filename = cameraThreadObj->popCompletedFilename();
        std::clog << "Video " << filename << " complete, converting to .mp4" << std::endl;
        fs::path file = fs::path(filename);
        Utilities::h264_to_avi(file, cameraThreadObj->getSettings().fps);
        // Delete the intermediate .h264 file.
        fs::remove(file);
    }
    
    // Terminate the running thread.
    cameraThreadObj->stop();
    return 0;
}
