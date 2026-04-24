# CPU Scheduling Simulator (C++)

A command-line based simulator implementing core CPU scheduling algorithms used in operating systems.

## Implemented Algorithms

* FCFS (First Come First Serve)
* SJF (Shortest Job First)
* SRTF (Shortest Remaining Time First - Preemptive)
* Priority Scheduling
* Round Robin (with Time Quantum)

## Features

* Modular architecture (separate files for each algorithm)
* Supports both file-based and CLI input
* Displays execution timeline (Gantt-style)
* Computes:

  * Waiting Time (WT)
  * Turnaround Time (TAT)
  * Response Time (RT)
  * Average metrics

## Project Structure
main.cpp          // Entry point

fcfs.cpp          // FCFS implementation


sjf.cpp           // SJF implementation

srtf.cpp          // SRTF implementation

priority.cpp      // Priority scheduling

rr.cpp            // Round Robin

utils.cpp/.h      // Input, output, helper functions

process.h         // Process structure

## How to Compile
g++ *.cpp -o scheduler

## How to Run
./scheduler processes.txt

OR
./scheduler < processes.txt

## Input Format
PID ArrivalTime BurstTime

### Example Input
5

1 0 5

2 1 3

3 2 8

4 3 6

5 4 2

## Output
* Execution timeline (Gantt chart style)
* Table of process metrics
* Average Waiting Time and Turnaround Time

## Notes
* Designed for educational use (Operating Systems concepts)
* Easily extensible to add more scheduling algorithms
* Round Robin uses a configurable time quantum
