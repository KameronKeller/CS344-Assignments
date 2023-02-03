/*
Author: Kameron Keller
Course: CS 344 OS I
Assignment: Project 4
Example usage:

    $ ./lswc /usr/bin
        5685

    // Equivalent to:
    $ ls -1a | wc -l
*/


#include <stdio.h> // for printf(), perror()
#include <stdlib.h> // for exit()
#include <unistd.h> // for fork(), execlp(), close()

#define STDIN 0
#define STDOUT 1

void create_pipe(int pipe_fd[]) {
    int pipe_status = pipe(pipe_fd);
    if (pipe_status == -1)
        perror("pipe failure");
}

int no_path_specified(int argc) {
    return argc > 1 ? 0 : 1; // return false (0) if no additional arguments
}

char *get_path(int argc, char *argv[]) {
    char *current_directory = ".";
    if (no_path_specified(argc)) {
        return current_directory;
    } else {
        return argv[1];
    }
}

int is_child_process(int pid) {
    return pid == 0 ? 1 : 0; // return true (1) if pid is child (0)
}

void count_files(int read_pipe, int write_pipe) {
    dup2(read_pipe, STDIN);
    close(write_pipe);
    execlp("wc", "wc", "-l", NULL);
}

void list_files(int read_pipe, int write_pipe, char *path) {
    dup2(write_pipe, STDOUT);
    close(read_pipe);
    execlp("ls","ls", "-1a", path, NULL);
}

int main(int argc, char *argv[]) {
    int pipe_fd[2];

    char *path = get_path(argc, argv);

    create_pipe(pipe_fd);
    const int READ_PIPE = pipe_fd[0];
    const int WRITE_PIPE = pipe_fd[1];

    pid_t pid = fork();

    if (is_child_process(pid)) {
        count_files(READ_PIPE, WRITE_PIPE);
    } else {
        list_files(READ_PIPE, WRITE_PIPE, path);
        wait(NULL);
    }  

    return 0;
}

