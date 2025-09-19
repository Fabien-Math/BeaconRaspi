cmake_minimum_required(VERSION 3.8.2)  # Modern version for better features
project(beacon_bot LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# Compilation flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -march=native -g")
# Note: If you have issues with -march=native, recompile dependencies or remove it

# Options
option(WITH_OPENCV_CONTRIB "Use OpenCV contrib modules" OFF)
option(WITH_IBOW_LCD "Use iBOW-LCD Loop Closer" OFF)

# Informative message
if(WITH_OPENCV_CONTRIB)
    add_definitions(-DOPENCV_CONTRIB)
    message(STATUS "OpenCV contrib is enabled.")
else()
    message(STATUS "OpenCV contrib is disabled.")
endif()

# Dependencies
find_package(OpenCV REQUIRED)
if(OpenCV_VERSION_MAJOR LESS 3)
    message(FATAL_ERROR "OpenCV version 3 or higher is required. Found: ${OpenCV_VERSION}")
endif()

if(OpenCV_VERSION VERSION_LESS "3.3.0")
    add_definitions(-DOPENCV_LAMBDA_MISSING)
    message(STATUS "OpenCV < 3.3.0 detected: enabling internal ParallelLoopBodyLambdaWrapper.")
endif()

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
message(STATUS "Found Eigen3: ${Eigen3_VERSION}")

find_package(PCL 1.3 REQUIRED)
add_definitions(${PCL_DEFINITIONS})
message(STATUS "Found PCL: ${PCL_VERSION}")

find_package(Ceres REQUIRED PATHS ${PROJECT_SOURCE_DIR}/Thirdparty/ceres-solver/install/)
message(STATUS "Found Ceres: ${CERES_VERSION}")

find_package(Sophus REQUIRED PATHS ${PROJECT_SOURCE_DIR}/Thirdparty/Sophus/build/)
message(STATUS "Found Sophus: ${Sophus_VERSION}")

find_package(opengv QUIET)
if(opengv_FOUND)
    add_definitions(-DUSE_OPENGV)
    get_target_property(opengv_INCLUDE_DIR opengv INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "Found OpenGV at ${opengv_INCLUDE_DIR}")
endif()

# iBOW-LCD detection
if(EXISTS "${PROJECT_SOURCE_DIR}/Thirdparty/ibow_lcd/build/liblcdetector.so")
    add_definitions(-DIBOW_LCD)
    set(WITH_IBOW_LCD ON)
    message(STATUS "iBOW-LCD detected and enabled.")
else()
    message(STATUS "iBOW-LCD not found. Loop Closer disabled.")
endif()

# Include backward-cpp for better stack traces
add_subdirectory(Thirdparty/backward-cpp)

# Library target
add_library(${PROJECT_NAME}
    src/parameter_parser.cpp
    src/ov2slam/ov2slam.cpp
    src/ov2slam/logger.cpp
    src/ov2slam/sensors_catcher.cpp
    src/ov2slam/visual_front_end.cpp
    src/ov2slam/frame.cpp
    src/ov2slam/slam_params.cpp
    src/ov2slam/camera_calibration.cpp
    src/ov2slam/feature_extractor.cpp
    src/ov2slam/feature_tracker.cpp
    src/ov2slam/map_manager.cpp
    src/ov2slam/map_point.cpp
    src/ov2slam/mapper.cpp
    src/ov2slam/multi_view_geometry.cpp
    src/ov2slam/ceres_parametrization.cpp
    src/ov2slam/optimizer.cpp
    src/ov2slam/estimator.cpp
    src/ov2slam/loop_closer.cpp
    src/path_planner/path_manager.cpp
    src/path_planner/path_params.cpp 
    src/hardware/hardware_manager.cpp
    src/hardware/hardware_params.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/ov2slam
        ${PROJECT_SOURCE_DIR}/include/hardware
        ${PROJECT_SOURCE_DIR}/include/path_planner
        ${PROJECT_SOURCE_DIR}/include/ov2slam/ceres_parametrization
		${OpenCV_INCLUDE_DIRS}
		${PCL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Eigen3::Eigen
        Sophus::Sophus
    PRIVATE
        ${OpenCV_LIBS}
        Ceres::ceres
        ${PCL_LIBRARIES}
)

if(WITH_IBOW_LCD)
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/Thirdparty/ibow_lcd/include
            ${PROJECT_SOURCE_DIR}/Thirdparty/obindex2/lib/include
    )
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/Thirdparty/ibow_lcd/build/liblcdetector.so
    )
endif()

if(opengv_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengv)
endif()

# Executable
add_executable(${PROJECT_NAME}_main src/main.cpp ${BACKWARD_ENABLE})
add_backward(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}_main PRIVATE ${PROJECT_NAME})

# Optional: Add dependencies or install targets if needed
# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_main DESTINATION bin)
