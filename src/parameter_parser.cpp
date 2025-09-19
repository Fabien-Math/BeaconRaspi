/*    
*    Copyright (C) 2025 Fabien MATHE
*
*	 Author : Fabien MATHE
*/

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

#include <opencv2/core.hpp>

#include "hardware_params.hpp"
#include "path_params.hpp"
#include "slam_params.hpp"


std::shared_ptr<SlamParams> ParseSlamParams(std::string ov2slam_parameters_file);
std::shared_ptr<HardwareParams> ParseHardwareParams(std::string hardware_parameters_file);
std::shared_ptr<PathParams> ParsePathParams(std::string path_parameters_file);


std::shared_ptr<SlamParams> ParseSlamParams(std::string ov2slam_parameters_file)
{
	std::cout << "\nLoading parameters file : " << ov2slam_parameters_file << "...\n";
	
    const cv::FileStorage fsSettings(ov2slam_parameters_file.c_str(), cv::FileStorage::READ);
    if(!fsSettings.isOpened()) {
		std::cout << "Failed to open settings file...";
    } else {
		std::cout << "\nParameters file loaded...\n";
    }
	std::cout << "\nLoading parameters file : " << ov2slam_parameters_file << "...\n";

	std::shared_ptr<SlamParams> params;
    params.reset( new SlamParams(fsSettings) );

	return params;
}

std::shared_ptr<HardwareParams> ParseHardwareParams(std::string hardware_parameters_file)
{
	std::cout << "\nLoading parameters file : " << hardware_parameters_file << "...\n";
	
    const cv::FileStorage fsSettings(hardware_parameters_file.c_str(), cv::FileStorage::READ);
    if(!fsSettings.isOpened()) {
		std::cout << "Failed to open settings file...";
    } else {
		std::cout << "\nParameters file loaded...\n";
    }
	std::cout << "\nLoading parameters file : " << hardware_parameters_file << "...\n";

	std::shared_ptr<HardwareParams> params;
    params.reset( new HardwareParams(fsSettings) );

	return params;
}

std::shared_ptr<PathParams> ParsePathParams(std::string path_parameters_file)
{
	std::cout << "\nLoading parameters file : " << path_parameters_file << "...\n";
	
    const cv::FileStorage fsSettings(path_parameters_file.c_str(), cv::FileStorage::READ);
    if(!fsSettings.isOpened()) {
		std::cout << "Failed to open settings file...";
    } else {
		std::cout << "\nParameters file loaded...\n";
    }
	std::cout << "\nLoading parameters file : " << path_parameters_file << "...\n";

	std::shared_ptr<PathParams> params;
    params.reset( new PathParams(fsSettings) );

	return params;
}