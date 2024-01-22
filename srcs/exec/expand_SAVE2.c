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
	char	*trim_val;
	char	*v;
	int		i;
	int		j;

	i = 0;
	j = 0;
	v = ft_calloc(ft_strlen(value) - count_consec_spc(value) + 1, sizeof(char));
	if (!v)
		return (NULL);
	while (value[i])
	{		
		v[j] = value[i];
		if (value[i] == ' ')
		{
			while (value[i] && value[i] == ' ')
				i++;
		}
		else
			i++;
		j++;
	}
	v[j] = '\0';
	trim_val = trim_beg(v, " ");
	return (free(v), trim_val);
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

int	dol_standalone_return(char c)
{
	if (!c)
		return (1);
	else
		return (0);
}

int	dol_standalone(char *arg, t_dol **dol)
{
	int	i;

	i = 0;
	if (arg[i] && arg[i] == ' ')
	{
		while (arg[i] && arg[i] == ' ')
			i++;
		return (dol_standalone_return(arg[i]));
	}
	if (arg[i] && arg[i] == '$')
	{
		while (arg[i] && arg[i] == '$')
			i++;
		while (arg[i] && arg[i] == ' ')
			i++;
		return (dol_standalone_return(arg[i]));
	}
	if ((*dol)->c->n == 0)
		return (1);
	return (0);
}

int	find_cur_dol(char *arg, int data[5])
{
	int	i;
	int	dol_nb;

	i = 0;
	dol_nb = data[0];
	while (arg[i] && (arg[i] != '$' || (arg[i] == '$' && dol_nb > 0)))
	{
		if (arg[i] == '$')
			dol_nb--;
		i++;
	}
	return (i);
}

int	delimitate_var(char *arg, int i, t_dol **dol)
{
	int	j;

	j = i + 1;
	while (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\''
		&& (*dol)->c->n--)
		j++;
	return (j);
}

void	free_if(char *str)
{
	if (str)
		free(str);
}

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
	if (data[3])
	{
		*i += 1;
		*j = data[4] - 2;
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

int	tab_size(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**redefine_args_bis(t_cmd *cmd, char *d[2], char **new_args, int i)
{
	int		size;
	int		k;

	size = tab_size(cmd->args);
	k = 0;
	while (k < i)
	{
		new_args[k] = ft_strdup(cmd->args[k]);
		if (!new_args[k])
			return (free_tab(new_args), free(d[0]), free(d[1]), NULL);
		k++;
	}
	new_args[k++] = d[0];
	new_args[k++] = d[1];
	while (k < size)
	{
		new_args[k] = ft_strdup(cmd->args[k - 1]);
		if (!new_args[k])
			return (free_tab(new_args), NULL);
		k++;
	}
	new_args[k] = NULL;
	return (new_args);
}

char	**redefine_args(t_cmd *cmd, int i, int j, int data[5])
{
	char	**new_args;
	char	*d[2];
	int		size;

	size = tab_size(cmd->args);
	while (cmd->args[i][j] && cmd->args[i][j] != ' ')
		j++;
	if (j == 0)
		return (cmd->args);
	d[0] = ft_calloc(j + 2, sizeof(char));
	if (!d[0])
		return (NULL);
	ft_strlcpy(d[0], cmd->args[i], j + 1);
	d[1] = ft_calloc(ft_strlen(&cmd->args[i][j + 1]) + 1, sizeof(char));
	if (!d[1])
		return (free(d[0]), NULL);
	ft_strlcpy(d[1], &cmd->args[i][j + 1], ft_strlen(&cmd->args[i][j + 1]) + 1);
	new_args = ft_calloc(size + 2, sizeof(char *));
	if (!new_args)
		return (free(d[0]), free(d[1]), NULL);
	new_args = redefine_args_bis(cmd, d, new_args, i);
	data[3] = 1;
	data[4] = data[2] - (ft_strlen(new_args[i]) - (j - 1));
	return (free_tab(cmd->args), new_args);
}

int	args_redef(t_cmd *cmd, int i, int j, int data[5])
{
	if (!cmd->args[i])
		return (1);
	if (i == 0 && contains_spc(cmd->args[i], j, data))
		cmd->args = redefine_args(cmd, i, j, data);
	if (!cmd->args)
		return (1);
	return (0);
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
		while (cmd->args[i][++j])
		{
			if (cmd->args[i][j] == '$')
			{
				if (dol->d->n)
				{
					if (i == 0)
						cmd->args[i] = expfirst(ms, cmd->args[i], data, &dol);
					else
						cmd->args[i] = expand_dol(ms, cmd->args[i], data, &dol);
					if (args_redef(cmd, i, j, data) == 1)
						return (ms->exit_code = 255, 255);
				}
				update_expand_pos(data, &i, &j, &dol);
			}
		}
	}
	return (0);
}
