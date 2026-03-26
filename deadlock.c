#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 10

int wfg[MAX_NODES][MAX_NODES];
int visited[MAX_NODES];
int rec_stack[MAX_NODES];
int num_nodes;

void init_coordinator(int nodes) {
    num_nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        visited[i] = 0;
        rec_stack[i] = 0;
        for (int j = 0; j < nodes; j++) {
            wfg[i][j] = 0;
        }
    }
}

void add_wait_for(int p1, int p2) {
    wfg[p1][p2] = 1;
    printf("Coordinator received: Process %d is waiting for Process %d\n", p1, p2);
}

int detect_cycle_util(int v) {
    if (visited[v] == 0) {
        visited[v] = 1;
        rec_stack[v] = 1;

        for (int i = 0; i < num_nodes; i++) {
            if (wfg[v][i]) {
                if (!visited[i] && detect_cycle_util(i)) {
                    return 1;
                } else if (rec_stack[i]) {
                    return 1;
                }
            }
        }
    }
    rec_stack[v] = 0;
    return 0;
}

void check_deadlock() {
    for (int i = 0; i < num_nodes; i++) {
        visited[i] = 0;
        rec_stack[i] = 0;
    }

    for (int i = 0; i < num_nodes; i++) {
        if (!visited[i]) {
            if (detect_cycle_util(i)) {
                printf("\n>>> RESULT: Deadlock Detected! Cycle found in Global WFG.\n");
                return;
            }
        }
    }
    printf("\n>>> RESULT: No Deadlock Detected. System is safe.\n");
}

int main() {
    printf("--- Centralized Deadlock Detection Simulation ---\n\n");
    
    init_coordinator(4); 
    
    add_wait_for(0, 1);
    add_wait_for(1, 2);
    check_deadlock(); 
    
    printf("\nNew request arrives...\n");
    add_wait_for(2, 0); 
    check_deadlock();   
    
    return 0;
}