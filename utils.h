#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include "process.h"

// Read processes from user keyboard input
void readFromCLI(std::vector<Process>& processes);

// Read processes from a file
bool readFromFile(std::vector<Process>& processes, const std::string& filename);

// Print the Gantt-style execution timeline
void printTimeline(const std::vector<std::pair<int,int>>& timeline);

// Print the process metrics table (PID, AT, BT, CT, TAT, WT, RT)
void printTable(const std::vector<Process>& processes);

// Print average WT and TAT
void printAverages(const std::vector<Process>& processes);

// Reset all computed fields so we can re-run a different algorithm
void resetProcesses(std::vector<Process>& processes);

#endif
