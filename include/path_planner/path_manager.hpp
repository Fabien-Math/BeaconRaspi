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

#include "path_params.hpp"

class PathManager {

	public:
		PathManager(std::shared_ptr<PathParams> params);

		void run();

	private:
		
	};
	
