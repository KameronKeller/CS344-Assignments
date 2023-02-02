#include <stdio.h> // for printf(), fgets(), perror()
#include <stdlib.h> // for exit()
#include <string.h> // for strtok(), strcmp()
#include <unistd.h> // for fork(), execvp()
#include <sys/wait.h> // for wait()

void read_input(char *input, int max_input_size) {
    fgets(input, max_input_size, stdin); // stops after \n is found, but includes \n
    input[strcspn(input, "\n")] = 0; // get rid of the \n (https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input)
}

// Note to self: **commands is a pointer to a pointer of words
void parse_commands(char *input, char **commands) {
    int counter = 0;

    char *token = strtok(input, " \t\n\r");  // split on space, tab, newline, return
    while(token) {
        commands[counter] = token; // Append each word to an array
        token = strtok(NULL, " \t\n\r");
        counter++;
    }
    commands[counter] = NULL; // Terminate the array with NULL
}


void execute_commands(char *commands[]) {
    if (strcmp(commands[0], "cd") == 0) {
        int status_code = chdir(commands[1]);
        if (status_code == -1) {
            perror("cd");
        }
    } else if (strcmp(commands[0], "exit") == 0) {
        exit(0);
    } else {
        pid_t pid = fork();

        if (pid == 0) {
            int status_code = execvp(commands[0], commands);
            if (status_code == -1) {
                perror("execvp");
                exit(1);
            }
        }
    }

    wait(NULL);
}


int main(int argc, char const *argv[]) {
    const char PROMPT = '$';
    const int MAX_INPUT_SIZE = 100;
    const int MAX_INDIVIDUAL_WORDS = 128;

    (void)argc;  // Expression does nothing, but now argc & argv are "used"
    (void)argv;

    char input[MAX_INPUT_SIZE];
    char *commands[MAX_INDIVIDUAL_WORDS];

    while (1) {
        printf("%c ", PROMPT);
        fflush(stdout); // Force prompt to display
        read_input(input, MAX_INPUT_SIZE);
        parse_commands(input, commands);
        execute_commands(commands);
    }


    return 0;
}
