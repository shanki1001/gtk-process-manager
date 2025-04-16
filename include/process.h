#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include<stdlib.h>

typedef enum {
    RUNNING,
    WAITING,
    TERMINATED

} ProcessStatus;

typedef struct Process {
    int pid;
    char name[50];
    int memoryUsage;
    ProcessStatus status;
    struct Process *next;
} Process;

Process* create_process(const char *name, int memoryUsage);
void add_process(Process **head, Process *newProcess);
void terminate_process(Process *head, int pid);
void free_process_list(Process *head);

#endif // PROCESS_H_INCLUDED
