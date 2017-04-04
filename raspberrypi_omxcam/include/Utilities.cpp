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