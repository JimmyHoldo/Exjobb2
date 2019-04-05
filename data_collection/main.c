
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
    for(int j=0; j<argc; j++){
        proct(argv[j], o_utimestr, o_ctimestr);
        o_utime += atof(o_utimestr);
        o_ctime += atof(o_ctimestr);
    }
    uptime(time);
    nanosleep(&ts, NULL);

    for(int j=0; j<argc; j++){
        proct(argv[j], utimestr, ctimestr);
        utime += atof(utimestr);
        ctime += atof(ctimestr);
    }
    uptime(time2);
    double res = ((utime - o_utime)+(ctime - o_ctime) / (atof(time2)-atof(time)));
    sprintf(res2, "%lf", res);
    return 0;
}

void fetchIds(char *writerId, char *readerId, char *datagenId)
{
    char path1[1035],path2[1035],path3[1035];

    FILE *p1 = popen("ps -A | grep writer", "r");
    fgets(path1, sizeof(path1)-1, p1);
    const char *ptr = strtok( path1, " " );
    strcpy(writerId, ptr);
    printf("%s writerId\n", writerId);

    FILE *p2 = popen("ps -A | grep reader", "r");
    fgets(path2, sizeof(path2)-1, p2);
    const char *ptr2 = strtok( path2, " " );
    strcpy(readerId, ptr2);
    printf("%s readerId\n", readerId);

    FILE *p3 = popen("ps -A | grep datagen", "r");
    fgets(path3, sizeof(path3)-1, p3);
    const char *ptr3 = strtok( path3, " " );
    strcpy(datagenId, ptr3);
    printf("%s datagenId\n", datagenId);

    pclose(p1);
    pclose(p2);
    pclose(p3);
}

void fetchIdsErl(char *erts, char *child, char *readerwriter1, char *readerwriter2)
{
    char path1[1035],path2[1035],path3[1035];

    FILE *p1 = popen("ps x | grep bin/beam", "r");
    fgets(path1, sizeof(path1)-1, p1);
    //printf("%s", path1);
    //strtok( path1, " " );
    const char *ptr = strtok( path1, " " );
    strcpy(erts, ptr);
    printf("%s erts\n", erts);

    FILE *p2 = popen("ps x | grep erl_child", "r");
    fgets(path2, sizeof(path2)-1, p2);
    const char *ptr2 = strtok( path2, " " );
    strcpy(child, ptr2);
    printf("%s erl_child\n", child);

    FILE *p3 = popen("ps x | grep readerwriterprg", "r");
    fgets(path3, sizeof(path3)-1, p3);
    //printf("%s", path3);
    const char *ptr3 = strtok( path3, " " );
    strcpy(readerwriter1, ptr3);
    printf("%s readerwriter1\n", readerwriter1);
    fgets(path3, sizeof(path3)-1, p3);
    //printf("%s", path3);
    const char *ptr4 = strtok( path3, " " );
    strcpy(readerwriter2, ptr4);
    printf("%s readerwriter2\n", readerwriter2);

    pclose(p1);
    pclose(p2);
    pclose(p3);
}

