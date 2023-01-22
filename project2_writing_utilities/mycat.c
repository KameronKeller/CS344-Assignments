#include <fcntl.h> // open()
#include <unistd.h> // read(), write(), close()

int main(int argc, char const *argv[]) {
	const int STDIN = 0;
	const int STDOUT = 1;
	const int MAX_FILE_SIZE = 2048;
	const int TRUE = 1;
	const int NO_ARGS = 1;

	char buffer[MAX_FILE_SIZE];

	if (argc == NO_ARGS) {
		while (TRUE) {
			int bytesRead = read(STDIN, &buffer, MAX_FILE_SIZE);
			// EOF will have 0 bytes, an error will return -1
			if (bytesRead <= 0) {
				break;
			} else {
				write(STDOUT, &buffer, bytesRead);
			}
		}
	} else {
		for (int i = 1; i < argc; i++) {
			int fildes = open(argv[i], O_RDONLY);
			int bytesRead = read(fildes, &buffer, MAX_FILE_SIZE);
			write(STDOUT, &buffer, bytesRead);
			close(fildes);
		}
	}
	return 0;
}
