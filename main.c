#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

int main(int argc, char *argv[])
{
    int opt;
    struct option longopts[]=
    {     
        {"help",0,NULL,'h'},
        {"run",1,NULL,'r'},
        {"path",1,NULL,'p'},
        {0,0,0,0}
    };

    while ((opt=getopt_long(argc,argv,":hr:f",longopts,NULL))!=-1)
    {
        switch (opt)
        {
            case 'h':
                printf("No help for u\n");    
                break;
            case 'r':
                printf("Run\n");   
                ProcesRun(optarg);
                break;    
            case ':':
                printf("Options needs a value\n");
                break;
            case '?':
                printf("Unknown option: %c\n",optopt);
                break;     

        }
        /* code */
    }

    int* a;
    scanf(a);
    

    return 0;
}

void ProcesRun(char * name)
{
    pid_t pid;
    int rv;
    switch(pid=fork()) 
    {
        case -1:
            perror("Fork Error");
            exit(1);
        case 0:
            printf("Im a Fork");
            execl(name," ","Hello", "World!", NULL);
            //int execl("chrome.exe",NULL);
            break;
    }

}
