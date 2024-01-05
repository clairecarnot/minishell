#include "../../include/exec.h" 

char	*get_varvalue(t_ms *ms, char *arg, int i, int j)
{
	char	*var;
	char	*value;
	char	*new_var;

	value = NULL;
	var = ft_calloc(j - i, sizeof(char));
	if (!var)
	{
		ms->exit_code = 255;
		return (NULL);
	}
	ft_strlcpy(var, &arg[i + 1], j - i);
	value = getenv(var);
	if (!value)
		return (free(var), NULL);
	value = ft_strdup(value);
	if (!value)
	{
		ms->exit_code = 255;
		return (free(var), NULL);
	}
	new_var = ft_strtrim(value, " ");
	if (!new_var)
	{
		ms->exit_code = 255;
		return (free(value), free(var), NULL);
	}
	return (free(value), free(var), new_var);
}

char	*skip_dol(char *arg, int i, int j, int data[2])
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

char	*expand_dol(t_ms *ms, char *arg, int data[2], t_dol **dol)
{
//	dprintf(2, "expand dol1\n");
//	dprintf(2, "%s\n", arg);
	int		i;
	int		j;
	char	*exp_arg;
	char	*var;
	int		dol_nb;

	dol_nb = data[0];
	exp_arg = NULL;
	var = NULL;
	i = 0;
	while (arg[i] && (arg[i] != '$' || (arg[i] == '$' && dol_nb)))
	{
		if (arg[i] == '$')
			dol_nb--;
		i++;
	}
//	dprintf(2, "i = %d\n", i);
//	dprintf(2, "expand dol2\n");
//	dprintf(2, "%c\n", arg[i]);
	if (!arg[i] || !arg[i + 1] || dol_standalone(&arg[i + 1], dol))
		return (arg);
//	dprintf(2, "%s\n", arg);
//	dprintf(2, "arg[%d] = %c\n", i, arg[i]);
	if (arg[i + 1] == '$')
		arg = keep_one_dol_only(ms, arg, i, dol);
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
		free(var);
	return (free(arg), exp_arg);
}

void	init_data(int data[2])
{
	data[0] = 0;
	data[1] = 0;
}

void	update_expand_pos(int data[2], int *j, t_dol **dol)
{
	data[0] += 1;
	if (data[1] == 1)
		*j -= 1;
	data[1] = 0;
	(*dol)->d = (*dol)->d->next;
	(*dol)->c = (*dol)->c->next;
}

int	cmd_expand(t_ms *ms, char **args, t_dol *dol)
{
	int	i;
	int	j;
	int	data[2];
	//data[0] = dol_count
	//data[1] = 1 means a dol has been skipped and we need to go back from 1 char 
	// in the loop (j -= 1)

	i = -1;
	if (!args)
		return (0);
	while (args[++i])
	{
		j = -1;
		init_data(data);
		while (args[i][++j])
		{
			if (args[i][j] == '$')
			{
				if (!dol->d)
					dprintf(2, "dol->d n'existe pas\n");
				if (dol->d->n)
					args[i] = expand_dol(ms, args[i], data, &dol);
				if (!args[i])
					return (1);
//				dol = dol->next;
//				dol_count++;
				update_expand_pos(data, &j, &dol);
			}
		}
	}
	return (0);
}
