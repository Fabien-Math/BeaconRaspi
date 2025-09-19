/*    
*    Copyright (C) 2025 Fabien MATHE
*
*	 Author : Fabien MATHE
*/
#pragma once


#include <iostream>
#include <string>
#include <chrono>

#include <opencv2/core.hpp>

class HardwareParams {

public:

    HardwareParams();
    
    HardwareParams(const cv::FileStorage &fsSettings);

    void reset();

};
