/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Utilities.cpp
 * Author: Developer
 * 
 * Created on February 13, 2017, 10:33 PM
 */

#include "Utilities.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <boost/filesystem/convenience.hpp>

std::string Utilities::getCurrentTime() {
    auto currentTime = std::chrono::system_clock::now();
    auto currentTime_time_t = std::chrono::system_clock::to_time_t(currentTime);
    auto currentTime_c = std::localtime(&currentTime_time_t);
    // Use ISO 8601 format.
    std::ostringstream fileName;
    fileName
            << std::setw(4) << currentTime_c->tm_year + 1900 << '-'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_mon + 1 << '-'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_mday << 'T'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_hour << '.'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_min << '.'
            << std::setw(2) << std::setfill('0') << currentTime_c->tm_sec + 1
            ;
    return fileName.str();
}

fs::path Utilities::h264_to_avi(const fs::path& file, float fps) {
    fs::path newFile = fs::change_extension(file, "mp4");
    // Check that file actually points to a .h264 file.
    if (!file.extension().compare("h264")) {
        throw new std::invalid_argument("Provided file does not have .h264 extension.");
    }

    if (fps <= 0) {
        throw new std::invalid_argument("fps cannot be less than or equal to zero.");
    }

    // If .mp4 file exists, delete it and log the event.
    if (fs::exists(newFile)) {
        std::cerr << "Utilities.cpp -> Warning -> " << newFile.generic_string() << " exists, deleting.";
        fs::remove(newFile);
    }


    std::string command = "MP4Box -add " + file.generic_string() + ":fps=" + std::to_string(fps) + " " + newFile.generic_string();
    system(command.c_str());
    
    return newFile;
}

void Utilities::concat_mp4(const fs::path& before, const fs::path& after) {
    if (!before.extension().compare("mp4")) {
        throw new std::invalid_argument("Provided file1 does not have .h264 extension.");
    }
    if (!after.extension().compare("mp4")) {
        throw new std::invalid_argument("Provided file2 does not have .mp4 extension.");
    }
    
    std::string command = "MP4Box -cat " + after.generic_string() + " " + before.generic_string();
    system(command.c_str());

}