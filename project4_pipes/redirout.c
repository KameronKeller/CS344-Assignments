/*
Author: Kameron Keller
Course: CS 344 OS I
Assignment: Project 4
Example usage:

    $ redirout foo.txt ls -l -a

*/

#include <stdio.h> // for printf(), perror()
#include <stdlib.h> // for exit()
#include <unistd.h> // for fork(), execlp(), close()
#include <fcntl.h> // for open()

#define STDIN 0
#define STDOUT 1

void verify_correct_num_args(int argc) {
    if (argc < 3) {
        printf("Usage: redirout <file_path> <command> [options]\n");
        exit(EXIT_FAILURE);
    }
}

void create_pipe(int pipe_fd[]) {
    int pipe_status = pipe(pipe_fd);
    if (pipe_status == -1)
        perror("pipe failure");
}

int is_child_process(int pid) {
    return pid == 0 ? 1 : 0; // return true (1) if pid is child (0)
}

void run_command(char *argv[], int pipe_input, int pipe_output) {
    dup2(pipe_output, STDOUT);
    close(pipe_input);
    execvp(argv[2], &argv[2]);
}

void write_to_file(int output_fd, int pipe_input, int pipe_output) {
    const int BUFFER_SIZE = 1024;
    int bytes_read = 0;
    char *buffer[BUFFER_SIZE];
    close(pipe_output);
    // while (reading_file(pipe_input, buffer, BUFFER_SIZE)) {
    while ((bytes_read = read(pipe_input, buffer, BUFFER_SIZE)) > 0) {
        write(output_fd, buffer, bytes_read);
    }
    close(pipe_input);
    close(output_fd);

}

int main(int argc, char *argv[]) {
    int pipe_fd[2];
    char *file_path = argv[1];

    verify_correct_num_args(argc);
    int output_fd = open(file_path, O_WRONLY|O_CREAT, 0644);
    create_pipe(pipe_fd);
    const int PIPE_INPUT = pipe_fd[0];
    const int PIPE_OUTPUT = pipe_fd[1];

    pid_t pid = fork();

    if (is_child_process(pid)) {
        run_command(argv, PIPE_INPUT, PIPE_OUTPUT);
    } else {
        write_to_file(output_fd, PIPE_INPUT, PIPE_OUTPUT);
        wait(NULL);
    }  

    return 0;
}
