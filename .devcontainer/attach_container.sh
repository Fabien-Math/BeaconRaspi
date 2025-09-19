#!/bin/bash

CONTAINER_NAME="rpi-zero2-container"

# Check the status of the container
CONTAINER_STATUS=$(docker ps -a --filter "name=${CONTAINER_NAME}" --format "{{.State}}")

case "$CONTAINER_STATUS" in
	"")
		echo "Container does not exist. Starting it with docker-compose..."
		docker compose up -d $CONTAINER_NAME
		echo "Attaching to the container..."
		docker exec -it "$CONTAINER_NAME" bash
		;;
	"exited")
		echo "Container exists but is stopped. Starting it..."
		docker start "$CONTAINER_NAME"
		echo "Attaching to the container..."
		docker exec -it "$CONTAINER_NAME" bash
		;;
	"running")
		echo "Container is already running. Attaching to it..."
		docker exec -it "$CONTAINER_NAME" bash
		;;
	*)
		echo "Unexpected container state: $CONTAINER_STATUS"
		exit 1
		;;
esac