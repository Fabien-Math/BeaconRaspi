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

#include <opencv2/opencv.hpp>

#include "sensors_catcher.hpp"

#include "hardware_manager.hpp"
#include "hardware_params.hpp"

HardwareManager::HardwareManager(std::shared_ptr<HardwareParams> params)
{
	_image_width = 1280;
	_image_height = 720;
	_camera_port = 0;

	_running = true;

	_freq = 5; // Hz
	double time = std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
	_next_capture_time = time;
}


void HardwareManager::Run(SensorsCatcher *sensors_catcher)
{
	cv::VideoCapture camera(_camera_port, cv::CAP_V4L2);

	while (!camera.isOpened())
		initCameraPort();
	
	
	// Set the video resolution for both images capture
	camera.set(cv::CAP_PROP_FRAME_WIDTH, _image_width);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, _image_height);

	while (_running)
	{
		double time = std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
		
		if (time < _next_capture_time)
			continue;

		// Increment next time for image capture
		_next_capture_time += 1.0 / _freq;

		cv::Mat frame;

		// Get a new frame from camera
		camera >> frame;
		
		sensors_catcher->subLeftImage(frame, time);
		sensors_catcher->sync_process();
	}
}



void HardwareManager::initCameraPort()
{
	for (int i = 0; i < 10; i++)
	{
		// cv::VideoCapture _camera(i);
		cv::VideoCapture camera(i, cv::CAP_V4L2);
		
		std::cout << "Try to open video" << i << "port..." << std::endl;
		if(camera.isOpened())
		{
			_camera_port = i;
			std::cout << "Camera port initialized to video" << i << "." << std::endl;
			camera.release();

			return;
		}

		camera.release();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
