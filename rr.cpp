#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "scheduler.h"
#include "process.h"
#include "utils.h"

using namespace std;

class RoundRobin : public Scheduler {
    int quantum;

public:
    RoundRobin(int q) : quantum(q) {}

    string name() override {
        return "Round Robin (RR) [Quantum = " + to_string(quantum) + "]";
    }

    void run(vector<Process>& processes) override {
        int n = processes.size();
        int current_time = 0;
        int completed = 0;

        // Sort by arrival time initially
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.arrival_time < b.arrival_time;
        });

        // Keep track of which processes are in the ready queue
        queue<int> ready; // stores index into processes[]
        vector<bool> in_queue(n, false);

        vector<pair<int,int>> timeline;

        // Enqueue the first process (or wait until one arrives)

        // Helper to enqueue all processes that have arrived by 'current_time'
        auto enqueue_arrived = [&]() {
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] &&
                    processes[i].remaining_time > 0 &&
                    processes[i].arrival_time <= current_time) {
                    ready.push(i);
                    in_queue[i] = true;
                }
            }
        };

        enqueue_arrived();

        while (completed < n) {
            if (ready.empty()) {
                // CPU idle — advance time until next process arrives
                timeline.push_back({current_time, 0});
                current_time++;
                enqueue_arrived();
                continue;
            }

            int idx = ready.front();
            ready.pop();

            Process& p = processes[idx];

            // First time on CPU
            if (!p.started) {
                p.response_time = current_time - p.arrival_time;
                p.started = true;
            }

            // Run for up to 'quantum' time units
            int run_time = min(quantum, p.remaining_time);

            for (int t = 0; t < run_time; t++) {
                timeline.push_back({current_time, p.pid});
                current_time++;

                // While running, check if new processes have arrived
                // (we'll add them to the queue after the current slice)
            }

            p.remaining_time -= run_time;

            // Enqueue any new arrivals that happened during this slice
            // (they go before re-enqueuing the current process, if it's still running)
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] &&
                    processes[i].remaining_time > 0 &&
                    processes[i].arrival_time <= current_time) {
                    ready.push(i);
                    in_queue[i] = true;
                }
            }

            if (p.remaining_time == 0) {
                // Process is done
                p.completion_time  = current_time;
                p.turnaround_time  = p.completion_time - p.arrival_time;
                p.waiting_time     = p.turnaround_time - p.burst_time;
                completed++;
            } else {
                // Re-add to back of queue
                ready.push(idx);
                // Note: in_queue[idx] is already true, so the loop above won't re-add it
            }
        }

        printTimeline(timeline);
        printTable(processes);
        printAverages(processes);
    }
};

Scheduler* createRR(int quantum) {
    return new RoundRobin(quantum);
}
