#include "../../include/exec.h"
#include "../../include/signals.h"

void	save_ptrs(t_list **s1, t_list **s2, t_list **t1, t_list **t2)
{
	*t1 = *s1;
	*t2 = *s2;
}
