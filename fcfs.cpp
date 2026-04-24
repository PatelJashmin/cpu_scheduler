#include <iostream>
#include <vector>
#include <algorithm>
#include "scheduler.h"
#include "process.h"
#include "utils.h"

using namespace std;

class FCFS : public Scheduler {
public:
    string name() override {
        return "First Come First Serve (FCFS)";
    }

    void run(vector<Process>& processes) override {
        // Sort by arrival time; ties broken by pid
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            if (a.arrival_time == b.arrival_time)
                return a.pid < b.pid;
            return a.arrival_time < b.arrival_time;
        });

        int current_time = 0;
        vector<pair<int,int>> timeline; // (start_time, pid)

        for (Process& p : processes) {
            // If CPU is free before this process arrives, it sits idle
            if (current_time < p.arrival_time) {
                for (int t = current_time; t < p.arrival_time; t++)
                    timeline.push_back({t, 0}); // 0 = idle
                current_time = p.arrival_time;
            }

            // First time on CPU — record response time
            p.response_time = current_time - p.arrival_time;

            // Run to completion (non-preemptive)
            for (int t = 0; t < p.burst_time; t++) {
                timeline.push_back({current_time, p.pid});
                current_time++;
            }

            p.completion_time  = current_time;
            p.turnaround_time  = p.completion_time - p.arrival_time;
            p.waiting_time     = p.turnaround_time - p.burst_time;
        }

        printTimeline(timeline);
        printTable(processes);
        printAverages(processes);
    }
};

// Factory function so main.cpp can create an FCFS object
Scheduler* createFCFS() {
    return new FCFS();
}
