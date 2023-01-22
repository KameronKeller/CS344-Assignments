#include <fcntl.h> // open()
#include <unistd.h> // read(), write(), close()
#include <ctype.h> //toupper()
#include <stdio.h> //perror()
#include <stdlib.h> //exit()

void convert_buffer(int bytesRead, char *buffer) {
	for (int i = 0; i < bytesRead; i++) {
		buffer[i] = toupper(buffer[i]);
	}
}

int main(int argc, char const *argv[]) {
	const int STDIN = 0;
	const int STDOUT = 1;
	const int MAX_BUFFER_SIZE = 2048;
	const int NO_ARGS = 1;

	char buffer[MAX_BUFFER_SIZE];
	int bytesRead = 0;


	if (argc == NO_ARGS) {
		while ((bytesRead = read(STDIN, &buffer, MAX_BUFFER_SIZE)) != 0) {
			convert_buffer(bytesRead, buffer);
			write(STDOUT, &buffer, bytesRead);
		}
	} else {
		for (int i = 1; i < argc; i++) {
			int fildes = open(argv[i], O_RDONLY);

			if (fildes == -1) {
				perror("open");
				exit(1);
			}

			while ((bytesRead = read(fildes, &buffer, MAX_BUFFER_SIZE)) != 0) {
				convert_buffer(bytesRead, buffer);
				write(STDOUT, &buffer, bytesRead);
			}
			close(fildes);
		}
	}
	return 0;
}
