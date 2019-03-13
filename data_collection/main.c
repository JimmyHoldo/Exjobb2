
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

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
       // printf("%s", path);

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
        //printf("%s", path);

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
int uptime(char *val)
{
    FILE *fp;
    char path[1035];

    /* Open the command for reading. */
    fp = popen("cat /proc/uptime", "r");

    if (fp == NULL)
    {
        printf("Failed to run command\n" );
        exit(1);
    }

    char seps[]   = " ";
    char *token;
    int stop = 0;
    while ((fgets(path, sizeof(path)-1, fp) != NULL) && stop == 0)
    {
        //printf("%s", path);

        int j = 0;
        token = strtok( path, seps );
        while( token != NULL )
        {
            /* While there are tokens in "string" */
            //printf( " %s\n", token );
            if(j == 0){
                const char *ptr = token;
                strcpy(val, ptr);
                stop = 1;
                break;
            }

            /* Get next token: */
            token = strtok( NULL, seps );
            j++;
        }
    }
    /* close */
    pclose(fp);
    return 0;
}

int proct(char *pid, char *value, char *value2)
{
    FILE *fp;
    char path[1035];

    char command[100];
    sprintf(command, "cat /proc/%s/stat", pid);
    /* Open the command for reading. */
    fp = popen(command, "r");

    if (fp == NULL)
    {
        printf("Failed to run command\n" );
        exit(1);
    }

    char seps[]   = " ";
    char *token;

    while (fgets(path, sizeof(path)-1, fp) != NULL)
    {
        //printf("%s", path);

        int j = 0;
        token = strtok( path, seps );
        while( token != NULL )
        {
            /* While there are tokens in "string" */
            //printf( " %s\n", token );
            if(j == 13){
                const char *ptr = token;
                strcpy(value, ptr);
            } else if(j == 14){
                const char *ptr = token;
                strcpy(value2, ptr);
                break;
            }

            /* Get next token: */
            token = strtok( NULL, seps );
            j++;
        }
    }
    /* close */
    pclose(fp);
    return 0;
}

int cpuUsage(int argc, char *argv[], char *res2)
{
    struct timespec ts;
    ts.tv_sec  = 1;
    ts.tv_nsec = 0;
    double utime=0, ctime=0, o_utime=0, o_ctime=0;
    char utimestr[20], ctimestr[20], o_utimestr[20], o_ctimestr[20], time[20], time2[20];
    for(int j=2; j<argc; j++){
        proct(argv[j], o_utimestr, o_ctimestr);
        o_utime += atof(o_utimestr);
        o_ctime += atof(o_ctimestr);
    }
    uptime(time);
    nanosleep(&ts, NULL);

    for(int j=2; j<argc; j++){
        proct(argv[j], utimestr, ctimestr);
        utime += atof(utimestr);
        ctime += atof(ctimestr);
    }
    uptime(time2);
    double res = ((utime - o_utime)+(ctime - o_ctime) / (atof(time2)-atof(time)));
    sprintf(res2, "%lf", res);
    return 0;
}

int main(int argc, char *argv[] )
{
    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 10000;
    FILE *f = fopen("../files/arm_erl_350ms.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "CPUProc\t\t\tCPU  \t\t\tMEM  \t\t\tVSZ  \t\tDRS  \t\tRSS  \t\tUSED  \t\tFREE  \t\tAVAILABLE\t\n");

    int i = 0;
    char cpustr[20], memstr[20], vszstr[20], drsstr[20], rssstr[20], used[20], free[20], available[20], cpuproc[20];
    if(atoi((argv)[1]) == 1)
    {
        while(i < 120)
        {
            printf("Iterration %d\n", i );
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

            cpuUsage(argc, argv, cpuproc);

            fprintf(f, "%.3f  \t\t\t%.3f\t\t\t%.3f\t\t\t%d  \t\t%d  \t\t%d  \t\t%s  \t\t%s  \t\t%s\t\n", atof(cpuproc),cpuvalue, memvalue, vszint, drsint, rssint, used, free, available);
            i++;
            //nanosleep(&ts, NULL);
        }
    }
    else
    {
        while(i < 10000)
        {
            runfree(used, free, available);
            fprintf(f, "%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\n", "", "", "", "", "", "", used, free, available);
            i++;
            nanosleep(&ts, NULL);
        }
    }

    fclose(f);
    printf("File is created\n");
    return 0;
}
