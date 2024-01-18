#include "../../include/exec.h" 

char	*get_varvalue_redir(t_ms *ms, char *arg, int i, int j)
{
	char	*var;
	char	*value;
//	char	*new_var;

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
//	new_var = ft_trimvar(value);
//	if (!new_var)
//	{
//		ms->exit_code = 255;
//		return (free(value), free(var), NULL);
//	}
//	return (free(value), free(var), new_var);
	return (free(var), value);
}

char	*expand_dol_redir(t_ms *ms, char *arg, int data[5], t_dol **dol)
{
//	dprintf(2, "expand dol1\n");
//	dprintf(2, "arg = %s\n", arg);
//	dprintf(2, "dol_nb =  %d\n", data[0]);
	int		i;
	int		j;
	char	*exp_arg;
	char	*var;
	int		dol_nb;

	dol_nb = data[0];
	exp_arg = NULL;
	var = NULL;
	i = 0;
	while (arg[i] && (arg[i] != '$' || (arg[i] == '$' && dol_nb > 0)))
	{
		if (arg[i] == '$')
			dol_nb--;
		i++;
	}
//	dprintf(2, "i = %d\n", i);
//	dprintf(2, "expand dol2\n");
//	dprintf(2, "%c\n", arg[i]);
	if (!arg[i] || !arg[i + 1] || dol_standalone(&arg[i + 1], dol))
//	{
//		dprintf(2, "ici\n");
		return (arg);
//	}
//	dprintf(2, "%s\n", arg);
//	dprintf(2, "arg[%d] = %c\n", i, arg[i]);
	if (arg[i + 1] == '$')
		arg = keep_one_dol_only(ms, arg, i, dol);
//	dprintf(2, "expand dol2 - 2\n");
	if (!arg)
		return (NULL);
//	dprintf(2, "arg[%d] = %c\n", i, arg[i]);
//	dprintf(2, "expand dol3\n");
//	dprintf(2, "%s\n", arg);
	j = i + 1;
	while (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\''
			&& (*dol)->c->n--)
		j++;
	var = get_varvalue_redir(ms, arg, i, j);
//	dprintf(2, "var = %s\n", var);
	if (!var && ms->exit_code == 255)
		return (free(arg), NULL);
	else if (!var)
		exp_arg = skip_dol(arg, i, j, data);
	else
		exp_arg = repl_dol(arg, var, i, j);
//	dprintf(2, "expand dol4\n");
//	dprintf(2, "%s\n", exp_arg);
	if (!exp_arg)
	{
		ms->exit_code = 255;
		if (var)
			free(var);
		return (free(arg), NULL);
	}
	if (var)
	{
		data[2] += ft_strlen(var);
		free(var);
	}
	data[0] -= 1;
//	dprintf(2, "exp_arg = %s\n", exp_arg);
	return (free(arg), exp_arg);
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
				arg = expand_dol_redir(ms, arg, data, &dol);
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
