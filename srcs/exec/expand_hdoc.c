/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_hdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:28:10 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:28:12 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h" 

char	*hd_keep_one_dol_only(t_ms *ms, char *arg, int i)
{
	char	*new_arg;
	int		j;
	int		k;

	new_arg = NULL;
	j = i + 1;
	while (arg[j] && arg[j] == '$')
		j++;
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

int	hd_dol_standalone(char *arg)
{
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
	return (0);
}

char	*hd_expand_dol(t_ms *ms, char *arg, int data[5])
{
	int		i;
	int		j;
	char	*exp_arg;
	char	*var;

	i = find_cur_dol(arg, data);
	if (!arg[i] || !arg[i + 1] || hd_dol_standalone(&arg[i + 1]))
		return (arg);
	if (arg[i + 1] == '$')
		arg = hd_keep_one_dol_only(ms, arg, i);
	if (!arg)
		return (NULL);
	j = hd_delimitate_var(arg, i);
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

void	hd_update_expand_pos(int data[5], int *j)
{
	data[0] += 1;
	if (data[1] == 1)
		*j -= 1;
	if (data[2] > 0)
		*j += data[2] - 1;
	data[1] = 0;
	data[2] = 0;
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

char	*expand_hdoc(t_ms *ms, char *arg)
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
			arg = hd_expand_dol(ms, arg, data);
			if (!arg)
				return (NULL);
			hd_update_expand_pos(data, &j);
		}
	}
	return (arg);
}
