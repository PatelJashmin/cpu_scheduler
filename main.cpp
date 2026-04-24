#include <iostream>
#include <vector>
#include <string>
#include "process.h"
#include "scheduler.h"
#include "utils.h"

using namespace std;

// Forward declarations for factory functions defined in each .cpp file
Scheduler* createFCFS();
Scheduler* createSJF();
Scheduler* createSRTF();
Scheduler* createRR(int quantum);
Scheduler* createPriority();

int main() {
    cout << "========================================\n";
    cout << "       CPU Scheduling Simulator\n";
    cout << "========================================\n\n";

    // ---- Step 1: Get process data ----
    vector<Process> processes;

    cout << "Input Mode:\n";
    cout << "  1. Manual (keyboard)\n";
    cout << "  2. File input\n";
    cout << "Choose: ";

    int input_choice;
    cin >> input_choice;

    if (input_choice == 1) {
        readFromCLI(processes);
    } else if (input_choice == 2) {
        string filename;
        cout << "Enter filename (e.g., processes.txt): ";
        cin >> filename;
        if (!readFromFile(processes, filename)) {
            cout << "Failed to read file. Exiting.\n";
            return 1;
        }
        cout << "Loaded " << processes.size() << " processes from file.\n";
    } else {
        cout << "Invalid choice. Exiting.\n";
        return 1;
    }

    if (processes.empty()) {
        cout << "No processes to schedule. Exiting.\n";
        return 1;
    }

    // ---- Step 2: Choose algorithm ----
    cout << "\nScheduling Algorithm:\n";
    cout << "  1. FCFS      (First Come First Serve)\n";
    cout << "  2. SJF       (Shortest Job First, non-preemptive)\n";
    cout << "  3. SRTF      (Shortest Remaining Time First, preemptive)\n";
    cout << "  4. Round Robin\n";
    cout << "  5. Priority  (non-preemptive)\n";
    cout << "Choose: ";

    int algo_choice;
    cin >> algo_choice;

    Scheduler* scheduler = nullptr;

    switch (algo_choice) {
        case 1:
            scheduler = createFCFS();
            break;
        case 2:
            scheduler = createSJF();
            break;
        case 3:
            scheduler = createSRTF();
            break;
        case 4: {
            int quantum;
            cout << "Enter time quantum for Round Robin: ";
            cin >> quantum;
            if (quantum <= 0) {
                cout << "Quantum must be > 0. Using 1.\n";
                quantum = 1;
            }
            scheduler = createRR(quantum);
            break;
        }
        case 5:
            scheduler = createPriority();
            break;
        default:
            cout << "Invalid choice. Exiting.\n";
            return 1;
    }

    // ---- Step 3: Run simulation ----
    cout << "\n========================================\n";
    cout << "Algorithm : " << scheduler->name() << "\n";
    cout << "========================================\n";

    scheduler->run(processes);

    // ---- Cleanup ----
    delete scheduler;

    cout << "\n========================================\n";
    cout << "         Simulation Complete\n";
    cout << "========================================\n";

    return 0;
}
