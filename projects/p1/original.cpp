#include <iostream>

using namespace std;

int MyStrlen(char * s) {
	int retval = 0;
	while (*(s++) != 0)
		retval++;
	return retval;
}

int main(int argc, char * argv[]) {
	while (*argv) {
		cout << "[" << MyStrlen(*argv) << "] [" << *(argv++) << "]" << endl;
	}
	return 0;
}
