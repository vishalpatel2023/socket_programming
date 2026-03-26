#include <stdio.h>

#define MAX 10

int num_processes;
int state[MAX]; 
int timestamps[MAX];
int deferred_replies[MAX][MAX];

void init() {
    for (int i = 0; i < num_processes; i++) {
        state[i] = 0; 
        timestamps[i] = 0;
        for (int j = 0; j < num_processes; j++) {
            deferred_replies[i][j] = 0;
        }
    }
}

void request_cs(int pid, int ts) {
    state[pid] = 1;
    timestamps[pid] = ts;
    printf("\nProcess %d requests CS with Timestamp %d\n", pid, ts);
    
    int replies_received = 0;
    
    for (int i = 0; i < num_processes; i++) {
        if (i != pid) {
            if (state[i] == 0 || (state[i] == 1 && (ts < timestamps[i] || (ts == timestamps[i] && pid < i)))) {
                printf("Process %d sends REPLY to Process %d\n", i, pid);
                replies_received++;
            } else {
                printf("Process %d DEFERS reply to Process %d\n", i, pid);
                deferred_replies[i][pid] = 1;
            }
        }
    }
    
    if (replies_received == num_processes - 1) {
        state[pid] = 2;
        printf("--> Process %d ENTERED Critical Section.\n", pid);
    } else {
        printf("--> Process %d is WAITING for deferred replies.\n", pid);
    }
}

void release_cs(int pid) {
    state[pid] = 0;
    printf("\nProcess %d RELEASES Critical Section.\n", pid);
    
    for (int i = 0; i < num_processes; i++) {
        if (deferred_replies[pid][i] == 1) {
            deferred_replies[pid][i] = 0;
            printf("Process %d sends deferred REPLY to Process %d\n", pid, i);
        }
    }
}

int main() {
    num_processes = 3;
    init();
    
    printf("--- Ricart-Agrawala Algorithm Simulation ---\n");
    request_cs(0, 15);
    request_cs(1, 10); 
    
    release_cs(1); 
    
    return 0;
}