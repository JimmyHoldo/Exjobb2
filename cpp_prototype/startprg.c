#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

int main()
{
    /*Spawn a child to run the program.*/
    pid_t pid=fork();
    if (pid==0) { /* child process */
        execv("writer");
        exit(127); /* only if execv fails */
    }
    else { /* pid!=0; parent process */
        pid_t pid2=fork();
        if (pid2==0) { /* child process */

            execv("datagen");
            exit(127); /* only if execv fails */
        }
        else { /* pid!=0; parent process */
            execv("reader");
        }
    }
    return 0;
}
