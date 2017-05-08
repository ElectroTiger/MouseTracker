/* 
 * File:   main.cpp
 * Author: Weimen Li
 *
 * Created on October 18, 2016, 9:12 PM
 */

#include <cstdlib>
#include <thread>
#include <functional>
#include <iostream>
#include "CameraThread.h"
#include "Utilities.h"
#include <string>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <chrono>
namespace fs = boost::filesystem;

/*
 * 
 */
int main(int argc, char** argv) {

    const fs::path workingDirectory{"/home/pi/DCIM/"};
    if (fs::exists(workingDirectory) && !fs::is_directory(workingDirectory)) {
        fs::remove(workingDirectory);
    }

    if (!fs::exists(workingDirectory)) {
        fs::create_directory(workingDirectory);
    }
    
    // Set up redirection of clog and cerr to a file output.
    std::string logName = Utilities::getCurrentTime() + ".log";
    std::ofstream log_fileStream(workingDirectory.generic_string() + logName);
    auto old_clog_rdbuf = std::clog.rdbuf();
    std::clog.rdbuf(log_fileStream.rdbuf());
    
    auto old_cerr_rdbuf = std::cerr.rdbuf();
    std::cerr.rdbuf(log_fileStream.rdbuf());
    

    auto cameraThreadObj = CameraThread::Instance();
    cameraThreadObj->set_directory(workingDirectory.c_str());
    std::clog << "Starting program" << std::endl;
    cameraThreadObj->start();

    // Start video capture.
    if (!cameraThreadObj->getVideoOn()) {
        cameraThreadObj->setVideoOn(true);
    }
    // Revert any backup files to their original form:
    {
        fs::recursive_directory_iterator it(workingDirectory);
        fs::recursive_directory_iterator endit;
        const std::string ext = ".bak";

        while (it != endit) {
            if (fs::is_regular_file(*it) && it->path().extension() == ext) {
                std::clog << "Existing .bak " << (*it).path().generic_string() << " found, converting to original." << std::endl;
                fs::path newPath = fs::change_extension(*it, "");
                if (fs::exists(newPath)) {
                    std::clog << "Warning, corrupt original found, removing." << std::endl;
                    fs::remove(newPath);
                }
                fs::copy(*it, newPath);
                fs::remove(*it);
                std::clog << "Existing .bak removal complete." << std::endl;
            }
            it++;
        }
    }

    bool isFirst = true;
    fs::path firstFilePath;

    auto time_start = std::chrono::steady_clock::now();
    const std::chrono::minutes recordDuration(60);
    while (true) {
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
        fs::path newFilePath = Utilities::h264_to_avi(file, cameraThreadObj->getSettings().fps);
        // Delete the intermediate .h264 file.
        fs::remove(file);
        if (isFirst) {
            firstFilePath = newFilePath;
            isFirst = false;
        } else {
            // Backup the original file.
            fs::path backupPath{firstFilePath.generic_string() + ".bak"};
            fs::copy(firstFilePath, backupPath);
            // Concatenate the new file.
            Utilities::concat_mp4(firstFilePath, newFilePath);
            // Remove the new file.
            fs::remove(newFilePath);
            // Delete the backup.
            fs::remove(backupPath);
            auto timeDiff = std::chrono::steady_clock::now() - time_start;
            if(timeDiff > recordDuration) {
                isFirst = true;
                time_start = std::chrono::steady_clock::now();
            }
        }
    }

    // Terminate the running thread.
    cameraThreadObj->stop();
    std::clog.rdbuf(old_clog_rdbuf);
    std::cerr.rdbuf(old_cerr_rdbuf);
    return 0;
}
