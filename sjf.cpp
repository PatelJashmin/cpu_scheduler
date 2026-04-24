#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "scheduler.h"
#include "process.h"
#include "utils.h"

using namespace std;

class SJF : public Scheduler {
public:
    string name() override {
        return "Shortest Job First - Non-Preemptive (SJF)";
    }

    void run(vector<Process>& processes) override {
        int n = processes.size();
        vector<bool> done(n, false);
        int completed = 0;
        int current_time = 0;

        vector<pair<int,int>> timeline;

        while (completed < n) {
            // Find all processes that have arrived and are not done
            int chosen = -1;
            int shortest = INT_MAX;

            for (int i = 0; i < n; i++) {
                if (!done[i] && processes[i].arrival_time <= current_time) {
                    // Pick the one with shortest burst; break ties by arrival, then pid
                    if (processes[i].burst_time < shortest ||
                       (processes[i].burst_time == shortest &&
                        processes[i].arrival_time < processes[chosen].arrival_time)) {
                        shortest = processes[i].burst_time;
                        chosen = i;
                    }
                }
            }

            if (chosen == -1) {
                // No process is ready — CPU is idle
                timeline.push_back({current_time, 0});
                current_time++;
                continue;
            }

            Process& p = processes[chosen];

            // Response time = first time on CPU minus arrival
            p.response_time = current_time - p.arrival_time;

            // Run to completion (non-preemptive)
            for (int t = 0; t < p.burst_time; t++) {
                timeline.push_back({current_time, p.pid});
                current_time++;
            }

            p.completion_time  = current_time;
            p.turnaround_time  = p.completion_time - p.arrival_time;
            p.waiting_time     = p.turnaround_time - p.burst_time;

            done[chosen] = true;
            completed++;
        }

        printTimeline(timeline);
        printTable(processes);
        printAverages(processes);
    }
};

Scheduler* createSJF() {
    return new SJF();
}
