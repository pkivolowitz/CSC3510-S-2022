#include <stdio.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char ** argv) {
	int retval = 1;
	char * dirname = ".";

	if (argc > 1)
		dirname = argv[1];

	DIR * dir = opendir(dirname);
	if (dir) {
		struct dirent * de;
		errno = 0;
		while ((de = readdir(dir)) != NULL)
			printf("%-20llu 0x%02x %s\n", de->d_ino, de->d_type, de->d_name);
		if (errno != 0)
			perror("readdir() failed");
		closedir(dir);
		retval = (errno != 0); // force error return to be 1
	}
	else
		perror(dirname);
	return retval;
}