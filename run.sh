#!/bin/bash

PROJECT_NAME=vulkan_renderer
PATH_TO_MAKE=out/build
PATH_TO_EXEC=out/build

(cd $PATH_TO_MAKE && make)

(cd $PATH_TO_EXEC && ./$PROJECT_NAME)
