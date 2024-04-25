#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    char input[MAX_LINE]; /* store user input */

    while (should_run) {
        printf("mysh> ");
        fflush(stdout);

        /* Read user input */
        fgets(input, MAX_LINE, stdin);

        /* Tokenize user input */
        char *token = strtok(input, " \n");
        int i = 0;
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, " \n");
            i++;
        }
        args[i] = NULL; // Null-terminate the argument list

        if (strcmp(args[0], "exit") == 0) { // Check if user wants to exit
            should_run = 0;
        } else {
            int background = 0;
            if (strcmp(args[i-1], "&") == 0) { // Check if the last argument is "&"
                background = 1;
                args[i-1] = NULL; // Remove "&" from the arguments
            }

            pid_t pid = fork(); // Fork a child process

            if (pid < 0) { // Fork failed
                fprintf(stderr, "Fork failed\n");
                return 1;
            } else if (pid == 0) { // Child process
                execvp(args[0], args);
                fprintf(stderr, "Execution failed\n");
                return 1;
            } else { // Parent process
                if (!background) {
                    wait(NULL); // Parent waits for child to terminate if not in background
                }
            }
        }
    }

    return 0;
}
