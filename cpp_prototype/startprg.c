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
<<<<<<< HEAD
        execv("writer", NULL);
=======
        execv("writer");
>>>>>>> 986ad2c94d4b4fabe550f7295c88e1edf2fe1a49
        exit(127); /* only if execv fails */
    }
    else { /* pid!=0; parent process */
        pid_t pid2=fork();
        if (pid2==0) { /* child process */

<<<<<<< HEAD
            execv("datagen", NULL);
            exit(127); /* only if execv fails */
        }
        else { /* pid!=0; parent process */
            execv("reader", NULL);
=======
            execv("datagen");
            exit(127); /* only if execv fails */
        }
        else { /* pid!=0; parent process */
            execv("reader");
>>>>>>> 986ad2c94d4b4fabe550f7295c88e1edf2fe1a49
        }
    }
    return 0;
}
<<<<<<< HEAD

=======
>>>>>>> 986ad2c94d4b4fabe550f7295c88e1edf2fe1a49
