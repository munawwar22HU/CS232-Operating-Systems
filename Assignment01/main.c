# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "Scheduler.h"

int main (int argc, char* argv[]){
    char fifo[5] = "FIFO";
    char sjf[4] = "SJF";
    char stcf[5] = "STCF";
    char rr[3] = "RR";
    if ((argc-1)<2)
    {
        fprintf(stderr,"Error. Usage: ./mysched filename POLICY \nwhere POLICY can be one of the following strings:\nFIFO\nSJF\nSTCF\nRR\n");
        exit(1);
    }
    else
    {
        if(strcmp(argv[2],fifo)!=0 && strcmp(argv[2],sjf)!=0 && strcmp(argv[2],stcf)!=0 && strcmp(argv[2],rr)!=0  )
        {
            fprintf(stderr,"Error. Usage: ./mysched filename POLICY \nwhere POLICY can be one of the following strings:\nFIFO\nSJF\nSTCF\nRR\n");
            exit(1);
        }
    }
    

    FILE * stream = fopen(argv[1],"r");
    int NumProcesses = 0;
    int x = 0;

    struct Process * ProcessList = NULL;
    char line[1024];
    char pname[10];
    int  pid;
    int  duration;
    int  arrivaltime;
    while (fscanf(stream,"%s",line)!=EOF){

        if (ProcessList == NULL){
            NumProcesses = NumProcesses+1;
            ProcessList = (struct Process*) malloc(NumProcesses * sizeof(struct Process));
        }
        else if (ProcessList != NULL)
        {
            NumProcesses = NumProcesses+1;
            ProcessList = realloc(ProcessList,NumProcesses * sizeof(struct Process));
        }
        char * token = strtok(line,":");
        strcpy((ProcessList+x)->pname,token);
        token = strtok(NULL,":");
        pid = atoi(token);
        (ProcessList+x)->pid = pid;
        token = strtok(NULL,":");
        duration = atoi(token);
        (ProcessList+x)->duration = duration;
        token = strtok(NULL,":");
        arrivaltime = atoi(token);
        (ProcessList+x)->arrivaltime = arrivaltime;
        (ProcessList+x)->time_spent_running = 0;
        x++;
    }

    bubbleSort(ProcessList,NumProcesses);
    
   
    struct node * head = NULL;
    int timer = 1;
    int index = 0;
    struct Process* Running = NULL;
    int removal_time = 0;
    int exec_time = 0;
    int pc = 0;
    if (strcmp(argv[2],fifo)==0)
    {

        struct  process * temp = NULL;
      
        
        while (1){
            printf("%d:",timer);
            if (Running != NULL) // A process is currently executing
            {
                printf("%s:",Running->pname);
                print(head);
                if (removal_time == timer) // Execution of the Current Process has completed
                {
                    Running = dequeue(&head);   // Dequeue the process from the Ready Queue
                    if (Running->pid == -1)
                        break;
                    removal_time = timer+ Running->duration;
                }

                for (int index = pc; index<NumProcesses;index++)
                {
                        if( (ProcessList+index)->arrivaltime == timer)
                        {
                            enqueue(&head,(ProcessList+index),(ProcessList+index)->arrivaltime);   // Add the Next Process to the Ready Queue
                            pc ++;
                            break;
                        }
                }
            }
            else
            {
                printf("idle:");
                print(head);
                for (int index = pc; index<NumProcesses;index++)
                    {
                        if( (ProcessList+index)->arrivaltime == timer)
                        {
                            enqueue(&head,(ProcessList+index),(ProcessList+index)->arrivaltime);   // Add the Next Process to the Ready Queue
                            pc++;
                            Running = dequeue(&head);
                            removal_time = timer+ Running->duration;
                            break;
                        }
                }
            }
            timer++;
        }
        free(Running);    
    }
    if (strcmp(argv[2],sjf)==0)
    {
        
        while (1){
            printf("%d:",timer);
            if (Running != NULL)
            {
                printf("%s:",Running->pname);
                print(head);
                if (removal_time == timer)  // Execution of the Running Process has completed
                {

                    Running = dequeue(&head);   // Dequeue the process
                    if (Running->pid == -1)
                        break;
                    removal_time = timer+ Running->duration;
                }

                for (int index = pc; index<NumProcesses;index++)
                    {
                        if( (ProcessList+index)->arrivaltime == timer)
                        {
                            enqueue(&head,(ProcessList+index),(ProcessList+index)->duration);   // Add the Next Process to the Ready Queue
                            pc ++;
                            break;
                        }
                }

            }
            else
            {
                printf("idle:");
                print(head);
                for (int index = pc; index<NumProcesses;index++)
                    {
                        if( (ProcessList+index)->arrivaltime == timer)
                        {
                            enqueue(&head,(ProcessList+index),(ProcessList+index)->duration);   // Add the Next Process to the Ready Queue
                            pc++;
                            Running = dequeue(&head);
                            removal_time = timer+ Running->duration;
                            break;
                        }
                }
            }
            timer ++;
        }
        free(Running);  
    }
    if (strcmp(argv[2],stcf)==0)
    {
        int count1 =0;

        while (1)
        {
            printf("%d:",timer);
            if (Running==NULL)
            {
                printf("idle:");
                print(head);
                for (int index = 0; index<NumProcesses;index++)
                {
                    if( (ProcessList+index)->arrivaltime == timer)
                    {
                        enqueue(&head,(ProcessList+index),(ProcessList+index)->duration);
                        Running = dequeue(&head);
                        removal_time = timer+ Running->duration;
                        count1 = count1 +1;
                    }
                }
            }
            else if (Running!=NULL)
            {
                printf("%s:",Running->pname);
                print(head);
                if (removal_time == timer)  // Execution of the Running Process has completed
                {
                   
                    Running = dequeue(&head);   // Dequeue the process
                    if (Running->pid == -1)
                    {
                        break;
                    }
                    else
                    {
                        removal_time = timer+ Running->duration;
                    }
                }
                for (int index = 0; index<NumProcesses;index++)
                {
                    if( (ProcessList+index)->arrivaltime == timer)
                    {
                        enqueue(&head,(ProcessList+index),(ProcessList+index)->duration);
                        count1 =count1 +1;
                        if (removal_time-timer >(ProcessList+index)->duration)
                        {
                            Running->duration = removal_time - timer;
                            enqueue(&head,Running,Running->duration);
                            Running = dequeue(&head);
                            removal_time = timer+ Running->duration;
                        }
                        
                    }
                }
                
            }
            timer++;
         
        }
        free(Running);  
    }
    if (strcmp(argv[2],rr)==0)
    {
        int count = 0;
        while (count !=NumProcesses || head!=NULL || Running!=NULL)
        {
            printf("%d:",timer);
            if (Running == NULL)
            {
                printf("idle:");
                print(head);
                for (int index = 0; index<NumProcesses;index++)
                {
                    if((ProcessList+index)->arrivaltime == timer)
                    {
                        enqueue(&head,(ProcessList+index),(ProcessList+index)->duration);
                        count = count + 1;
                    }
                    
                }
                if (head!=NULL)
                {
                    Running = dequeue(&head);
                    if (Running->pid == -1)
                    break;
                    removal_time = timer + 1;
                    
                    Running->duration = Running->duration - 1;
                }
            }
            else
            {
                printf("%s:",Running->pname);
                print(head);
                if (removal_time==timer)
                {
                    if (Running->duration!=0)
                    {
                        //Running->duration = Running->duration - 1;
                        add_last(&head,Running);
                    } 
                    else
                    {
                        Running = NULL;
                    }
                      
                }
                for (int index = 0; index<NumProcesses;index++)
                {
                    if( (ProcessList+index)->arrivaltime == timer)
                    {
                        enqueue(&head,(ProcessList+index),(ProcessList+index)->duration);
                        count = count + 1;
                    
                        
                      
                    }
                }
                if (head!=NULL)
                {
                Running = dequeue(&head);
                if (Running->pid == -1)
                    break;
                removal_time = timer + 1;
                Running->duration = Running->duration - 1;
                }
            }
            timer++;

        }
        free(Running);  
        
        
    }
    free(ProcessList);
    fclose(stream);
    return  0;

}