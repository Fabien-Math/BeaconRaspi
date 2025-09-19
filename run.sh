#!/bin/bash

# Change these parameter files as needed
param_file1="parameters/ov2slam_params.yaml"
param_file2="parameters/hardware_params.yaml"
param_file3="parameters/path_params.yaml"

# Path to your executable
executable="./build/beacon_bot_main"

# Run the executable with the parameters
echo "Running: $executable $param_file1 $param_file2 $param_file3"
$executable "$param_file1" "$param_file2" "$param_file3"

# Check exit status
if [ $? -ne 0 ]; then
  echo "Application exited with an error."
else
  echo "Application ran successfully."
fi
