# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "Scheduler.h"

void bubbleSort(struct Process* ProcessList, int n) 
{ 
   int i, j; 
    char T_pname[10];
    int  T_pid;
    int  T_duration;
    int  T_arrivaltime;
    for (i = 0; i < n-1; i++) 
    {      
        for (j = 0; j < n-i-1; j++)  
        {
            if ((ProcessList+j)->arrivaltime > (ProcessList+j+1)->arrivaltime) 
            {
                strcpy(T_pname,(ProcessList+j)->pname);
                strcpy((ProcessList+j)->pname,(ProcessList+j+1)->pname);
                strcpy((ProcessList+j+1)->pname,T_pname);

                T_duration = (ProcessList+j)->duration;
                (ProcessList+j)->duration = (ProcessList+j+1)->duration;
                (ProcessList+j+1)->duration = T_duration;

                T_pid = (ProcessList+j)->pid;
                (ProcessList+j)->pid = (ProcessList+j+1)->pid;
                (ProcessList+j+1)->pid = T_pid;
                T_arrivaltime = (ProcessList+j)->arrivaltime;
                (ProcessList+j)->arrivaltime = (ProcessList+j+1)->arrivaltime;
                (ProcessList+j+1)->arrivaltime = T_arrivaltime;
            }
        }
    }
} 

void enqueue (struct node ** headaddr,struct Process* ProcessNode, int priority){

    if (headaddr==NULL){
        fprintf(stderr, "NULL ptr passed\n"); exit(1);
    }

    struct node * n = malloc(sizeof(struct node));
 

    if (n==NULL){
        fprintf(stderr,"memory allocation failed\n");exit(1);
    }
    n->ProcessNode = ProcessNode;
    n->next = NULL;
    n->priority = priority;


    if (*headaddr == NULL){
        *headaddr = n;
    }
    else {


        if (priority<(*headaddr)->priority)
        {
            n->next= *headaddr;
            *headaddr = n;
        }
        else
        {

            struct node* tmp = * headaddr;
            while (tmp->next != NULL && tmp->next->priority<= priority)
            {
                tmp = tmp -> next;
            }

            n->next = tmp->next;
            tmp-> next = n;
        }
    }
}

void add_last(struct node ** headaddr,struct Process* ProcessNode){

    if (headaddr==NULL){
        fprintf(stderr, "NULL ptr passed\n"); exit(1);
    }

    struct node * n = malloc(sizeof(struct Process));
 

    if (n==NULL){
        fprintf(stderr,"memory allocation failed\n");exit(1);
    }
    n->ProcessNode = ProcessNode;
    n->next = NULL;


    if (*headaddr == NULL){ // empty list
        *headaddr = n;
        n->priority = n->ProcessNode->arrivaltime;
    }
    else {
        // get to tail
        struct node* tmp = * headaddr;
        while (tmp->next != NULL)
        {
            tmp = tmp -> next;
        }
        tmp -> next = n;
        n->priority = tmp->next->priority + 1;
    }
}

struct  Process* dequeue (struct node ** headaddr){


    if (headaddr==NULL){
        fprintf(stderr, "NULL ptr passed\n"); exit(1);
    }

    if (*headaddr == NULL){ // empty list
        struct Process *ProcessNode = malloc(sizeof(struct Process));
        ProcessNode->pid = -1;
        return ProcessNode;
    }

    else
    {
        //struct Process  * ProcessNode = malloc(sizeof(struct Process));
        struct node *n = *headaddr;
        *headaddr = (*headaddr)->next;
        struct Process * ProcessNode = (n-> ProcessNode);
        free(n);
        return ProcessNode;
    }
    
}

int print (struct node * head)
{
    if (head == NULL)
    {
        fprintf(stdout,"empty:\n");
        return 0;
    }
    else
    {
        while(head!=NULL)
        {
            fprintf(stdout,"%s(%d),",head->ProcessNode->pname,head->ProcessNode->duration);
           
            head = head ->next;
        }
        fprintf(stdout,":\n");
        
    }
    return 1;
    
}
