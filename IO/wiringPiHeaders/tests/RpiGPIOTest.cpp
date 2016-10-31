/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RpiGPIOTest.cpp
 * Author: Developer
 *
 * Created on October 16, 2016, 6:05 PM
 */

#include <stdlib.h>
#include <iostream>
#include "IO.h"
#include "IORpi.h"


/*
 * Simple C++ Test Suite
 */

void test1() {
//    IO::Pin ledPin(17, IO::PinMode::OUTPUT);
    //ledPin.write(1);
    std::cout << "RpiGPIOTest test 1" << std::endl;
}

void test2() {
    std::cout << "RpiGPIOTest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (RpiGPIOTest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% RpiGPIOTest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (RpiGPIOTest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (RpiGPIOTest)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (RpiGPIOTest)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (RpiGPIOTest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

