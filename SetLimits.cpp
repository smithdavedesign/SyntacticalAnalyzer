// Resource limits the course test harness used: at most 20 s of CPU time and 10 MB per
// output file, so a parser stuck in a loop stops instead of filling the disk.
#include <sys/resource.h>
#include "SetLimits.h"

static void Cap (int resource, rlim_t limit)
{
	struct rlimit current;
	if (getrlimit (resource, &current) != 0)
		return;
	if (current.rlim_max != RLIM_INFINITY && limit > current.rlim_max)
		limit = current.rlim_max;
	current.rlim_cur = limit;
	setrlimit (resource, &current);
}

void SetLimits ()
{
	Cap (RLIMIT_CPU, 20);
	Cap (RLIMIT_FSIZE, 10000000);
}
