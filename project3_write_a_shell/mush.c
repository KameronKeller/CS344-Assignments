#include <stdio.h> // for printf(), fgets(), perror()
// #include <stdlib.h> // for exit()
#include <string.h> // for strtok(), strcmp()
// #include <unistd.h> // for fork(), execvp()
// #include <sys/wait.h> // for wait()

int main(int argc, char const *argv[]) {
    const char PROMPT = '$';
    const int MAX_INPUT_SIZE = 100;

    (void)argc;  // Expression does nothing, but now argc is "used"
    (void)argv;

    char input[MAX_INPUT_SIZE];

    while (1) {
        // Print a prompt
        printf("%c ", PROMPT);

        // Read a line
        fgets(input, MAX_INPUT_SIZE, stdin); // stops after \n is found, but includes \n
        input[strcspn(input, "\n")] = 0; // get rid of the \n (https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input)
        
        // Parse it down to individual words
        char *token = strtok(input, " ");
        while(token) {
            printf("token = %s\n", token);
            token = strtok(NULL, " ");
        }
        


    }


    return 0;
}
