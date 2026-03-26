#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

int queues[MAX_PROCESSES][MAX_PROCESSES];
int timestamps[MAX_PROCESSES];
int num_processes;

void init() {
    for (int i = 0; i < num_processes; i++) {
        timestamps[i] = 0;
        for (int j = 0; j < num_processes; j++) {
            queues[i][j] = 9999; 
        }
    }
}

void request_cs(int pid, int timestamp) {
    timestamps[pid] = timestamp;
    for (int i = 0; i < num_processes; i++) {
        queues[i][pid] = timestamp; 
        if (i != pid) {
            printf("Message: Process %d sent REQUEST to Process %d with TS %d\n", pid, i, timestamp);
            printf("Message: Process %d sent REPLY to Process %d\n", i, pid);
        }
    }
}

void release_cs(int pid) {
    for (int i = 0; i < num_processes; i++) {
        queues[i][pid] = 9999; 
        if (i != pid) {
            printf("Message: Process %d sent RELEASE to Process %d\n", pid, i);
        }
    }
}

void check_cs() {
    for (int i = 0; i < num_processes; i++) {
        int min_ts = 9999;
        int min_pid = -1;
        
        for (int j = 0; j < num_processes; j++) {
            if (queues[i][j] < min_ts || (queues[i][j] == min_ts && j < min_pid)) {
                min_ts = queues[i][j];
                min_pid = j;
            }
        }
        
        if (min_pid == i && min_ts != 9999) {
            printf("--> Process %d can ENTER the Critical Section.\n", i);
        }
    }
}

int main() {
    num_processes = 3;
    init();
    
    printf("--- Lamport Algorithm Simulation ---\n");
    request_cs(0, 10);
    request_cs(1, 8); 
    
    printf("\nChecking who enters CS first based on local queues:\n");
    check_cs();
    
    printf("\nProcess 1 executes and releases CS:\n");
    release_cs(1);
    check_cs();
    
    return 0;
}