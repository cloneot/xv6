#include "types.h"
#include "user.h"

void myassert(int condition, char* message) {
	if(!condition) {
		printf(1, "(%s) test failed\n", message);
		exit();
	}
	printf(1, "(%s) OK\n", message);
}

int main(int argc, char *argv[])
{
	printf(1, "[mytest (%s)] starts\n\n", argv[1]);

	if(!strcmp(argv[1], "sleep_long")) {
		for(int i = 10; i <= 1000; i *= 10) {
			printf(1, "Sleeping for %d ticks\n", i);
			sleep(i);
		}
	}
	else if(!strcmp(argv[1], "exception")) {
		printf(1, "[setpriority]\n");
		myassert(setpriority(-1, 0) == -1, "invalid pid");
		myassert(setpriority(1, -1) == -2, "invalid priority");
		printf(1, "\n");

		printf(1, "[setmonopoly]\n");
		int sid = 2022095287;
		int pid = getpid();
		myassert(setmonopoly(-1, sid) == -1, "invalid pid");
		myassert(setmonopoly(1, -1) == -2, "invalid password");
		myassert(setmonopoly(pid, sid) == -4, "setmonopoly itself");
		if((pid = fork()) == 0) {
			// printf(1, "child: pid(%d)\n", getpid());
			while(1);
			exit();
		}
		else {
			sleep(10);
			// printf(1, "parent: child pid(%d)\n", pid);
			myassert(setmonopoly(pid, sid) == 1, "setmonopoly call");
			myassert(setmonopoly(pid, sid) == -3, "duplicated setmonopoly call");
			kill(pid);
		}
	}
	else if(!strcmp(argv[1], "divide")) {
		// mytest divide 0
		int x = argv[2][0] - '0';
		printf(1, "1 / x: %d\n", 1 / x);
	}

	printf(1, "[mytest (%s)] OK\n", argv[1]);
	exit();
}
