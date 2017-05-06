/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Utilities.h
 * Author: Developer
 *
 * Created on February 13, 2017, 10:33 PM
 * Description: Contains extraneous functions useful to all parts of IRCamera. 
 */

#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
#include <fstream>
#include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;
namespace Utilities {
    /// Returns the current time as a string in almost ISO 8601 format.
    std::string getCurrentTime();
    /// Convert the .h264 file pointed to by "path" to a .avi file by calling an external program.
    /// @warning This function is non-portable! It is only tested to work on Raspberry Pi when the program MP4Box is present on it.
    /// @return the path of the new .mp4 file.
    fs::path h264_to_avi(const fs::path& file, float fps);
    /// Appends the .mp4 file "after" the .mp4 file "before". 
    /// @warning This function is non-portable! It is only tested to work on Raspberry Pi when the program MP4Box is present on it.
    void concat_mp4(const fs::path& before, const fs::path& after);
}

#endif /* UTILITIES_H */

