#!/bin/bash
echo "DEPENDENCIES are sdl2 library for context creation and glm library for math"
echo "url to sdl2 https://www.libsdl.org/  and to glm https://glm.g-truc.net/0.9.9/index.html "

echo "Trying to install SDL2"
sudo apt install libsdl2-dev

echo "Trying to install glm and it's doc"

sudo apt install libglm-dev
sudo apt install libglm-doc
