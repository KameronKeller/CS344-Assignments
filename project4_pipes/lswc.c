#include <stdio.h> // for printf(), perror()
#include <stdlib.h> // for exit()
#include <unistd.h> // for fork(), execlp(), close()

int no_arguments(int argc) {
    return argc > 1? 0 : 1; // return 0 if no additional arguments
}

const char *parse_input(int argc, const char *argv[]) {
// char *parse_input(int argc) {
    if (no_arguments(argc)) {
        return ".";
    } else {
        return argv[1];
    }
}


int main(int argc, const char *argv[]) {
    int pipe_fd[2];


    const char *path = parse_input(argc, argv);
    // printf("%s\n", path);
    // create_pipe()
    int pipe_status = pipe(pipe_fd);
    if (pipe_status == -1)
        perror("pipe failure");

    pid_t pid = fork();
    // if child_process(pid) {}
    // else {
        // wait(NULL);
    // }  

    return 0;
}

