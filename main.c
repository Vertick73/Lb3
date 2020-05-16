#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>


#define MAX_COMMANDLEN 1024
#define MAX_OPTIONS 255
#define MAX_PROCESS 255
#define _GNU_SOURCE
#include <getopt.h>

int procCount = 0;
int proces[MAX_PROCESS];
int ischild=-1;


void ProcesRun(char *, char* argv[]);
void InputParser(char*);

void KillChild(sig_t  s)
{
    if(s!=2)
    {
        return;
    }
    if(ischild==1)
        return;    
    printf("Caught signal %d\n",s);
    if(procCount>0)
    {
        printf("Kill Pid %d\n",proces[--procCount]);
        kill(proces[procCount],SIGTERM);
    }
    else
    {
        printf("No Children \n Exiting\n");
        exit(0);
    }           
}


int main(int argc, char *argv[])
{
   struct sigaction sigIntHandler;


   sigIntHandler.sa_handler = KillChild;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);


    char input[MAX_COMMANDLEN];

    do
    {
        printf("Use -r {name} -o {options}\n");
        fgets(input, MAX_COMMANDLEN, stdin);
        InputParser(&input);

    } while (input[0]!='q');

    return 0;
}

void InputParser(char* input)
{
    char*argv[MAX_OPTIONS];
    int argc =1;
    optind =1;
    char * pch = strtok (input," ");
    while (pch != NULL)
    {
        argv[argc++]=pch;
        pch = strtok (NULL, " \n");
    }
    argv[argc]=NULL;
    int opt;
    struct option longopts[]=
    {     
        {"help",0,NULL,'h'},
        {"run",1,NULL,'r'},
        {"option",1,NULL,'o'},
        {0,0,0,0}
    };
    char *argvToRun[MAX_OPTIONS];
    int argCToRun=0;
    int needRun = -1;
    char* name;

        while ((opt=getopt_long(argc,argv,":hr:o:",longopts,NULL))!=-1)
    {
        switch (opt)
        {
            case 'h':
                printf("No help for u\n");    
                break;
            case 'r':
                if(optarg!=NULL)
                    name=optarg;
                else
                {
                    perror("Error to read name");
                }
                needRun=1;            
                break;    
            case 'o':
                argvToRun[argCToRun++]=optarg;
                break;
            case ':':
                printf("Options needs a value\n");
                break;
            case '?':
                printf("Unknown option: %c\n",optopt);
                break;     

        }
    }

    if(needRun==1)
    {
        argvToRun[argCToRun]=NULL;
        ProcesRun(name,argvToRun);
    }
}

void ProcesRun(char * name, char* argv[])
{
    pid_t pid;
    int rv;
    switch(pid=fork()) 
    {
        case -1:
            perror("Fork Error\n");
            exit(1);
        case 0:
            ischild=1;
            execv(name,argv);
            exit(1);
            break;
        default:
            printf("Forked with pid%d\n",pid);
            proces[procCount++]=pid;

    }

}
