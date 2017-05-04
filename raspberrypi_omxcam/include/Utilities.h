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
    void h264_to_avi(const fs::path& file, float fps);
    /// Concatenate two .avi files file1 and file2 to a newFile.
    /// @warning This function is non-portable! It is only tested to work on Raspberry Pi when the program MP4Box is present on it.
    /// void concat_avi(const fs::path& file1, const fs::path& file2, const fs::path& newFile);
}

#endif /* UTILITIES_H */

