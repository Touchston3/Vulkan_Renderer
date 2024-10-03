#!/bin/bash

EXECUTABLE_NAME=Renderer
PATH_TO_BUILD=build

cmake --build $PATH_TO_BUILD
(cd $PATH_TO_BUILD && ./$EXECUTABLE_NAME)
