# Lidar Client

This project implements interaction with the 2D lidar sensor company [sick](https://www.sick.com/us/en/lidar-sensors/2d-lidar-sensors/lms1xx/c/g91901).

## About project

Lidar Client is a tcp-client application that receives hexadecimal telegrams from tcp-server - lidar.
The format of telegrams for communication with the lidar was given in the documentation for it.
The program allows to receive data from the device, both in continuous scanning mode and in last scan shot.
The result of the lidar scan is displayed on the screen as a graph, which is a cloud of points.

The result of working in the last scan mode is shown in the screenshot below.

![scan](https://github.com/annachs/LidarClient/assets/120400708/ce60977d-ce7f-4f91-be2f-910dbdef71bc)
