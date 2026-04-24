#ifndef PROCESS_H
#define PROCESS_H

// Represents a single process in the simulation
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;   // used in preemptive algorithms
    int priority;         // lower number = higher priority

    // filled in after simulation
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;

    bool started;         // has the process been picked up by CPU at least once?

    Process() :
        pid(0), arrival_time(0), burst_time(0), remaining_time(0),
        priority(0), completion_time(0), turnaround_time(0),
        waiting_time(0), response_time(0), started(false) {}
};

#endif
