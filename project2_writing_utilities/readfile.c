#include <fcntl.h> // open()
#include <unistd.h> // read(), write(), close()

int main(int argc, char const *argv[]) {
	const int MAX_FILE_SIZE = 2048;
	
	const int STDOUT = 1;
	char buffer[MAX_FILE_SIZE];

	int fildes = open(argv[1], O_RDONLY);
	// Note to self:	read() expects a pointer to a buffer,
	//					so must use address-of operator
	int bytesRead = read(fildes, &buffer, MAX_FILE_SIZE);
	write(STDOUT, &buffer, bytesRead);
	close(fildes);
	return 0;
}
