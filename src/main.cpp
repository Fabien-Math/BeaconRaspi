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
#include <opencv2/imgproc/imgproc.hpp>

#include "slam_params.hpp"
#include "sensors_catcher.hpp"
#include "ov2slam.hpp"

#include "hardware_params.hpp"
#include "hardware_manager.hpp"

#include "path_params.hpp"
#include "path_manager.hpp"

#include "parameter_parser.cpp"


int main(int argc, char** argv)
{
    if(argc < 4)
    {
       std::cout << "\nUsage: Pass all parameter files as argument when calling the main function (ov2slam, hardware, path)\n";
       return 1;
    }

	bool running = true;
	
    // Load the parameters
    std::string ov2slam_parameters_file = argv[1];
    std::string hardware_parameters_file = argv[2];
    std::string path_parameters_file = argv[3];
	
	std::shared_ptr<SlamParams> slam_params = ParseSlamParams(ov2slam_parameters_file);
	std::shared_ptr<HardwareParams> hw_params = ParseHardwareParams(hardware_parameters_file);
	std::shared_ptr<PathParams> path_params = ParsePathParams(path_parameters_file);
	
    // Setting up the SLAM Manager
    SlamManager slam(slam_params);
	SensorsCatcher sensors_catcher(&slam);
	// slam.setSensorCatcher(&sensors_catcher);
    // Setting up the Hardware Manager
	HardwareManager hardware_mng(hw_params);
    // Setting up the Path Manager
	PathManager path_manager(path_params);

	
    // Start the SLAM thread
    std::cout << "\nLaunching OV²SLAM...\n\n";
    std::thread slamthread(&SlamManager::run, &slam);
	
    std::cout << "\nLaunching Path Planner...\n\n";
    std::thread ppthread(&PathManager::run, &path_manager);

    // Program loop
	while (running)
	{
		hardware_mng.Run(&sensors_catcher);
		// Call hardware manager to retreive data or give actions

	}

	// Request Slam Manager thread to exit
    slam.bexit_required_ = true;

    return 0;
}
