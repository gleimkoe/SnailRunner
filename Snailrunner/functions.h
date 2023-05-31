#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include "SnailRunner.h"
#include <ctime>
#include "IntelligentMotor.h"

using namespace std;

//Menu functions
void calibrationMenu(SnailRunner* robot);
void settingsMenu(SnailRunner* robot);

//Keeping track of Time
void startTimer(SnailRunner* robot);
double getElapsedTime(SnailRunner* robot);
void endTimer(SnailRunner* robot);
double getDuration(SnailRunner* robot);
std::string getTimeStamp();

//Keeping track of Track
void resetDistance(SnailRunner* robot);
void resetEncoder(SnailRunner* robot);
void addtoDistance(SnailRunner* robot);

//Log Functions
void logStartConditions(SnailRunner* robot, std::ofstream &file);
void logLapBanner(SnailRunner* robot, std::ofstream &file);
void logOFF_TRAIL(SnailRunner* robot, std::ofstream &file);
void logObstacle(SnailRunner* robot, std::ofstream &file);
void logCorner(SnailRunner* robot, std::ofstream &file);
void logLapConclusion(SnailRunner* robot, std::ofstream &file);

#endif