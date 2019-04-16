#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

#define NEW 0
#define READY 1
#define WAITING 2
#define RUNNING 3
#define EXIT 4

int now=0,time_in_execution=0,EVERYTHING_EXITED=0,contexttime=2,do_a_context_switch=1,n;
struct Process_struct 
{
	int pid;
	int state;
	int to_run_time;
        int priority;
        int t2pass,eqt;
	int at;
	int wt,tat,ct,bt;
	struct Process_struct *prev;
	struct Process_struct *next;
} *process_list;
int chpp;
struct Queue
{
	struct Process_struct *front ,*rear;
}*Ready_Queue;

void enqueue(struct Process_struct *p)
{
        p->eqt=now;
	if(Ready_Queue->front==NULL)
	{
		Ready_Queue->front=p;
		Ready_Queue->rear=p;
		p->next=NULL;
	}
        
	else
	{
		if(p->priority<Ready_Queue->front->priority)
		{
			p->next=Ready_Queue->front;
			Ready_Queue->front->prev=p;
			Ready_Queue->front=p;
		}
               else if(p->priority==Ready_Queue->front->priority)
		{
			p->next=Ready_Queue->front->next;
			p->prev=Ready_Queue->front;
			Ready_Queue->front->next=p;
                        if(p->next!=NULL)
                      {
                        p->next->prev=p;
		      }
                }
		else if(p->priority>Ready_Queue->rear->priority)
		{
			p->next=NULL;
			Ready_Queue->rear->next=p;
			p->prev=Ready_Queue->rear;
			Ready_Queue->rear=p;
		}
		else
		{
                   struct Process_struct *start=Ready_Queue->front->next;
		   while(start->priority<p->priority)
		   {
			   start=start->next;
		   }
                   if(start!=NULL&& p->priority==start->priority)
                   {
                          p->next=start->next;
                          start->next=p;
                          p->prev=start;
                      
                   }
                  else
               {
		   (start->prev)->next=p;
		   p->next=start;
		   p->prev=start->prev;
		   start->prev=p;
                }

		}
	}
}
