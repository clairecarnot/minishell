#include "../../include/exec.h" 

char	*expand_dol(t_ms *ms, char *arg, int data[5], t_dol **dol)
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
	var = get_varvalue(ms, arg, i, j);
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

int	init_data(int data[5], int *j)
{
	*j = -1;
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	return (1);
}

void	update_expand_pos(int data[5], int *i, int *j, t_dol **dol)
{
	if (data[3] == 1)
	{
		*i += 1;
		*j = data[4] - 1;
	}
	else if (data[3] == 2)
	{
		*i -= 1;
		*j = data[4];
	}
	else
	{
		data[0] += 1;
		if (data[1] == 1)
			*j -= 1;
		if (data[2] > 0)
			*j += data[2] - 1;
	}
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
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

int	cmd_expand(t_ms *ms, t_cmd *cmd, t_dol *dol)
{
	int	i;
	int	j;
	int	data[5];

	i = -1;
	while (cmd->args[++i] && init_data(data, &j))
	{
		dprintf(2, "i = %d\n", i);
		dprintf(2, "cmd->args[i] = %s\n", cmd->args[i]);
		while (cmd->args[i][++j])
		{
			dprintf(2, "j = %d\n", j);
			dprintf(2, "cmd->args[i][j] = %c\n", cmd->args[i][j]);
			if (cmd->args[i][j] == '$')
			{
				if (dol->d->n)
				{
					if (i == 0)
						cmd->args[i] = expfirst(ms, cmd->args[i], data, &dol);
					else
						cmd->args[i] = expand_dol(ms, cmd->args[i], data, &dol);
					if (args_redef(cmd, i, j, data) == 1)
						return (ms->exit_code = 255, 1);
				}
				update_expand_pos(data, &i, &j, &dol);
				dprintf(2, "i redef = %d\n", i);
				dprintf(2, "j redef = %d\n", j);
			}
		}
	}
	return (0);
}
