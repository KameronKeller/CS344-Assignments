#include <stdio.h> // for printf(), fgets(), perror()
#include <stdlib.h> // for exit()
#include <string.h> // for strtok(), strcmp()
#include <unistd.h> // for fork(), execvp()
#include <sys/wait.h> // for wait()


void execute_commands(char *commands[]) {
    pid_t pid = fork();

    if (pid == 0) {
        int status_code = execvp(commands[0], commands);
        if (status_code == -1) {
            perror("execute_commands");
            exit(1);
        }
    }
    wait(NULL);
}


int main(int argc, char const *argv[]) {
    const char PROMPT = '$';
    const int MAX_INPUT_SIZE = 100;
    const int MAX_INDIVIDUAL_WORDS = 128;

    (void)argc;  // Expression does nothing, but now argc is "used"
    (void)argv;

    char input[MAX_INPUT_SIZE];
    char *commands[MAX_INDIVIDUAL_WORDS];

    while (1) {
        // Print a prompt
        printf("%c ", PROMPT);

        // Read a line
        fgets(input, MAX_INPUT_SIZE, stdin); // stops after \n is found, but includes \n
        input[strcspn(input, "\n")] = 0; // get rid of the \n (https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input)
        
        // Parse the prompt down to individual words
        char *token = strtok(input, " ");

        // Count the number of words
        int counter = 0;

        while(token) {
            commands[counter] = token; // Append each word to an array
            // printf("token = %s\n", token);
            token = strtok(NULL, " ");
            counter++;
        }

        commands[counter] = NULL; // Terminate the array with NULL

        // execvp(commands[0], commands);
        execute_commands(commands);

        // Verify array was filled correctly
        // for (int i = 0; i < counter + 1; i++) {
        //     printf("command %d: %s\n", i, commands[i]);
        // }

    }


    return 0;
}
