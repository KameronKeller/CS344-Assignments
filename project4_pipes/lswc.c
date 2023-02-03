#include <stdio.h> // for printf(), perror()
#include <stdlib.h> // for exit()
#include <unistd.h> // for fork(), execlp(), close()

#define STDIN 0
#define STDOUT 1


int no_arguments(int argc) {
    return argc > 1 ? 0 : 1; // return 0 if no additional arguments
}

char *parse_input(int argc, char *argv[]) {
    if (no_arguments(argc)) {
        return ".";
    } else {
        return argv[1];
    }
}

int is_child_process(int pid) {
    return pid == 0 ? 1 : 0;
}

void count_files(int pipe_fd[]) {
    const int READ_PIPE = pipe_fd[0];
    const int WRITE_PIPE = pipe_fd[1];

    // dup2(STDIN, READ_PIPE); // original
    dup2(READ_PIPE, STDIN);
    close(WRITE_PIPE);
    execlp("wc", "wc", "-l", NULL);
}

void list_files(int pipe_fd[], char *path) {
    const int READ_PIPE = pipe_fd[0];
    const int WRITE_PIPE = pipe_fd[1];

    // dup2(STDOUT, WRITE_PIPE); // original
    dup2(WRITE_PIPE, STDOUT);
    close(READ_PIPE);
    execlp("ls","ls", "-1a", path, NULL);

}

int main(int argc, char *argv[]) {
    int pipe_fd[2];


    char *path = parse_input(argc, argv);
    // printf("%s\n", path);
    // create_pipe()
    int pipe_status = pipe(pipe_fd);
    if (pipe_status == -1)
        perror("pipe failure");

    pid_t pid = fork();
    if (is_child_process(pid)) {
        count_files(pipe_fd);
    } else {
        list_files(pipe_fd, path);
        wait(NULL);
    }  

    return 0;
}

