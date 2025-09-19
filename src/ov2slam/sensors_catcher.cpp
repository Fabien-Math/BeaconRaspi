/**
*    This file is part of OV²SLAM.
*    
*    Copyright (C) 2020 ONERA
*
*    For more information see <https://github.com/ov2slam/ov2slam>
*
*    OV²SLAM is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    OV²SLAM is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with OV²SLAM.  If not, see <https://www.gnu.org/licenses/>.
*
*    Authors: Maxime Ferrera     <maxime.ferrera at gmail dot com> (ONERA, DTIS - IVA),
*             Alexandre Eudes    <first.last at onera dot fr>      (ONERA, DTIS - IVA),
*             Julien Moras       <first.last at onera dot fr>      (ONERA, DTIS - IVA),
*             Martial Sanfourche <first.last at onera dot fr>      (ONERA, DTIS - IVA)
* 	
*	THE CODE IS MODIFIED AND DIFFERS FROM THE ONE ORIGNALY MADE BY THE AUTHORS
*/

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ov2slam.hpp"
#include "sensors_catcher.hpp"


SensorsCatcher::SensorsCatcher(SlamManager *slam): pslam_(slam) 
{
	std::cout << "\nSensors Catcher is created...\n";
}
	
void SensorsCatcher::subLeftImage(const cv::Mat &image, double time) {
	std::lock_guard<std::mutex> lock(img_mutex_);
	img0_buf_.push(image);
	img0_time_ = time;
}

void SensorsCatcher::subRightImage(const cv::Mat &image, double time) {
	std::lock_guard<std::mutex> lock(img_mutex_);
	img1_buf_.push(image);
	img1_time_ = time;
}
	
cv::Mat SensorsCatcher::convertToGrayImage(const cv::Mat &image)
{
	// Get and prepare images
	cv::Mat gray_image;
	cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

	return gray_image;
}
	
// extract images with same timestamp from two topics
// (mostly derived from Vins-Fusion: https://github.com/HKUST-Aerial-Robotics/VINS-Fusion)
void SensorsCatcher::sync_process()
{
	std::cout << "\nStarting the measurements reader thread!\n";
	
	// while( !pslam_->bexit_required_ )
	// {
		if( pslam_->pslamstate_->stereo_ )
		{
			cv::Mat image0, image1;

			std::lock_guard<std::mutex> lock(img_mutex_);

			if (!img0_buf_.empty() && !img1_buf_.empty())
			{
				// sync tolerance
				if(img0_time_ < img1_time_ - 0.015)
				{
					img0_buf_.pop();
					std::cout << "\n Throw img0 -- Sync error : " << (img0_time_ - img1_time_) << "\n";
				}
				else if(img0_time_ > img1_time_ + 0.015)
				{
					img1_buf_.pop();
					std::cout << "\n Throw img1 -- Sync error : " << (img0_time_ - img1_time_) << "\n";
				}
				else
				{
					image0 = convertToGrayImage(img0_buf_.front());
					image1 = convertToGrayImage(img1_buf_.front());
					img0_buf_.pop();
					img1_buf_.pop();

					if( !image0.empty() && !image1.empty() ) {
						pslam_->addNewStereoImages(img0_time_, image0, image1);
					}
				}
			}
		} 
		else if( pslam_->pslamstate_->mono_ ) 
		{
			cv::Mat image0;

			std::lock_guard<std::mutex> lock(img_mutex_);

			if ( !img0_buf_.empty() )
			{
				image0 = convertToGrayImage(img0_buf_.front());
				img0_buf_.pop();

				if( !image0.empty()) {
					pslam_->addNewMonoImage(img0_time_, image0);
				}
			}
		// }

		std::chrono::milliseconds dura(1);
		std::this_thread::sleep_for(dura);
	}

	std::cout << "\n Bag reader SyncProcess thread is terminating!\n";
}
		
