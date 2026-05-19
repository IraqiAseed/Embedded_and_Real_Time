#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int parent_to_child[2];
    int child_to_parent[2];

    char buffer[100];

    if (pipe(parent_to_child) == -1)
    {
        perror("pipe parent_to_child");
        return 1;
    }

    if (pipe(child_to_parent) == -1)
    {
        perror("pipe child_to_parent");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        /* Child */

        close(parent_to_child[1]); /* child does not write here */
        close(child_to_parent[0]); /* child does not read here */

        read(parent_to_child[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        write(child_to_parent[1], "pong", strlen("pong") + 1);

        close(parent_to_child[0]);
        close(child_to_parent[1]);

        exit(0);
    }
    else
    {
        // Parent

        close(parent_to_child[0]); //parent does not read here
        close(child_to_parent[1]); // parent does not write here

        write(parent_to_child[1], "ping", strlen("ping") + 1);

        read(child_to_parent[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(parent_to_child[1]);
        close(child_to_parent[0]);

        wait(NULL);
    }

    return 0;
}