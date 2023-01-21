#include <fcntl.h> // open()
#include <unistd.h> // read(), write(), close()

int main(int argc, char const *argv[]) {
	const int MAX_FILE_SIZE = 2048;
	
	int stdout = 1;
	char buffer[MAX_FILE_SIZE];

	int fildes = open(argv[1], O_RDONLY);
	// Note to self:	read() expects a pointer to a buffer,
	//					so must use address-of operator
	int bytes_read = read(fildes, &buffer, MAX_FILE_SIZE);
	write(stdout, &buffer, bytes_read);
	close(fildes);
	return 0;
}
