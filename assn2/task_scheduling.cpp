/** 
 @brief Assignment 2 - Non-Preemptive Task Scheduling Algorithms 
 @author Jasper Charlinski
 @date October 7th, 2023
*/

#include <iostream>
#include <string>
#include <iomanip>
#include "task_scheduling.hpp"

    using namespace std;

/** 
 --------------------------------------------------------------
 @brief Implementation of methods for Task_List
 --------------------------------------------------------------
*/

/// @brief Prints all tasks in current list and their attributes  
void Task_List::print_tasks() {

    if (front == NULL) {
        cout << "No tasks!\n";
        return;
    }
    task *tmp = front;
    cout << "Printing " << num_tasks << " tasks:\n\n";

    while (tmp) {
        cout << "Task PID: " << tmp->pid << "\nCPU Burst: " << tmp->cpu_burst << "\nArrival: " << tmp->arrival << "\nPriority: "  << tmp->priority << endl<<endl;
        tmp = tmp->next;
    }
}

/// @brief Get task attributes from user and store them in input buffer  
void Task_List::get_task() {

    cout << endl << " Enter CPU Burst:" << endl;
    cin >> input[0];
    if (cin.fail() || input[0] < 0) {
        cin.clear();
        cin.ignore(MAX_TASKS, '\n');
        input[0] = 0;
        cerr << " Error: Invalid Input, Burst must be an integer >= 0" << endl;
    }

    cout << " Enter Arrival:" << endl;
    cin >> input[1];
    if (cin.fail() || input[1] < 0) {
        cin.clear();
        cin.ignore(MAX_TASKS, '\n');
        input[1] = 0;
        cerr << " Error: Invalid Input, Arrival must be an integer >= 0" << endl;
    }

    cout << " Enter Priority:" << endl;
    cin >> input[2];
    if (cin.fail() || input[2] > MAX_PRIORITY || input[2] < MIN_PRIORITY) {
        cin.clear();
        cin.ignore(MAX_TASKS, '\n');
        cerr << " Error: Invalid Input, Priority must be an integer in the range" << MAX_PRIORITY << " - " << MIN_PRIORITY << endl;
    }
}

/// @brief Get the number of tasks the user wants to insert 
int Task_List::get_max_tasks() {
    bool asking = true;
    int max;
    while (asking) {
        cout << " Enter the number of tasks to schedule: " << endl;
        cin >> max;
        if (cin.fail() || max <= 0 || max > MAX_TASKS) {
            cin.clear();
            cin.ignore(MAX_TASKS, '\n');
            cerr << "Error: Invalid Number, must be in the range of 0 - 256" << endl;
        } else {
            asking = false;
        }
    }
    return max;
}

/// @brief Reset list to initial values  
void Task_List::initialize() {
    front = NULL;
    back = NULL;
    num_tasks = 0;
}

/// @brief Insert new task into list, inserts task in-order of arrival time, operation has complexity of O(N),
///        must search whole list except front and back task in worst case.
void Task_List::insert_task(task *tmp) {

    if (tmp == NULL) {
        return;
    }

    // first task in list
    if (front == NULL) {
        front = tmp;
        back = front;
        num_tasks++;
        return;
    }

    // insert to back
    if (tmp->arrival >= back->arrival) {
        back->next = tmp;
        tmp->prev = back;
        back = tmp;
        num_tasks++;
        return;
    }

    // insert to front
    if (tmp->arrival < front->arrival) {
        front->prev = tmp;
        tmp->next = front;
        front = tmp;
        num_tasks++;
        return;
    }

    // else insert between two nodes
    task *after = front;
    while((after != NULL) && (after->arrival <= tmp->arrival))
    {
        after = after->next;
    }
    task *before = after->prev;
    before->next = tmp;
    after->prev = tmp;
    tmp->next = after;
    tmp->prev = before;
    num_tasks++;
    return;
}

