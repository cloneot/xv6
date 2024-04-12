#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	const char* sid = "2022xxx287";
	printf(1, "My student id is %s\n", sid);
	printf(1, "My pid is %d\n", getpid());
	printf(1, "My gpid is %d\n", getgpid());
	exit();
}
