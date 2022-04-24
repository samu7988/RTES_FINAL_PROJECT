#!/bin/bash
rm -f *.csv
rm -f *.ppm
git pull
make clean
make
sudo ./main 1 1