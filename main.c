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
int silent = 0;


void ProcesRun(char *, char* argv[]);
void InputParser(char*);

void KillChild()
{
    if(procCount>0)
    {
        printf("Kill Pid %d\n",proces[--procCount]);
        kill(proces[procCount],15);
    }
    else
    {
        printf("No Children \n Exiting\n");
    }           
}


int main(int argc, char *argv[])
{
    char input[MAX_COMMANDLEN];

    do
    {
        if(silent==0&&procCount>0)
        {
             printf("Use -r {name} -o {options}\n");         
        }
       
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
        {"run",1,NULL,'r'},
        {"option",1,NULL,'o'},
        {"silent",0,NULL,'s'},
        {"kill",0,NULL,'k'},
        {0,0,0,0}
    };
    char *argvToRun[MAX_OPTIONS];
    int argCToRun=0;
    int needRun = -1;
    char* name;

        while ((opt=getopt_long(argc,argv,":skr:o:",longopts,NULL))!=-1)
    {
        switch (opt)
        {
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
            case 'k':
                KillChild();
                return;
            case 's':
                silent = 1;
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
            execv(name,argv);
            break;
        default:
            printf("Forked with pid %d\n",pid);
            proces[procCount++]=pid;

    }

}
