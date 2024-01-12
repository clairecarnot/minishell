#include "../../include/exec.h" 

int	is_same_len(char *s1, char *s2)
{
	int	len;
	int	i;

	i = 0;
	len = ft_strlen(s1);
	while (s2[i] && s2[i] != '=')
		i++;
	if (i == len)
		return (1);
	return (0);
}

char	*ft_getenv(t_ms *ms, char *var)
{
//	dprintf(2, "var = %s\n", var);
	t_list	*tmp;
	char	*value;

	value = NULL;
	tmp = ms->env;
	while (tmp)
	{
		if (ft_strncmp(var, tmp->content, ft_strlen(var)) == 0
				&& is_same_len(var, tmp->content))
		{
			value = ft_strdup(tmp->content + ft_strlen(var) + 1);
			if (!value)
			{
				ms->exit_code = 255;
				return (NULL);
			}
			return (value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*exp_exitcode(t_ms *ms)
{
	char	*new_var;

	new_var = ft_itoa(ms->previous_exit_code);
	if (!new_var)
	{
		ms->exit_code = 255;
		return (NULL);
	}
	return (new_var);
}

int	isset(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*trim_beg(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		k;
	char	*d;

	i = 0;
	if (!s1 || !set)
		return (0x0);
	while (s1[i] && isset(s1[i], set))
		i++;
	j = ft_strlen(s1) - 1;
//	while (j > i && isset(s1[j], set))
//		j--;
	d = malloc(sizeof(char) * (j - i + 2));
	if (!d)
		return (0x0);
	k = 0;
	while (i <= j)
	{
		d[k] = s1[i];
		k++;
		i++;
	}
	d[k] = '\0';
	return (d);
}
int	count_consec_spc(char *value)
{
	int	i;
	int	j;
	int	consec_spc;

	i = 0;
	consec_spc = 0;
	while (value[i])
	{
		j = 1;
		if (value[i] == ' ')
		{
			while (value[i + j] && value[i + j] == ' ')
			{
				consec_spc++;
				j++;
			}
		}
		i += j;
	}
	return (consec_spc);
}

char	*ft_trimvar(char *value)
{
//	dprintf(2, "value = %s\n", value);
	char	*trim_val;
	char	*new_val;
	int	i;
	int	j;

	i = 0;
	j = 0;
	trim_val = NULL;
//	dprintf(2, "count = %lu\n", ft_strlen(value) - count_consec_spc(value) + 1);
	new_val = ft_calloc(ft_strlen(value) - count_consec_spc(value) + 1,
			sizeof(char));
	if (!new_val)
		return (NULL);
	while (value[i])
	{		
//		dprintf(2, "value[i] = %c", value[i]);
//		dprintf(2, "-\n");
		new_val[j] = value[i];
		if (value[i] == ' ')
		{
			while (value[i] && value[i] == ' ')
				i++;
			j++;
		}
		else
		{
			i++;
			j++;
		}
	}
	new_val[j] = '\0';
//	dprintf(2, "new_val = %s\n", new_val);
	trim_val = trim_beg(new_val, " ");
	if (!trim_val)
		return (free(new_val), NULL);
//	dprintf(2, "trim_val = %s\n", trim_val);
	return (free(new_val), trim_val);
}

char	*get_varvalue(t_ms *ms, char *arg, int i, int j)
{
	char	*var;
	char	*value;
	char	*new_var;

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
	new_var = ft_trimvar(value);
	if (!new_var)
	{
		ms->exit_code = 255;
		return (free(value), free(var), NULL);
	}
	return (free(value), free(var), new_var);
}

char	*skip_dol(char *arg, int i, int j, int data[5])
{
//	dprintf(2, "skip dol\n");
	char	*new_arg;
	int		k;

	k = 0;
	new_arg = NULL;
	new_arg = ft_calloc(ft_strlen(arg) - (j - i) + 1, sizeof(char));
	if (!new_arg)
		return (NULL);
	while (k < i)
	{
		new_arg[k] = arg[k];
		k++;
	}
	while (arg[j])
	{
		new_arg[k] = arg[j];
		j++;
		k++;
	}
	new_arg[k] = '\0';
	data[0] -= 1;
	data[1] = 1;
	return (new_arg);
}

char	*repl_dol(char *arg, char *var, int i, int j)
{
//	dprintf(2, "repl dol\n");
	char	*new_arg;
	int		k;

	k = -1;
	new_arg = NULL;
	new_arg = ft_calloc(ft_strlen(arg) - (j - i)
			+ ft_strlen(var) + 1, sizeof(char));
	if (!new_arg)
		return (NULL);
	while (++k < i)
		new_arg[k] = arg[k];
	while (*var)
	{
		new_arg[k] = *var;
		var++;
		k++;
	}
	while (arg[j])
	{
		new_arg[k] = arg[j];
		j++;
		k++;
	}
	new_arg[k] = '\0';
	return (new_arg);
}

char	*keep_one_dol_only(t_ms *ms, char *arg, int i, t_dol **dol)
{
//	dprintf(2, "keep one dol only\n");
	char	*new_arg;
	int		j;
	int		k;

	new_arg = NULL;
	j = i + 1;
	while (arg[j] && arg[j] == '$')
	{
		j++;
		(*dol)->d = (*dol)->d->next;
		(*dol)->c = (*dol)->c->next;
	}
	new_arg = ft_calloc(ft_strlen(arg) - (j - i) + 2, sizeof(char));
	if (!new_arg)
	{
		ms->exit_code = 255;
		return (free(arg), NULL);
	}
	k = -1;
	while (++k <= i)
		new_arg[k] = arg[k];
	while (arg[j])
		new_arg[k++] = arg[j++];
	new_arg[k] = '\0';
	return (free(arg), new_arg);
}

int	dol_standalone(char *arg, t_dol **dol)
{
//	dprintf(2, "dol stdalone\n");
	int	i;

	i = 0;
	if (arg[i] && arg[i] == ' ')
	{
		while (arg[i] && arg[i] == ' ')
			i++;
		if (!arg[i])
			return (1);
		else
			return (0);
	}
	if (arg[i] && arg[i] == '$')
	{
		while (arg[i] && arg[i] == '$')
			i++;
		while (arg[i] && arg[i] == ' ')
			i++;
		if (!arg[i])
			return (1);
		else
			return (0);
	}
	if ((*dol)->c->n == 0)
		return (1);
	return (0);
}

char	*expand_dol(t_ms *ms, char *arg, int data[5], t_dol **dol)
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
	var = get_varvalue(ms, arg, i, j);
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
		data[2] += ft_strlen(var) - 1;
		free(var);
	}
	data[0] -= 1;
//	dprintf(2, "exp_arg = %s\n", exp_arg);
	return (free(arg), exp_arg);
}

void	init_data(int data[5])
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
}

void	update_expand_pos(int data[5], int *i, int *j, t_dol **dol)
{
	if (data[3])
	{
		*i += 1;
		*j = data[4];
	}
	else
	{
		data[0] += 1;
		if (data[1] == 1)
			*j -= 1;
		if (data[2] > 0)
			*j += data[2];
	}
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	(*dol)->d = (*dol)->d->next;
	(*dol)->c = (*dol)->c->next;
}

int	contains_spc(char *arg, int j, int data[5])
{
	int	i;

	i = 0;
	if (data[1])
		return (0);
	while (arg[j] && i < data[2])
	{
		if (arg[j] == ' ')
			return (1);
		j++;
		i++;
	}
	return (0);
}

char	**redefine_args(t_cmd *cmd, int i, int j, int data[5])
{
	char	**new_args;
	char	*beg;
	char	*end;
	int	size;
	int	k;

	size = 0;
	beg = NULL;
	end = NULL;
	if (j == 0)
	{
		while (cmd->args[i][j] && cmd->args[i][j] != ' ')
			j++;
	}
	beg = ft_calloc(j + 1, sizeof(char));
	if (!beg)
		return (NULL);
	ft_strlcpy(beg, cmd->args[i], j);
	end = ft_calloc(ft_strlen(&cmd->args[i][j + 1]) + 1, sizeof(char));
	if (!end)
		return (free(beg), NULL);
	ft_strlcpy(beg, &cmd->args[i][j + 1], ft_strlen(&cmd->args[i][j + 1]));
	while (cmd->args[size])
		size++;
	new_args = ft_calloc(size + 2, sizeof(char *));
	if (!new_args)
		return (free(beg), free(end), NULL);
	k = 0;
	while (k < i)
	{
		new_args[k] = ft_strdup(cmd->args[k]);
		if (!new_args[k])
			return (free_tab(new_args), free(beg), free(end), NULL);
		k++;
	}
	new_args[k++] = beg;
	new_args[k++] = end;
	while (k < size)
	{
		new_args[k] = ft_strdup(cmd->args[k - 1]);
		if (!new_args[k])
			return (free_tab(new_args), NULL);
		k++;
	}
	new_args[k] = NULL;
	data[3] = 1;
	data[4] = ft_strlen(cmd->args[i]) - (data[2] - ft_strlen(new_args[i]));
	return (free_tab(cmd->args), new_args);
}

int	cmd_expand(t_ms *ms, t_cmd *cmd, t_dol *dol)
{
	int	i;
	int	j;
	int	data[5];
	//data[0] = dol_count
	//data[1] = 1 means a dol has been skipped and we need to go back from 1 char 
	// in the loop (j -= 1)
	//data[2] = x means a $ has been replaced and we move from x chars since they are
	//not to be analyzed in the loop
	//data[3] = 1 means the list of args has been redefined
	//data[4] = x gives the new start of j, after the list of args has been redefined

	i = -1;
	if (!cmd->args)
		return (0);
	while (cmd->args[++i])
	{
		j = -1;
		init_data(data);
		while (cmd->args[i][++j])
		{
//			dprintf(2, "cmd->args[i] = %s\n", cmd->args[i]);
//			dprintf(2, "cmd->args[i][j] = %c\n", cmd->args[i][j]);
//			dprintf(2, "dol->d->n = %d\n", dol->d->n);
			if (cmd->args[i][j] == '$')
			{
//				if (!dol->d)
//					dprintf(2, "dol->d n'existe pas\n");
				if (dol->d->n)
				{
//					dprintf(2, "args[i] = %s\n", args[i]);
//					dprintf(2, "args[i][j] = %c\n", args[i][j]);
//					dprintf(2, "dol->d->n = %d\n", dol->d->n);
					if (i == 0)
					{
						cmd->args[i] = expand_dol_first(ms, cmd->args[i], data, &dol);
						if (!cmd->args[i])
							return (1);
						if (contains_spc(cmd->args[i], j, data))
							cmd->args = redefine_args(cmd, i, j, data);
						if (!cmd->args)
							return (1);
					}
					else
					{
						cmd->args[i] = expand_dol(ms, cmd->args[i], data, &dol);
						if (!cmd->args[i])
							return (1);
					}
				}
				update_expand_pos(data, &i, &j, &dol);
			}
		}
	}
//	dprintf(2, "return 0\n");
	return (0);
}
