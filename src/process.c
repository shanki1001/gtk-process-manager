#include<stdio.h>
#include<string.h>
#include "process.h"

Process* create_process(const char *name, int memoryUsage){
    static int next_pid = 1;
    Process *new_process = (Process*)malloc(sizeof(Process));
    if(!new_process) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    new_process->pid = next_pid++;
    strncpy(new_process->name, name, sizeof(new_process->name) -1);
    new_process->name[sizeof(new_process->name)-1] = '\0';
    new_process->memoryUsage = memoryUsage;
    new_process->status = RUNNING;
    new_process->next = NULL;

    return new_process;

}
void add_process(Process **head, Process *newProcess){
    if(*head == NULL){
        *head = newProcess;
    }
    else{
        Process *temp= *head;
        while(temp->next!=NULL){
            temp= temp->next;
        }
        temp->next = newProcess;
    }
}

void terminate_process(Process *head, int pid){
    while(head!=NULL){
        if(head->pid==pid){
            head->status = TERMINATED;
            printf("Process %d terminated.\n",pid);
            return;
        }
        head = head->next;
    }
    printf("Process with PID %d not found.\n",pid);
}
void free_process_list(Process *head){

    Process *temp;
    while(head!=NULL){
        temp = head;
        head = head->next;
        free(temp);
    }

}


