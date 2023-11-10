// Assignment 2 - Non-Preemptive Task Scheduling Algorithms 

/// @author Jasper Charlinski
/// @date October 7th, 2023

#include <iostream>
#include <cctype>
#include "task_scheduling.hpp"

    using namespace std;

void print_commands() {
    cout << " \n\n Enter A Command: " << endl;
    cout << " ------------------------------" << endl;
    cout << " Run First Come First Serve: F / f" << endl;
    cout << " Run Round Robbin: R / r" << endl;
    cout << " Run Priority: P / p" << endl;
    cout << " Run Shortest Job First: S / s" << endl;
    cout << " Quit: Q / q" << endl;
    cout << " ------------------------------" << endl<<endl;
}

void print_credits() {
    cout << " \n\n -------------------------------------------------------------" << endl;
    cout << " Assignment 2 - Non-Preemptive Task Scheduling Algorithms " << endl;
    cout << " By: Jasper Charlinski" << endl;
    cout << " For: CSCI 360 Intro to Operating Systems" << endl;
    cout << " Date: October 7th, 2023" << endl;
    cout << " -------------------------------------------------------------" << endl<<endl;
}

int main () {

    print_credits();

    Task_List TL;

    cout << " \n\n-- Fill Schedule With Tasks -- " << endl;
    cout << " ------------------------------" << endl<<endl;
    TL.fill_tasks();

    bool running = true;
    char cmd = '\0';

    while (running) {
        // make deep copy of original task list for all of the scheduling algorithms
        // realistically should be defined outside of while loop, but if you want to do a algorithm more than once must re-initialize it
        First_Come_First_Serve FCFS(TL);
        Round_Robbin RR(TL);
        Priority P(TL);
        Shortest_Job_First SJF(TL);
        print_commands();
        cin >> cmd;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(MAX_TASKS, '\n');
            cerr << " Error: Invalid Input" << endl;
        } else {
            cmd = toupper(cmd);
            switch (cmd)
            {
            case 'F':
                FCFS.run();
                break;
            case 'R':
                RR.run();
                break;
            case 'P':
                P.run();
                break;
            case 'S':
                SJF.run();
                break;
            case 'Q':
                cout << " Quitting..." << endl << endl;
                running = false;
                break;
            default:
                cerr << " Error: Invalid Option" << endl;
                break;
            }
        }
    }
    return 0;
}