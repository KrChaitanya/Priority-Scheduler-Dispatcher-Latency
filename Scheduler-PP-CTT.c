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
struct Process_struct *  deQueue()
{
     if(Ready_Queue->front==NULL)
     {
	     return NULL;
     }
     struct Process_struct * temp=Ready_Queue->front;
     Ready_Queue->front=Ready_Queue->front->next;
     temp->next=NULL;
     if(Ready_Queue->front==NULL)
     {
	     Ready_Queue->rear=NULL;
     }
     return temp;
}

void maintain_queue()
{
int count=0;
           for(int i=0;i<n;i++)
	   {
                   
		   if(process_list[i].state == NEW && now>=process_list[i].at)
		   {      
			   
			   enqueue(&process_list[i]);
			   process_list[i].state=READY;
			   
                    }
                  if(process_list[i].state==EXIT)
                  {
                    count++;
			}
	   }
	   if(count==n)
	   {
		  EVERYTHING_EXITED=1; 
		  
	   }
}
int main()
{
       printf("\n\t\t\tCoder: Chaitanya Arora\n");

Ready_Queue =(struct Queue*) malloc(sizeof(struct Queue));
	printf("\n\n\t\tHow many Processes to schedule # ");
	scanf("%d",&n);
	process_list=(struct Process_struct *)malloc(sizeof(struct Process_struct)*n);
        
	for(int i=0;i<n;i++)
	{
             process_list[i].pid=i+1;
	     printf("\n\n\t\tEnter Arrival Time For Process %d # ",(i+1));
	     scanf("%d",&(process_list[i].at));
	     printf("\n\t\tEnter Execution Time For Process %d # ",(i+1));
	     scanf("%d",&(process_list[i].to_run_time));
             printf("\n\t\tEnter Priority For Process %d # ",(i+1));
	     scanf("%d",&(process_list[i].priority)); 
	     
            process_list[i].bt=process_list[i].to_run_time;
	    process_list[i].state=NEW;

	        
	}
       int i, j;
       struct Process_struct key; 
    for (i = 1; i < n; i++) { 
        key = process_list[i]; 
        j = i - 1; 
  
        while (j >= 0 && process_list[j].at > key.at) { 
            process_list[j + 1] = process_list[j]; 
            j = j - 1; 
        } 
        process_list[j + 1] = key; 
    } 
       

struct Process_struct *running_process,*prev;
printf("Gannt Chart ############\n");
while(1)
{
     int nnl=0;
      
     if(Ready_Queue->front!=NULL)
    {
           nnl=1;
    }
    
        maintain_queue();
        if(EVERYTHING_EXITED==1)
        {

                    break;      
        }
        
        if(Ready_Queue->front!=NULL && do_a_context_switch==1)
	{
            if(nnl)
            {
              printf("%d+CONTEXT DELAY+",now);
                now+=contexttime;
                running_process->priority--;
                
                
             }
             
             time_in_execution=1;
             prev=running_process;
	     running_process=deQueue();
             if(prev!=running_process)
              {
                       printf("%d +Process %d+",now,running_process->pid);
               }
             running_process->t2pass=0;
             running_process->eqt=-1;
             running_process->state=RUNNING;
             if(nnl)
            {
               
                maintain_queue();
                
                
             }
             running_process->to_run_time--;
	     now++;
             
             for(int i=0;i<n;i++)
                {
                   if(process_list[i].state==READY)
                   {
                   process_list[i].t2pass++;
                   }
                }
             if(time_in_execution==running_process->bt)
             {
                  do_a_context_switch=1;
                  running_process->state=EXIT;
                  running_process->ct=now;
                  running_process->tat=now-running_process->at;
                  running_process->wt=running_process->tat-running_process->bt;
                   
                  
             }
             else
            {
               do_a_context_switch=0;
              }
	}
        else if(do_a_context_switch==0&&running_process!=NULL && running_process->state==RUNNING )
	{
             if(running_process->to_run_time==0)
             {
                  do_a_context_switch=1;
                  running_process->state=EXIT;
                  running_process->ct=now;
                  running_process->tat=now-(running_process->at);
                  running_process->wt=(running_process->tat)-(running_process->bt);
                  continue;
             }
             else if(Ready_Queue->front!=NULL)
             {
              if(running_process->priority>Ready_Queue->front->priority)
               {
                running_process->state=READY;
                enqueue(running_process);
                do_a_context_switch=1;
                
                continue;
                }
              }      
             time_in_execution++;
             running_process->to_run_time--;
            now++;
             for(int i=0;i<n;i++)
                {
                   if(process_list[i].state==READY)
                   {
                   process_list[i].t2pass++;
                   }
                }
             if(running_process->to_run_time==0)
             {
                  do_a_context_switch=1;
                  running_process->state=EXIT;
                  running_process->ct=now;
                  running_process->tat=now-(running_process->at);
                  running_process->wt=(running_process->tat)-(running_process->bt);
                  
             }            
             else if(Ready_Queue->front!=NULL)
             {
              if(running_process->priority>Ready_Queue->front->priority)
               {
                running_process->state=READY;
                enqueue(running_process);
                do_a_context_switch=1;
                }
                else
                {
                 do_a_context_switch=0;
                }
              }     
            else
          { 
              do_a_context_switch=0;
	  }

          
             
          
	}
else
{
printf("%d+ IDLE +");
now++;
 for(int i=0;i<n;i++)
                {
                   if(process_list[i].state==READY)
                   {
                   process_list[i].t2pass++;
                   }
                }
}

}
printf("%d\n",now);
int sumwt=0,sumtat=0;

printf("\n\n\tPID\t\tAT\t\tbt\t\tCT\t\tTAT\t\tWT\n");
printf("=====================================================================================\n");
for(int i=0;i<n;i++)
	{
           printf("\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",process_list[i].pid,process_list[i].at,process_list[i].bt,process_list[i].ct,process_list[i].tat,process_list[i].wt);
           sumwt+=process_list[i].wt;
           sumtat+=process_list[i].tat;
           
	}
printf("\n\n Avergae TAT=%f \n Average WT=%f\n\n",(sumtat/(n*1.0)),(sumwt/(n*1.0)));
}
