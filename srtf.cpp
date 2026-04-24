#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "scheduler.h"
#include "process.h"
#include "utils.h"

using namespace std;

class SRTF : public Scheduler {
public:
    string name() override {
        return "Shortest Remaining Time First - Preemptive (SRTF)";
    }

    void run(vector<Process>& processes) override {
        int n = processes.size();
        int completed = 0;
        int current_time = 0;

        // Find the time at which the last process arrives
        int max_time = 0;
        for (auto& p : processes)
            max_time = max(max_time, p.arrival_time + p.burst_time);

        vector<pair<int,int>> timeline;

        while (completed < n) {
            // Pick the process with shortest remaining time that has arrived
            int chosen = -1;
            int shortest = INT_MAX;

            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                    if (processes[i].remaining_time < shortest) {
                        shortest = processes[i].remaining_time;
                        chosen = i;
                    }
                }
            }

            if (chosen == -1) {
                // CPU idle
                timeline.push_back({current_time, 0});
                current_time++;
                continue;
            }

            Process& p = processes[chosen];

            // Record response time on first execution
            if (!p.started) {
                p.response_time = current_time - p.arrival_time;
                p.started = true;
            }

            // Execute for 1 time unit (preemptive — check again next unit)
            timeline.push_back({current_time, p.pid});
            p.remaining_time--;
            current_time++;

            // Process just finished
            if (p.remaining_time == 0) {
                p.completion_time  = current_time;
                p.turnaround_time  = p.completion_time - p.arrival_time;
                p.waiting_time     = p.turnaround_time - p.burst_time;
                completed++;
            }
        }

        printTimeline(timeline);
        printTable(processes);
        printAverages(processes);
    }
};

Scheduler* createSRTF() {
    return new SRTF();
}
