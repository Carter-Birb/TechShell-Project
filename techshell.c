// Name(s): Carter Landry
// Description: A simple Unix-like shell for CSC 222.
// Supports command execution, cd, exit, and basic I/O redirection.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <errno.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

typedef struct {
    char *command;
    char *args[MAX_ARGS];
    char *inputFile;
    char *outputFile;
} ShellCommand;


/* Display prompt: current working directory + "$ " */
void displayPrompt() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s$ ", cwd);
        fflush(stdout);
    } else {
        perror("getcwd");
    }
}


/* Read input from user */
char* getInput() {
    char *buffer = malloc(MAX_INPUT);

    if (!fgets(buffer, MAX_INPUT, stdin)) {
        free(buffer);
        exit(0);
    }

    // remove trailing newline
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}


/* Parse input into command struct */
ShellCommand parseInput(char *input) {
    ShellCommand cmd;
    memset(&cmd, 0, sizeof(cmd));

    int argIndex = 0;

    char *token = strtok(input, " ");
    while (token != NULL) {

        if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            cmd.outputFile = token;
        }
        else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            cmd.inputFile = token;
        }
        else {
            cmd.args[argIndex++] = token;
        }

        token = strtok(NULL, " ");
    }

    cmd.args[argIndex] = NULL;

    if (cmd.args[0] != NULL)
        cmd.command = cmd.args[0];

    return cmd;
}


/* Execute parsed command */
void executeCommand(ShellCommand cmd) {

    if (cmd.command == NULL)
        return;

    /* exit */
    if (strcmp(cmd.command, "exit") == 0) {
        exit(0);
    }

    /* cd */
    if (strcmp(cmd.command, "cd") == 0) {

        // cd with no args -> home directory
        if (cmd.args[1] == NULL || cmd.args[1][0] == '\0') {
            char *home = getenv("HOME");
            if (home == NULL) {
                fprintf(stderr, "cd: HOME environment variable not set\n");
            } else {
                if (chdir(home) != 0)
                    perror("cd");
            }
        }
        else {
            if (chdir(cmd.args[1]) != 0)
                perror("cd");
        }
        return;
    }

    /* touch */
    if (strcmp(cmd.command, "touch") == 0) {

        if (cmd.args[1] == NULL) {
            fprintf(stderr, "touch: missing filename\n");
            return;
        }

        int fd = open(cmd.args[1],
                      O_WRONLY | O_CREAT,
                      0644);

        if (fd < 0)
            perror("touch");
        else
            close(fd);

        return;
    }

    /* mkdir */
    if (strcmp(cmd.command, "mkdir") == 0) {

        if (cmd.args[1] == NULL) {
            fprintf(stderr, "mkdir: missing directory name\n");
            return;
        }

        if (mkdir(cmd.args[1], 0755) != 0)
            perror("mkdir");

        return;
    }

    /* rmdir */
    if (strcmp(cmd.command, "rmdir") == 0) {

        if (cmd.args[1] == NULL) {
            fprintf(stderr, "rmdir: missing directory name\n");
            return;
        }

        if (rmdir(cmd.args[1]) != 0)
            perror("rmdir");

        return;
    }


    /* OTHERWISE: external command */

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {

        // Input redirection
        if (cmd.inputFile != NULL) {
            int fd = open(cmd.inputFile, O_RDONLY);
            if (fd < 0) {
                perror("open input");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Output redirection
        if (cmd.outputFile != NULL) {
            int fd = open(cmd.outputFile,
                          O_WRONLY | O_CREAT | O_TRUNC,
                          0644);
            if (fd < 0) {
                perror("open output");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd.command, cmd.args);
        perror("execvp");
        exit(errno);
    }
    else {
        wait(NULL);
    }
}


int main() {
    char *input;
    ShellCommand command;

    for (;;) {
        displayPrompt();
        input = getInput();
        command = parseInput(input);
        executeCommand(command);
        free(input);
    }

    return 0;
}