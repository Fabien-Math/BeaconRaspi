/**
*    This file is an addon of OV²SLAM.
*    
*    Copyright (C) 2025 Fabien MATHE
*
*    For more information see <https://github.com/ov2slam/ov2slam>
*
*	 Author : Fabien MATHE
*/

#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

#include <opencv2/core.hpp>

#include "ov2slam.hpp"

class SensorsCatcher {

	public:
		SensorsCatcher(SlamManager *slam);
	
		void subLeftImage(const cv::Mat &image, double time);
	
		void subRightImage(const cv::Mat &image, double time);
	
		cv::Mat convertToGrayImage(const cv::Mat &image);
	
		// extract images with same timestamp from two topics
		// (mostly derived from Vins-Fusion: https://github.com/HKUST-Aerial-Robotics/VINS-Fusion)
		void sync_process();
	
		std::queue<cv::Mat> img0_buf_;
		std::queue<cv::Mat> img1_buf_;

		double img0_time_;
		double img1_time_;

		std::mutex img_mutex_;
		
		SlamManager *pslam_;
	};
	
