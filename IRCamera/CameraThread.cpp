/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Function called from thread which opens the camera, records from it, and 

#include "CameraThread.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <exception>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

std::string CameraThread::getCurrentTime(void) {
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

CameraThread::CameraThread() :
settings(defaultSettings), videoOn(false), numPicturesToTake(0), terminateNow(false) {
    setSettings(settings);
    thread = std::thread(&CameraThread::threadFunc, std::ref(*this));
}

CameraThread::~CameraThread() {
    //std::lock_guard<std::mutex> lock(settingsMutex);
    terminateNow = true;
    //settingscv.notify_one();
    if (thread.joinable()) {
        thread.join();
    }
}

bool CameraThread::getVideoOn() {
    std::lock_guard<std::mutex> lock(settingsMutex); // May not be needed because videoOn is written by the thread.
    return videoOn;
}

void CameraThread::setVideoOn(bool isOn) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    videoOn = isOn;
    settingscv.notify_one();
}

void CameraThread::takePicture() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    numPicturesToTake += 1;
    settingscv.notify_one();
}

void CameraThread::writePictureFrame(cv::Mat const &frame) {
    // Write the image.
    std::string filename = getCurrentTime() + ".png";
    try {
        imwrite(filename, frame); // This may throw an exception.
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
    completedFileNames.push_back(filename);
}

void CameraThread::threadFunc() {
        raspicam::RaspiCam_Cv camera;
    /* Check if the camera is attached. */

    /* Initialize the camera*/

    cv::VideoWriter writer;
    cv::Mat frame;
    std::string filename;

    enum ThreadState {
        OFF, VIDEO, CAMERA
    } state = OFF;
    bool terminate = false;

    while (!terminate) {
        switch (state) {
            case OFF:
            {
                std::unique_lock<std::mutex> lock(settingsMutex); // Unique lock is necessary because std::condition_variable requires it as input.
                // Release the camera and writer in case we arrived here due to an exception.
                if (camera.isOpened()) {
                    camera.release();
                }
                if (writer.isOpened()) {
                    writer.release();
                }
                if (terminateNow) {
                    terminate = true;
                    continue;
                }
                // Wait on a condition variable.
                settingscv.wait(lock);
                
                // Change the settings if need be.
                camera.set(CV_CAP_PROP_FRAME_WIDTH, settings.width);
                camera.set(CV_CAP_PROP_FRAME_HEIGHT, settings.height);
                camera.set(CV_CAP_PROP_FORMAT, settings.isColor ? CV_8UC3 : CV_8UC1);
                camera.set(CV_CAP_PROP_BRIGHTNESS, settings.brightness * 100 / 255.);
                camera.set(CV_CAP_PROP_CONTRAST, settings.contrast * 100 / 255.);
                camera.set(CV_CAP_PROP_SATURATION, settings.saturation * 100 / 255.);
                camera.set(CV_CAP_PROP_GAIN, settings.gain * 100 / 255.);
                camera.set(CV_CAP_PROP_EXPOSURE, settings.exposure * 100 / 255.);
                camera.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U, settings.whiteBalanceBlue * 100 / 255.);
                camera.set(CV_CAP_PROP_WHITE_BALANCE_RED_V, settings.whiteBalanceRed * 100 / 255.);
                
                if (numPicturesToTake > 0) {
                    state = CAMERA;
                } else if (videoOn) {
                    state = VIDEO;
                } else {
                    state = OFF;
                }

                break;
            }

            case VIDEO:
            {

                // Open the camera and throw an exception on failure.
                if (!camera.open()) {
                    throw std::runtime_error("Camera could not be opened.");
                    state = OFF;
                    continue;
                }

                // Prepare settings for the video writer.
                filename = getCurrentTime() + ".avi"; // Filename of video output.
                const int codec = CV_FOURCC('H', '2', '6', '4'); // Codec
                const cv::Size size((int) camera.get(CV_CAP_PROP_FRAME_WIDTH),
                        (int) camera.get(CV_CAP_PROP_FRAME_HEIGHT));

                {
                    std::lock_guard<std::mutex> lock(settingsMutex);
                            // Open the video writer and throw an exception on failure.
                    if (!writer.open(filename, codec, settings.fps, size, settings.isColor)) {
                        throw std::runtime_error("Video file could not be opened.");
                        state = OFF;
                        continue;
                    }
                }
                        
                
                auto lastTimePoint = std::chrono::steady_clock::now();
                while (!terminateNow && videoOn) {
                    auto sleepUntil = lastTimePoint + fps_to_stdchrono;
                    auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(sleepUntil - std::chrono::steady_clock::now());
                    if (elapsedTime.count() < 0) {
                        std::cerr << "Warning: fps requirement cannot be met. Exceeded by " << -elapsedTime.count() << " us. Likely set too high." << std::endl;
                    } else {
                        std::this_thread::sleep_until(sleepUntil);
                    }
                    if (!camera.grab()) {
                        throw std::runtime_error("Frame grab failed. Writing captured video.");
                        std::lock_guard<std::mutex> lock(settingsMutex);
                        videoOn = false;
                        continue;
                    }
                    lastTimePoint = std::chrono::steady_clock::now();

                    camera.retrieve(frame);

                    // Convert the frame to RGB color since it seems to be taken in BGR color.
                    //cv::Mat frameTemp = frame; 
                    //cv::cvtColor(frameTemp, frame, CV_BGR2RGB);

                    writer.write(frame);
                    if (numPicturesToTake != 0) {
                        writePictureFrame(frame);
                        numPicturesToTake--;
                    }
                }

                camera.release();
                writer.release();
                completedFileNames.push_back(filename);
                state = OFF;
                break;

                break;
            }

            case CAMERA:
            {
                // Open the camera
                if (!camera.open()) {
                    throw std::runtime_error("Camera could not be opened.");
                    state = OFF;
                    continue;
                }

                // Grab the frame.
                if (!camera.grab()) {
                    throw std::runtime_error("Frame grab failed. Writing captured video.");
                    state = OFF;
                    continue;
                }
                camera.retrieve(frame);
                // Convert the frame to RGB color since it seems to be taken in BGR color.
                cv::Mat frameTemp = frame;
                cv::cvtColor(frameTemp, frame, CV_BGR2RGB);

                writePictureFrame(frame);
                completedFileNames.push_back(filename);

                // Release the camera.
                camera.release();
                numPicturesToTake--;
                state = OFF;
                break;
            }
        }
    }
}

CameraThread::Settings CameraThread::getSettings() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return settings;
}

void CameraThread::setSettings(Settings const &newSettings) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    settings = newSettings;
    fps_to_stdchrono = std::chrono::microseconds(static_cast<int32_t> (1000000 / settings.fps));    
    settingscv.notify_one();
}

std::vector<std::string> CameraThread::getCompletedFilenames() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return completedFileNames;
}