/// @brief Remove task from list, operation done in O(1), done after task is completed 
bool Task_List::remove_task(Task_List::task *to_remove) {

   // if the task was not found in the list
   if(to_remove == NULL)
   {
      return false;
   }

   // if there is only one task in the list
   if((to_remove->next == NULL) && (to_remove->prev == NULL))
   {
      // Reset list to its original state
      initialize();
      return true;
   }
   // if the task to remove is in the back of the list
   else if (to_remove->next == NULL)
   {
      back = to_remove->prev;
      back->next = NULL;
   }
   // if the task to remove is in the front of the list
   else if (to_remove->prev == NULL)
   {
      front = to_remove->next;
      front->prev = NULL;
   }
   // if the task is in between two other nodes
   else
   {
      task *before = to_remove->prev;
      task *after = to_remove->next;
      before->next = after;
      after->prev = before;
   }
   // subtract one from node count
   num_tasks--;
   delete to_remove;
   return true;
}

/// @brief Dynamically allocate memory for task and fill attributes. 
Task_List::task *Task_List::create_task(int cpu_burst, int arrival, int priority)
 {
    task *new_task = new task;
    new_task->pid = num_tasks + 1;
    new_task->cpu_burst = cpu_burst;
    new_task->arrival = arrival;
    new_task->priority = priority;
    new_task->next = NULL;
    new_task->prev = NULL;
    return new_task;
}

/// @brief Continue to prompt user for tasks and insert them until maximum number of tasks is reached to the user ends the loop by entering 0 
void Task_List::fill_tasks()
{
    bool getting_tasks = true;
    while(getting_tasks && num_tasks <= max_tasks) {
        get_task();
        task *tmp = create_task(input[0], input[1], input[2]);
        insert_task(tmp);

        cout << "\n\n ------------------------ \n Again? Yes: 1 or No: 0" << endl;
        cin >> getting_tasks;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(MAX_TASKS, '\n');
            cerr << "Error: Invalid Input" << endl;
        }
    }
 }

/** 
 --------------------------------------------------------------
 @brief Implementation of methods for First_Come_First_Serve
 --------------------------------------------------------------
*/
void First_Come_First_Serve::first_come_first_serve() {

    if (TL.front == NULL) {
        cout << "No tasks!\n";
        return;
    }
    Task_List::task *tmp = TL.front;
    Task_List::task *to_remove;

    int current_time = 0;

    // traverse the list in order 
    while(tmp) {
        cout << " PID: " << tmp->pid << right << setw(10) << "Burst: " << tmp->cpu_burst << setw(13) << "Arrival: " << tmp->arrival << " |\n";
        // update arrival time if the is a gap between tasks arrival and current time
        if (tmp->arrival > current_time) {
            current_time = tmp->arrival;
        }
        cout << " " << current_time << right << setw(32) << current_time + tmp->cpu_burst << endl<<endl;
        current_time += tmp->cpu_burst;
        // remove current task from list
        to_remove = tmp;
        tmp = tmp->next;
        TL.remove_task(to_remove);
    }
}

/** 
 --------------------------------------------------------------
 @brief Implementation of methods for Round_Robbin
 --------------------------------------------------------------
*/

/// @brief Gets time quantum from user
/// @return Integer in the range of 0 - max_quantum
int Round_Robbin::get_time_quantum() {
    bool asking = true;
    int time_quantum;
    while (asking) {
        cout << " Enter the number of tasks to schedule: " << endl;
        cin >> time_quantum;
        if (cin.fail() || time_quantum <= 0 || time_quantum > max_quantum) {
            cin.clear();
            cin.ignore(MAX_TASKS, '\n');
            cerr << "Error: Invalid Number, must be in the range of 0 - " << max_quantum << endl;
        } else {
            asking = false;
        }
    }
    return time_quantum;
}

void Round_Robbin::round_robbin() {

    if (TL.front == NULL) {
        cout << "No tasks!\n";
        return;
    }

    Task_List::task *tmp = TL.front;
    Task_List::task *to_remove = NULL;

    int current_time = 0;
    int tmp_time;

    // continue to run as long as there are still tasks in the schedule, traverse the list in order
    while(TL.num_tasks != 0) {     

        tmp_time = current_time;

        cout << " PID: " << tmp->pid << right << setw(10) << "Burst: " << tmp->cpu_burst << setw(13) << "Arrival: " << tmp->arrival << " |\n";

        current_time += time_quantum;
        tmp->cpu_burst -= time_quantum;

        // if task burst time is less than the time_quantum, adjust the current time for the difference 
        if (tmp->cpu_burst <= 0) {
            current_time += tmp->cpu_burst;
        }

        cout << " " << tmp_time << right << setw(32) << current_time << endl<<endl;

        // if task has completed, prepare to remove task 
        if (tmp->cpu_burst <= 0) {
            to_remove = tmp;
        }

        // if end of list has been reached, start from front again
        if (tmp->next == NULL) {
            tmp = TL.front;
        }
        else {
            tmp = tmp->next;
        }

        // remove the completed task
        if (to_remove != NULL) {
            TL.remove_task(to_remove);
            to_remove = NULL; // reset to_remove for the next iteration
        }
    }
}

