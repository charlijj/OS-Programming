#include<pthread.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "assign4.h"

int NUM_CHAIRS; // number of chairs available in clinic
int MAX_PATIENTS; // number of patients that will enter the clinic

int num_patients = 0; // number if patients currently in the clinic

sem_t patients; // semaphore to track number of patients 
sem_t patient_count; // mutex for updating num_patients

/// @brief Generates time it takes for doctor to investigate a patient and sleeps for that amount of time.
/// @param seed Random number generation seed.
void investigate(unsigned int *seed) {
    int min = 1; // variables to set range of random number generation
    int max = 3;
    int sleep_amount;

    sleep_amount = (rand_r(seed) % (max - min + 1)) + min;
    // printf("\n Doctor Investigates For %d Seconds \n\n", sleep_amount);
    sleep(sleep_amount);
}

/// @brief Doctor thread start routine, while there are still patient entering the clinic, investigate the patient and decrement the patient count.
/// @param arg Not used.
void *doctor_function(void *arg) {
    unsigned int seed;  
    while(1) {  
        printf(" Doctor Is Beginning To Investigate Patient...\n");    
        sem_wait(&patients); // if there are no patients, the doctor sleeps (waits for patients)
        sem_wait(&patient_count); // acquire lock to updated patient count
        num_patients--;
        sem_post(&patient_count); // release lock on patient count

        seed = (unsigned int)time(NULL); // get seed for rand from timestamp
        investigate(&seed);
        printf(" Doctor Has Finished Investigating...\n\n");
        printf(" Available Chairs: %d \n\n", NUM_CHAIRS - num_patients);  
        if (num_patients == 0) {
            break;
        }  
    }

    printf("\n\n No More Patients, Clinic is Closed...\n\n");    
    pthread_exit(NULL);
}

/// @brief Patient thread start routine, if there are chairs available take a seat and increment patient count, if not leave clinic.
/// @param arg Index of patient thread array, used for patient ID.
void *patient_function(void *arg) {
    int patientID = (int)(long)arg;
    sem_wait(&patient_count); // acquire lock to updated patient count
    if (num_patients < NUM_CHAIRS) {
        num_patients++;
        printf(" Patient %d Has Entered The Waiting Room And Taken A Seat...\n\n", patientID + 1);  
        sem_post(&patients); // signal doctor a patient has entered
        sem_post(&patient_count); // release lock on patient count
    } 
    // all chairs are full so leave
    else {
        sem_post(&patient_count); // release lock on patient count
        printf(" Patient %d Has Entered The Waiting, They Could Not Find A Seat So They Leave...\n\n", patientID + 1); 
    }

    pthread_exit(NULL);
}

/// @brief Gets number of chairs and patients from user.
/// @param arg1 Reference to NUM_CHAIRS global.
/// @param arg2 Reference to MAX_PATIENTS global.
void get_input(int *arg1, int *arg2) {
    printf("\n\n");
    printf(" Enter number of chairs: \n");
    scanf("%d", arg1);
    printf(" Enter number of patients \n\n");
    scanf("%d", arg2);
}

int main()
{   
    sem_init(&patient_count, 0, 1);
    sem_init(&patients, 0, MAX_PATIENTS);

    get_input(&NUM_CHAIRS, &MAX_PATIENTS);

    printf(" Max Chairs: %i       Max Patients: %i \n\n", NUM_CHAIRS, MAX_PATIENTS);

    pthread_t patient_thread[MAX_PATIENTS];
    pthread_t doctor_thread;
    printf("\n\n !! Clinic Opens !! \n\n");

    pthread_create(&doctor_thread, NULL, doctor_function, NULL);

    for (int i = 0; i < MAX_PATIENTS; i++) {
        pthread_create(&patient_thread[i], NULL, patient_function, (void *)(long)i);
        usleep(250000); // sleep for 0.25 seconds
    }

    pthread_join(doctor_thread, NULL);

    for (int i = 0; i < MAX_PATIENTS; i++) {
        pthread_join(patient_thread[i], NULL);
    }

    printf("\n\n !! Clinic Closes !! \n\n");

    sem_destroy(&patient_count);
    sem_destroy(&patients);
    return 1;
}