#include "../../include/exec.h" 

char	*get_varvalue_redir(t_ms *ms, char *arg, int i, int j)
{
	char	*var;
	char	*value;

	value = NULL;
	if (arg[i + 1] == '?')
		return (exp_exitcode(ms));
	var = ft_calloc(j - i, sizeof(char));
	if (!var)
	{
		ms->exit_code = 255;
		return (NULL);
	}
	ft_strlcpy(var, &arg[i + 1], j - i);
	value = ft_getenv(ms, var);
	if (!value)
		return (free(var), NULL);
	return (free(var), value);
}

char	*expand_dol_redir(t_ms *ms, char *arg, int data[5], t_dol **dol)
{
	int		i;
	int		j;
	char	*exp_arg;
	char	*var;

	i = find_cur_dol(arg, data);
	if (!arg[i] || !arg[i + 1] || dol_standalone(&arg[i + 1], dol))
		return (arg);
	if (arg[i + 1] == '$')
		arg = keep_one_dol_only(ms, arg, i, dol);
	if (!arg)
		return (NULL);
	j = delimitate_var(arg, i, dol);
	var = get_varvalue_redir(ms, arg, i, j);
	if (!var && ms->exit_code == 255)
		return (free(arg), NULL);
	else if (!var)
		exp_arg = skip_dol(arg, i, j, data);
	else
		exp_arg = repl_dol(arg, var, i, j);
	if (!exp_arg)
		return (ms->exit_code = 255, free_if(var), free(arg), NULL);
	if (var)
		data[2] += ft_strlen(var);
	return (data[0] -= 1, free_if(var), free(arg), exp_arg);
}

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

/*
data[0] = dol_count
data[1] = 1 means a dol has been skipped and we need to go back from 1 char 
 in the loop (j -= 1)
data[2] = x means a $ has been replaced and we move from x chars since they are
 not to be analyzed in the loop
data[3] = 1 means the list of args has been redefined
data[4] = x gives the new start of j, after the list of args has been redefined
*/

char	*expand_redir(t_ms *ms, char *arg, t_dol *dol)
{
	int	j;
	int	data[5];

	if (!arg)
		return (NULL);
	j = -1;
	init_data(data, &j);
	while (arg[++j])
	{
		if (arg[j] == '$')
		{
			if (dol->d->n)
			{
				arg = expand_dol_redir(ms, arg, data, &dol);
				if (!arg)
					return (NULL);
			}
			r_update_expand_pos(data, &j, &dol);
		}
	}
	return (arg);
}
