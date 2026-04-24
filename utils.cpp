#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "utils.h"
#include "process.h"

using namespace std;

void readFromCLI(vector<Process>& processes) {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        Process p;
        p.pid = i + 1;
        cout << "\nProcess " << p.pid << ":\n";
        cout << "  Arrival Time : ";
        cin >> p.arrival_time;
        cout << "  Burst Time   : ";
        cin >> p.burst_time;
        cout << "  Priority     : ";
        cin >> p.priority;

        p.remaining_time = p.burst_time;
        processes.push_back(p);
    }
}

bool readFromFile(vector<Process>& processes, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'\n";
        return false;
    }

    // Skip the header line (pid arrival burst priority)
    string header;
    getline(file, header);

    Process p;
    while (file >> p.pid >> p.arrival_time >> p.burst_time >> p.priority) {
        p.remaining_time = p.burst_time;
        p.completion_time = 0;
        p.turnaround_time = 0;
        p.waiting_time = 0;
        p.response_time = 0;
        p.started = false;
        processes.push_back(p);
    }

    file.close();

    if (processes.empty()) {
        cout << "Error: No processes found in file.\n";
        return false;
    }

    return true;
}

// Timeline is a list of (pid, time_unit) pairs
// pid = 0 means CPU was idle
void printTimeline(const vector<pair<int,int>>& timeline) {
    cout << "\n--- Execution Timeline ---\n";

    // Compress consecutive same-pid entries into ranges for readability
    int i = 0;
    while (i < (int)timeline.size()) {
        int current_pid = timeline[i].second;
        int start_time  = timeline[i].first;
        int end_time    = start_time + 1;

        // Merge consecutive time units of the same process
        while (i + 1 < (int)timeline.size() &&
               timeline[i+1].second == current_pid &&
               timeline[i+1].first == end_time) {
            end_time++;
            i++;
        }

        if (current_pid == 0)
            cout << "  [" << start_time << " - " << end_time << "]  IDLE\n";
        else
            cout << "  [" << start_time << " - " << end_time << "]  P" << current_pid << "\n";

        i++;
    }
}

void printTable(const vector<Process>& processes) {
    cout << "\n--- Process Table ---\n";
    cout << left
         << setw(6)  << "PID"
         << setw(6)  << "AT"
         << setw(6)  << "BT"
         << setw(6)  << "Pri"
         << setw(6)  << "CT"
         << setw(6)  << "TAT"
         << setw(6)  << "WT"
         << setw(6)  << "RT"
         << "\n";

    cout << string(48, '-') << "\n";

    for (const Process& p : processes) {
        cout << left
             << setw(6) << p.pid
             << setw(6) << p.arrival_time
             << setw(6) << p.burst_time
             << setw(6) << p.priority
             << setw(6) << p.completion_time
             << setw(6) << p.turnaround_time
             << setw(6) << p.waiting_time
             << setw(6) << p.response_time
             << "\n";
    }
}

void printAverages(const vector<Process>& processes) {
    double total_wt = 0, total_tat = 0;
    for (const Process& p : processes) {
        total_wt  += p.waiting_time;
        total_tat += p.turnaround_time;
    }
    int n = processes.size();
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time    : " << total_wt  / n << "\n";
    cout << "Average Turnaround Time : " << total_tat / n << "\n";
}

void resetProcesses(vector<Process>& processes) {
    for (Process& p : processes) {
        p.remaining_time  = p.burst_time;
        p.completion_time = 0;
        p.turnaround_time = 0;
        p.waiting_time    = 0;
        p.response_time   = 0;
        p.started         = false;
    }
}
