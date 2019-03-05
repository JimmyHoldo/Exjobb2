#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int runps(char *arg, char *drs, char *rss)
{
    FILE *fp;
    char path[1035];

    char command[100];
    sprintf(command, "ps v -p %s", arg);

    /* Open the command for reading. */
    fp = popen(command, "r");

    if (fp == NULL)
    {
        printf("Failed to run command\n" );
        exit(1);
    }

    char seps[]   = " ";
    char *token;
    int test =  0;
    /* Read the output a line at a time - output it. */

    while (fgets(path, sizeof(path)-1, fp) != NULL)
    {
        printf("%s", path);

        if(test == 1)
        {
            int i = 0;
            token = strtok( path, seps );
            while( token != NULL )
            {
                /* While there are tokens in "string" */
                //printf( " %s\n", token );
                if(i == 6) {
                    const char *ptr = token;
                    strcpy(drs, ptr);
                } else if (i == 7){
                    const char *ptr = token;
                    strcpy(rss, ptr);
                }
                /* Get next token: */
                token = strtok( NULL, seps );
                i++;
            }

        } else {
            test = 1;
        }
    }
    /* close */
    pclose(fp);
    return 0;
}

int runps2(char *arg, char *cpu, char *mem, char *vsz)
{
    FILE *fp;
    char path[1035];

    char command[100];
    sprintf(command, "ps u -p %s", arg);

    /* Open the command for reading. */
    fp = popen(command, "r");

    if (fp == NULL)
    {
        printf("Failed to run command\n" );
        exit(1);
    }

    char seps[]   = " ";
    char *token;
    int test =  0;
    /* Read the output a line at a time - output it. */

    while (fgets(path, sizeof(path)-1, fp) != NULL)
    {
        printf("%s", path);

        if(test == 1)
        {
            int i = 0;
            token = strtok( path, seps );
            while( token != NULL )
            {
                /* While there are tokens in "string" */
                //printf( " %s\n", token );
                if(i == 2) {
                    const char *ptr = token;
                    strcpy(cpu, ptr);
                } else if (i == 3){
                    const char *ptr = token;
                    strcpy(mem, ptr);
                } else if (i == 4){
                    const char *ptr = token;
                    strcpy(vsz, ptr);
                }
                /* Get next token: */
                token = strtok( NULL, seps );
                i++;
            }

        } else {
            test = 1;
        }
    }
    /* close */
    pclose(fp);
    return 0;
}

int runfree(char *used, char *freevar, char *available)
{
    FILE *fp;
    char path[1035];

    /* Open the command for reading. */
    fp = popen("free", "r");

    if (fp == NULL)
    {
        printf("Failed to run command\n" );
        exit(1);
    }

    char seps[]   = " ";
    char *token;
    int test =  0;
    /* Read the output a line at a time - output it. */

    while (fgets(path, sizeof(path)-1, fp) != NULL)
    {
        //printf("%s", path);

        if(test == 1)
        {
            int j = 0;
            token = strtok( path, seps );
            while( token != NULL )
            {
                /* While there are tokens in "string" */
                //printf( " %s\n", token );
                if(j == 2){
                    const char *ptr = token;
                    strcpy(used, ptr);
                } else if (j == 3){
                    const char *ptr = token;
                    strcpy(freevar, ptr);
                } else if (j == 6) {
                    const char *ptr = token;
                    strcpy(available, ptr);
                }

                /* Get next token: */
                token = strtok( NULL, seps );
                j++;
            }
            test = 0;
        }
        else
        {
            test = 1;
        }
    }
    /* close */
    pclose(fp);
    return 0;
}

int main(int argc, char *argv[] )
{
    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 10000000L;
    FILE *f = fopen("../files/pc_no_prg.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "CPU\t\tMEM\t\tVSZ\t\tDRS\t\tRSS\t\tUSED\t\tFREE\t\tAVAILABLE\t\n");

    int i = 0;
    char cpustr[20], memstr[20], vszstr[20], drsstr[20], rssstr[20], used[20], free[20], available[20];
    if(atoi((argv)[1]) == 1)
    {
        while(i < 10000)
        {
            int drsint=0, rssint=0, vszint = 0;
            double cpuvalue = 0, memvalue = 0;
            for(int j=2; j<argc; j++){
                runps(argv[j], drsstr, rssstr);
                runps2(argv[j], cpustr, memstr, vszstr);
                drsint += atoi(drsstr);
                rssint += atoi(rssstr);
                cpuvalue += atof(cpustr);
                memvalue += atof(memstr);
                vszint += atoi(vszstr);
            }
            runfree(used, free, available);
            fprintf(f, "%.3f\t\t%.3f\t\t%d\t\t%d\t\t%d\t\t%s\t\t%s\t\t%s\t\n", cpuvalue, memvalue, vszint, drsint, rssint, used, free, available);
            i++;
            nanosleep(&ts, NULL);
        }
    }
    else
    {
        while(i < 10000)
        {
            runfree(used, free, available);
            fprintf(f, "%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\n", "", "", "", "", "", used, free, available);
            i++;
            nanosleep(&ts, NULL);
        }
    }


    fclose(f);
    printf("File is created\n");
    return 0;
}