int main(int argc, char *argv[] )
{
    struct timespec ts;
    ts.tv_sec  = 5;
    ts.tv_nsec = 100;
    struct timespec ts2;
    ts2.tv_sec  = 1;
    ts2.tv_nsec = 0;

    char filename[50];

    int i = 0;
    char cpustr[20], memstr[20], vszstr[20], drsstr[20], rssstr[20], used[20], free[20], available[20], cpuproc[20];
    if(atoi((argv)[1]) == 1)
    {
        char *fileArr[] = {"10s", "5s","2s","1s","750ms","500ms","350ms","250ms","200ms","175ms","150ms","125ms","100ms"};
        int timeSArr[] = {10, 5,2,1,0,0,0,0,0,0,0,0,0};
        int timeMsArr[] = {0,0,0,0,750,500,350,250,200,175,150,125,100};

        int timeErlMsArr[] = {10000,5000,2000,1000,750,500,350,250,200,175,150,125,100};

        for(int x=0; x<10; x++)
        {
            printf("Iteration x=%d\n", x );
            for(int y=0; y<13; y++)
            {
                FILE *pp = popen("./../clientserver/client", "r");
                pclose(pp);

                i=0;
                sprintf(filename, "../files/%darm_cpp_%s.txt", x, fileArr[y]);
                FILE *f = fopen(filename, "w");
                if (f == NULL)
                {
                    printf("Error opening file!\n");
                    exit(1);
                }
                fprintf(f, "CPUProc\t\t\tCPU  \t\t\tMEM  \t\t\tVSZ  \t\tDRS  \t\tRSS  \t\tUSED  \t\tFREE  \t\tAVAILABLE\t\n");
                fclose(f);
                FILE *p1 = popen("./../cpp_prototype/writer; ", "r");
                nanosleep(&ts, NULL);
                FILE *p2 = popen("./../cpp_prototype/reader", "r");

                char command2[100];
                sprintf(command2, "./../cpp_prototype/datagen %d %d", timeSArr[y], timeMsArr[y]);
                FILE *p3 = popen(command2, "r");

                char writerId[20], readerId[20], datagenId[20];
                fetchIds(writerId, readerId, datagenId);

                char *idsP[] = {writerId, readerId, datagenId};

                while(i < 120)
                {
                    printf("cppIteration %d\n", i );
                    int drsint=0, rssint=0, vszint = 0;
                    double cpuvalue = 0, memvalue = 0;
                    for(int j=0; j<3; j++){
                        runps(idsP[j], drsstr, rssstr);
                        runps2(idsP[j], cpustr, memstr, vszstr);
                        drsint += atoi(drsstr);
                        rssint += atoi(rssstr);
                        cpuvalue += atof(cpustr);
                        memvalue += atof(memstr);
                        vszint += atoi(vszstr);
                    }
                    runfree(used, free, available);

                    cpuUsage(3, idsP, cpuproc);
                    FILE *ff = fopen(filename, "a");
                    if (ff == NULL)
                    {
                        printf("Error opening file!\n");
                        exit(1);
                    }
                    fprintf(ff, "%.3f  \t\t\t%.3f\t\t\t%.3f\t\t\t%d  \t\t%d  \t\t%d  \t\t%s  \t\t%s  \t\t%s\t\n", atof(cpuproc),cpuvalue, memvalue, vszint, drsint, rssint, used, free, available);
                    fclose(ff);
                    i++;
                }
                FILE *p11 = popen("pkill -f datagen", "r");
                pclose(p11);
                FILE *p21 = popen("pkill -f reader", "r");
                pclose(p21);
                FILE *p31 = popen("pkill -f writer", "r");
                pclose(p31);

                pclose(p1);
                pclose(p2);
                pclose(p3);

                FILE *pp1 = popen("./../clientserver/client", "r");
                pclose(pp1);


                sprintf(filename, "../files/%darm_erl_%s.txt", x, fileArr[y]);


                char appFile[300];
                sprintf(appFile, "{application, app, [{vsn, \"1.0.0\"}, {modules, [app, worker_sup, subworker_sup, writer, reader, datagen, serialport]}, {register, [app]}, {mod, {app, [%d]}}]}.", timeErlMsArr[y]);
                char filename3[30];
                sprintf(filename3, "../Erl_prototype2/app.app");
                FILE *f3 = fopen(filename3, "w");
                if (f3 == NULL)
                {
                    printf("Error opening file!\n");
                    exit(1);
                }
                fprintf(f3, "%s", appFile);
                fclose(f3);
                FILE *f2 = fopen(filename, "w");
                fprintf(f2, "CPUProc\t\t\tCPU  \t\t\tMEM  \t\t\tVSZ  \t\tDRS  \t\tRSS  \t\tUSED  \t\tFREE  \t\tAVAILABLE\t\n");

                if (f2 == NULL)
                {
                    printf("Error opening file!\n");
                    exit(1);
                }
                fclose(f2);
                char command3[100];
                sprintf(command3, "cd ../Erl_prototype2; erl -noshell -eval \"application:start(app)\" +P 1024 +Q 1024 +Mea min +L");
                FILE *p4 = popen(command3, "r");
                nanosleep(&ts, NULL);
                char erts[20], child[20], readerwriter1[20], readerwriter2[20];
                fetchIdsErl(erts, child, readerwriter1, readerwriter2);

                char *idsP2[] = {erts, child, readerwriter1, readerwriter2};
                i = 0;
                while(i < 120)
                {
                    printf("erllIteration %d\n", i );
                    int drsint=0, rssint=0, vszint = 0;
                    double cpuvalue = 0, memvalue = 0;
                    for(int j=0; j<4; j++){
                        runps(idsP2[j], drsstr, rssstr);
                        runps2(idsP2[j], cpustr, memstr, vszstr);
                        drsint += atoi(drsstr);
                        rssint += atoi(rssstr);
                        cpuvalue += atof(cpustr);
                        memvalue += atof(memstr);
                        vszint += atoi(vszstr);
                    }
                    runfree(used, free, available);
                    cpuUsage(4, idsP2, cpuproc);
                    FILE *f3 = fopen(filename, "a");
                    fprintf(f3, "%.3f  \t\t\t%.3f\t\t\t%.3f\t\t\t%d  \t\t%d  \t\t%d  \t\t%s  \t\t%s  \t\t%s\t\n", atof(cpuproc),cpuvalue, memvalue, vszint, drsint, rssint, used, free, available);
                    fclose(f3);
                    i++;
                    //nanosleep(&ts, NULL);
                }
                char command[20];
                sprintf(command, "kill %s", erts);
                FILE *p41 = popen(command, "r");
                sprintf(command, "kill %s", child);
                FILE *p42 = popen(command, "r");
                sprintf(command, "kill %s", readerwriter1);
                FILE *p43 = popen(command, "r");
                sprintf(command, "kill %s", readerwriter2);
                FILE *p44 = popen(command, "r");

                pclose(p4);
                pclose(p41);
                pclose(p42);
                pclose(p43);
                pclose(p44);
            }
        }
    }
    else
    {
        sprintf(filename, "../files/arm_%s.txt", "no_prg");
        printf("No prg\n");
        FILE *f = fopen(filename, "w");
        fprintf(f, "CPUProc\t\t\tCPU  \t\t\tMEM  \t\t\tVSZ  \t\tDRS  \t\tRSS  \t\tUSED  \t\tFREE  \t\tAVAILABLE\t\n");

        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        while(i < 120)
        {
            printf("Iteration: %d\n", i);
            runfree(used, free, available);
            char s[10] = "     ";
            fprintf(f, "%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\n", s, s, s, s, s, s, used, free, available);
            i++;
            nanosleep(&ts2, NULL);
        }
        fclose(f);
    }
    printf("File is created\n");
    return 0;
}
