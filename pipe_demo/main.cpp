#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

const int READ_SIDE = 0;
const int WRITE_SIDE = 1;

int main() {
	int pipe_fds[2];
	int retval = 0;
	size_t bytes_read = 0;		// no need to initialize this
	char *buffer = nullptr;		// no need to initialize this
	pid_t pid;					// no need to initialize this

	try {
		if (pipe(pipe_fds) < 0) {
			perror("Failed to create pipe.");
			throw -1;
		}
		if ((pid = fork()) == 0) {
			// I am the child.
			string path("/bin/ls");
			close(pipe_fds[READ_SIDE]);						// We will NOT be using the read side in the child.
			close(STDOUT_FILENO);							// Prepare to make the write side of the pipe my stdout.
			dup(pipe_fds[WRITE_SIDE]);						// "make it so"
			close(pipe_fds[WRITE_SIDE]);					// Clean up after ourselves.
			execl(path.c_str(), path.c_str(), "-l", 0);		// Try to become "/bin/ls"
			perror("Failed to exec.");						// Ruhro.
			throw -1;
		} else if (pid > 0) {
			// I am the parent.
			close(pipe_fds[WRITE_SIDE]);					// Parent will not use the WRITE_SIDE so close it now.
			close(STDIN_FILENO);							// Optionally redirect standard input in the parent.
			dup(pipe_fds[READ_SIDE]);						// ditto.
			close(pipe_fds[READ_SIDE]);						// clean up.
			printf("Parent of child %d is ready to read from the pipe\n", pid);
			while (getline(&buffer, &bytes_read, stdin) != -1) {
				printf("%s", buffer);
			}
			printf("Parent of child %d is done reading from the pipe\n", pid);
		} else {
			perror("Failed to fork.");
			throw -1;
		}
	} catch (int r) {
		retval = r;
	}
	system("banner Foo Bar");
	return retval;
}