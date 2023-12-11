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
		ms->exit_code = 134;
		return (NULL);
	}
	ft_strlcpy(var, &arg[i + 1], j - i);
	value = getenv(var);
	if (!value)
		return (free(var), NULL);
	new_var = ft_strtrim(ft_strdup(value), " ");
	if (!new_var)
	{
		ms->exit_code = 134;
		return (free(var), NULL);
	}
	return (free(var), new_var);
}

char	*skip_dol(char *arg, int i, int j)
{
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
	return (new_arg);
}

char	*repl_dol(char *arg, char *var, int i, int j)
{
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

char	*keep_one_dol_only(t_ms *ms, char *arg, int i, t_list *dol)
{
	char	*new_arg;
	int		j;
	int		k;

	new_arg = NULL;
	j = i + 1;
	while (arg[j] && arg[j] == '$')
	{
		j++;
		dol = dol->next;
	}
	new_arg = ft_calloc(ft_strlen(arg) - (j - i) + 1, sizeof(char));
	if (!new_arg)
	{
		ms->exit_code = 134;
		return (free(arg), NULL);
	}
	k = -1;
	while (++k <= i)
		new_arg[k] = arg[k];
	while (arg[j])
		new_arg[k++] = arg[j++];
	return (free(arg), new_arg);
}

char	*expand_dol(t_ms *ms, char *arg, int dol_count, t_list *dol)
{
	int		i;
	int		j;
	int		cur_dol;
	char	*exp_arg;
	char	*var;

	exp_arg = NULL;
	var = NULL;
	i = 0;
	cur_dol = 0;
	while (arg[i] && (arg[i] != '$' || (arg[i] == '$' && cur_dol != dol_count)))
	{
		if (arg[i] == '$')
			cur_dol++;
		i++;
	}
	j = i + 1;
	if (!arg[j])
		return (arg);
	if (arg[j] == '$')
	{
		arg = keep_one_dol_only(ms, arg, i, dol);
		if (!arg)
			return (NULL);
	}
	if (arg[j] != '\"' || arg[j] != '\'')
		exp_arg = skip_dol(arg, i, j);
	else
	{
//		if (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\'')
//		{
		while (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\'')
			j++;
		var = get_varvalue(ms, arg, i, j);
//		}
		if (!var && ms->exit_code == 134)
			return (free(arg), NULL);
		else if (!var)
			exp_arg = skip_dol(arg, i, j);
		else
			exp_arg = repl_dol(arg, var, i, j);
	}
	if (!exp_arg)
	{
		ms->exit_code = 134;
		return (free(arg), NULL);
	}
	free(arg);
	return (exp_arg);
}

int	cmd_expand(t_ms *ms, char **args, t_list *dol)
{
	int	i;
	int	j;
	int	dol_count;

	i = -1;
	if (!args)
		return (0);
	while (args[++i])
	{
		j = -1;
		dol_count = 0;
		while (args[i][++j])
		{
			if (args[i][j] == '$')
			{
				if (dol && dol->n)
					args[i] = expand_dol(ms, args[i], dol_count, dol);
				if (!args[i])
					return (1);
				dol = dol->next;
				dol_count++;
			}
		}
	}
	return (0);
}
