#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#define MAX_LINE 1024
#define MAX_ARGS 64

int main()
{
    size_t len = 0;
    char buffer[MAX_LINE];
    char *args[MAX_ARGS];
    size_t i = 0;

    while (1)
    {

        //prompt
        printf("> ");
        fflush(stdout);

        //read command
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            break;
        }

        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }

        //parse command
        i = 0;
        char *token = strtok(buffer, " ");

        // i < MAX_ARGS - 1 to reserve one slot for the terminating NULL
        while (token != NULL && i < MAX_ARGS - 1)
        {
            args[i] = token;
            i++;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        if (args[0] == NULL)
        {
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        //parent wait, child execute command
        if (fork() != 0) //assume fork not fail
        {
            //Parent Code
            int status;
            waitpid(-1, &status, 0);
        }
        else
        {
            // Child code

            // char *envp[] = {NULL};
            // execve(args[0], args, envp);

            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        }
    }

    return 0;
}