/** 
 --------------------------------------------------------------
 @brief Implementation of methods for Priority
 --------------------------------------------------------------
*/

/// @brief Finds highest priority (lowest integer in the range -20 - 19) in the tasks, time complexity O(N)
/// @return Integer in the range -20 - 19 representing task priority 
int Priority::find_highest_priority() {

    Task_List::task *tmp = TL.front;
    int highest = MAX_PRIORITY;

    while (tmp) {
        if (tmp->priority < highest) {
            highest = tmp->priority;
        }
        tmp = tmp->next;
    } 
    return highest;
}

void Priority::priority() {
    Task_List:: task *tmp = TL.front;
    Task_List::task *to_remove;
    int highest;
    int current_time = 0;
    highest = find_highest_priority();
    // continue to run as long as there are still tasks in the schedule, traverse the list in order
    while (TL.num_tasks != 0) {

        // if highest priority task has been found 
        if (tmp->priority == highest ) {
            cout << " PID: " << tmp->pid << right << setw(10) << "Burst: " << tmp->cpu_burst << setw(13) << "Arrival: " << tmp->arrival << setw(13) << "Priority: "<< tmp->priority <<" |\n";
            cout << " " << current_time << right << setw(47) << current_time + tmp->cpu_burst << endl<<endl;
            current_time += tmp->cpu_burst;
            to_remove = tmp;
            if (tmp->next == NULL) {
                tmp = TL.front;
            } else {
                tmp = tmp->next;
            }
            // remove task and find next highest priority 
            TL.remove_task(to_remove);
            highest = find_highest_priority();
        }
        else {
            if (tmp->next == NULL) {
                tmp = TL.front;
            } else {
                tmp = tmp->next;
            }
        }
    } 
}

/** 
 --------------------------------------------------------------
 @brief Implementation of methods for Shortest_Job_First
 --------------------------------------------------------------
*/
/// @brief Finds highest priority (lowest integer in the range -20 - 19) in the tasks, time complexity O(N)
/// @return Integer representing task burst time
int Shortest_Job_First::find_shortest_task() {
    Task_List::task *tmp = TL.front;
    int shortest = MAX_TASKS;
    while(tmp) {
        if (tmp->cpu_burst < shortest) {
            shortest = tmp->cpu_burst;
        }
        tmp = tmp->next;
    }
    return shortest;
}

void Shortest_Job_First::shortest_job_first() {
    Task_List::task *tmp = TL.front;
    Task_List::task *to_remove;
    int shortest;
    int current_time = 0;
    shortest = find_shortest_task();
    // continue to run as long as there are still tasks in the schedule, traverse the list in order
    while (TL.num_tasks != 0) {
        // if shortest task has been found 
        if (tmp->cpu_burst == shortest ) {
            cout << " PID: " << tmp->pid << right << setw(10) << "Burst: " << tmp->cpu_burst << setw(13) << "Arrival: " << tmp->arrival << setw(13) << "Priority: "<< tmp->priority <<" |\n";
            cout << " " << current_time << right << setw(45) << current_time + tmp->cpu_burst << endl<<endl;
            current_time += tmp->cpu_burst;
            to_remove = tmp;
            if (tmp->next == NULL) {
                tmp = TL.front;
            } else {
                tmp = tmp->next;
            }
            // remove task and find next shortest 
            TL.remove_task(to_remove);
            shortest = find_shortest_task();
        }
        else {
            if (tmp->next == NULL) {
                tmp = TL.front;
            } else {
                tmp = tmp->next;
            }
        }
    }
}
