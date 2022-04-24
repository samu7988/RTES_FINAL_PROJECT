#!/bin/bash

ffmpeg -framerate $1 -i "test%08d.ppm" output.mp4
