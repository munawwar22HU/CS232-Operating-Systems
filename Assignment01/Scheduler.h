#ifndef SCHEDULER_H
#define SCHEDULER_H

struct Process{

    char pname[10];
    int  pid;
    int  duration;
    int  arrivaltime;
    int  time_spent_running;
};



struct node{
    struct Process* ProcessNode;
    struct node * next;
    int priority;
};


void bubbleSort(struct Process * ProcessList, int n);
void enqueue (struct node ** headaddr, struct Process* ProcessNode, int priority);
void add_last (struct node ** headaddr,struct Process* ProcessNode);
struct Process* dequeue(struct node ** headaddr);
int print (struct node * head);

#endif