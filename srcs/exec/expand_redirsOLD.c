#include "../../include/exec.h" 

void	r_update_expand_pos(int data[5], int *j, t_dol **dol)
{
	data[0] += 1;
	if (data[1] == 1)
		*j -= 1;
	if (data[2] > 0)
		*j += data[2] - 1;
	data[1] = 0;
	data[2] = 0;
	(*dol)->d = (*dol)->d->next;
	(*dol)->c = (*dol)->c->next;
}

char	*expand_redir(t_ms *ms, char *arg, t_dol *dol)
{
	int	j;
	int	data[5];
	//data[0] = dol_count
	//data[1] = 1 means a dol has been skipped and we need to go back from 1 char 
	// in the loop (j -= 1)
	//data[2] = x means a $ has been replaced and we move from x chars since they are
	//not to be analyzed in the loop
	//data[3] = 1 means the list of args has been redefined
	//data[4] = x gives the new start of j, after the list of args has been redefined

	if (!arg)
		return (NULL);
	j = -1;
	init_data(data);
	while (arg[++j])
	{
		if (arg[j] == '$')
		{
//			dprintf(2, "arg = %s\n", arg);
//			dprintf(2, "arg[j] = %c\n", arg[j]);
			if (dol->d->n)
			{
				arg = expand_dol(ms, arg, data, &dol);
				if (!arg)
					return (NULL);
			}
			r_update_expand_pos(data, &j, &dol);
//			dprintf(2, "j = %d\n", j);
		}
	}
//	dprintf(2, "arg = %s\n", arg);
	return (arg);
}
