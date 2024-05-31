#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_countfp(void)
{
	return countfp();
}

int
sys_countvp(void)
{
	return countvp();
}

int
sys_countpp(void)
{
	return countpp();
}

int
sys_countptp(void)
{
	return countptp();
}
