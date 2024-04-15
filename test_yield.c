#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	int pid;
	if((pid = fork()) < 0) {
		printf(1, "fork failed\n");
		exit();
	}
	const char* msg = (pid == 0) ? "Child" : "Parent";
	while(1) {
		printf(1, "%s\n", msg);
		yield();
	}
	exit();
}
