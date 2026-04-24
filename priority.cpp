#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "scheduler.h"
#include "process.h"
#include "utils.h"

using namespace std;

// Non-preemptive Priority Scheduling
// Lower priority number = higher priority (e.g., priority 1 runs before priority 3)
class PriorityScheduler : public Scheduler {
public:
    string name() override {
        return "Priority Scheduling - Non-Preemptive (lower number = higher priority)";
    }

    void run(vector<Process>& processes) override {
        int n = processes.size();
        vector<bool> done(n, false);
        int completed = 0;
        int current_time = 0;

        vector<pair<int,int>> timeline;

        while (completed < n) {
            // Among arrived and not-done processes, pick highest priority
            // (lowest priority number)
            int chosen = -1;
            int best_priority = INT_MAX;

            for (int i = 0; i < n; i++) {
                if (!done[i] && processes[i].arrival_time <= current_time) {
                    if (processes[i].priority < best_priority) {
                        best_priority = processes[i].priority;
                        chosen = i;
                    }
                    // Tie-break: among same priority, pick earlier arrival
                    else if (processes[i].priority == best_priority) {
                        if (processes[i].arrival_time < processes[chosen].arrival_time)
                            chosen = i;
                    }
                }
            }

            if (chosen == -1) {
                // No process ready — CPU idle
                timeline.push_back({current_time, 0});
                current_time++;
                continue;
            }

            Process& p = processes[chosen];

            // Record first time on CPU
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

Scheduler* createPriority() {
    return new PriorityScheduler();
}
