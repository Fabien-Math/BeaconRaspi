/*    
*    Copyright (C) 2025 Fabien MATHE
*
*	 Author : Fabien MATHE
*/

#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

#include "sensors_catcher.hpp"
#include "hardware_params.hpp"

class HardwareManager {

	public:
		HardwareManager(std::shared_ptr<HardwareParams> params);

		void Run(SensorsCatcher *sensors_catcher);

	private:
		void initCameraPort();
		void getImage();
		void getIMU();

		void mesureBatteryLevel();
		void askedBatteryLevel();

		void setServos();
		void getServos();

		void setLights();

		void sendAction();

		bool _running;
		double _next_capture_time;

		int _camera_port;
		int _image_width;
		int _image_height;
		double _freq;

		
	};
	
