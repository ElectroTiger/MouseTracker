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

namespace Utilities {
    /// Returns the current time as a string in almost ISO 8601 format.
    std::string getCurrentTime();
    
}

#endif /* UTILITIES_H */

