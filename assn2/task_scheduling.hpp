// Assignment 2 - Non-Preemptive Task Scheduling Algorithms 

/// @author Jasper Charlinski
/// @date October 7th, 2023

#pragma once
#include <iostream>
#include <memory>

// variable used to clear input from stream on cin error and maximum number of tasks allowed in the schedule
const int MAX_TASKS = 256;
const int MAX_PRIORITY = 19;
const int MIN_PRIORITY = -20;

void print_commands();

 using namespace std;

/**
 * @brief Doubly linked list, with in-order insert with O(N) complexly, 
*/
class Task_List {

    public:
        // number of attributes given by the user
        int num_attributes = 3;
        // buffer to store user input
        int input[3];
        // current number of tasks
        int num_tasks = 0;
        // maximum number of allowed tasks, given by the user
        int max_tasks;

        struct task
        {
            int pid; // incremented from 1
            int cpu_burst;
            int arrival;
            int priority;
            task* next;
            task* prev;
        };

        task *front = NULL;
        task *back = NULL;

        Task_List() {
            initialize();
            max_tasks = get_max_tasks();
            return;
        };

        // copy constructor to make deep copy of another Task_List
        Task_List(const Task_List& orig_task_list) : num_attributes(orig_task_list.num_attributes), num_tasks(orig_task_list.num_tasks) {
            initialize(); // Initialize the new Task_List
            task* tmp = orig_task_list.front;
            while (tmp) {
                task* newTask = create_task(tmp->cpu_burst, tmp->arrival, tmp->priority);
                insert_task(newTask);
                tmp = tmp->next;
            }
            return;
         }

        // deallocate list 
        ~Task_List() {
            if (front == NULL && back == NULL) {
                return;
            } 

            task *tmp = front;
            task *to_delete;

            while (tmp) {
                to_delete = tmp;
                tmp = tmp->next;
                delete to_delete;
            }
            return;
        }

        void initialize();
        void insert_task(task *tmp);
        bool remove_task(task *to_remove);
        // allocate memory for new task
        task *create_task(int cpu_burst, int arrival, int priority);
        // get task attributes from user
        void get_task();
        int get_max_tasks();
        void print_tasks();
        void print_num_tasks() {
            cout << "Number of tasks remaining: " << num_tasks << endl;
        }

        void fill_tasks();
};

class  First_Come_First_Serve
{
    private:
        // holds the copy of the original Task List
        Task_List TL; 

    public:
        /// @brief Uses copy constructor of TL to copy a reference of the original task list into TL, making a deep copy.
        ///        The same approach is done in all of the algorithms.
        /// @param INIT_TL Reference to original task list 
        First_Come_First_Serve(const Task_List& INIT_TL) : TL(INIT_TL) {}
        ~First_Come_First_Serve(){}

        void run() {
            cout << "\n\n -- First Come First Serve -- \n\n";
            first_come_first_serve();
        }
        void first_come_first_serve();
};


class  Round_Robbin 
{
    private:
        Task_List TL; 
        // holds user provided time quantum
        int time_quantum; 
        // this can be changed to test for a longer or shorter time quantum
        const int max_quantum = 16;
    
        int get_time_quantum();
    public:
        Round_Robbin(const Task_List& INIT_TL) : TL(INIT_TL){}
        ~Round_Robbin(){}
        void round_robbin();
        void run() {
            cout << "\n\n -- Round Robbin --\n\nEnter time quantum\n";
            time_quantum = get_time_quantum();
            cout << "\n\n Time Quantum: " << time_quantum << endl << endl;
            round_robbin();
        }
        
};


class Priority 
{
    private:
        Task_List TL;
        
        int find_highest_priority();

    public:
        Priority(const Task_List& INIT_TL) : TL(INIT_TL) {}
        ~Priority(){}
        void priority();
        void run() {
            cout << "\n\n -- Priority -- \n\n";
            priority();
        }
        
};

class  Shortest_Job_First 
{
    private:
        Task_List TL;
        int find_shortest_task();

    public:
        Shortest_Job_First(const Task_List& INIT_TL) : TL(INIT_TL) {}
        ~Shortest_Job_First(){}
        void shortest_job_first();
        void run() {
            cout << "\n\n -- Shortest Job First -- \n\n";
            shortest_job_first();
        }
        
};