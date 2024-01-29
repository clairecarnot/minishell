#include "../../include/exec.h"

char	**cpy_end_args(char **new, char **args, int k, int i)
{
	i++;
	while (args[i])
	{
		new[k++] = ft_strdup(args[i++]);
		if (!new[k - 1])
			return (free_tab(new), NULL);// c'est verifie
	}
	new[k] = 0;
	return (new);
}

char	**make_new_args(char **args, char **ad, int w_i, int i)
{
	int		k;
	int		j;
	char	**new;

	k = 0;
	j = 0;
	new = malloc((len_dchar(args) + len_dchar(ad) + 1 - 1) * sizeof(char *));
	if (!new) //c'est verifie
		return (NULL);
	while (i < w_i)
	{
		new[k++] = ft_strdup(args[i++]);
		if (!new[k - 1])
			return (free_tab(new), NULL);// c'est verifie
	}
	while (ad[j])
	{
		new[k++] = ft_strdup(ad[j++]);
		if (!new[k - 1])
			return (free_tab(new), NULL);// c'est verifie
	}
	if (!(new == cpy_end_args(new, args, k, i)))
		return (NULL);// c'est verifie
	return (new);
}

char	**wildcards(char **args, t_wildcard *wildc)
{
	DIR				*d;
	struct dirent	*file;

	if (wildcards_init(&d, &file) != 0)
		return (args);// a verifier
	wildc->tab_w = lstint_to_tab(wildc->wil->w);
	if (!wildc->tab_w)
		return (closedir(d), NULL);// c'est verifie
	wildc->added_args = wildcards_2(d, file, args, wildc);
	free(wildc->tab_w);
	if (!wildc->added_args)
		return (closedir(d), NULL);// c'est verifie
	if (wildc->added_args && wildc->added_args[0][0] != 0)
	{
		closedir(d);
		wildc->new_args = make_new_args(args, wildc->added_args, wildc->i, 0);
		if (!wildc->new_args)
			return (free_tab(wildc->added_args), NULL);// c'est verifie
		wildc->i = wildc->i + len_dchar(wildc->added_args) - 1;
		free_tab(args);
		return (free_tab(wildc->added_args), wildc->new_args);
	}
	return (closedir(d), free_tab(wildc->added_args), args);//c'est verifie
}

int	same_len(char **args, t_list *w)
{
	int		size_w;
	int		a;
	int		i;
	int		j;
	t_list	*tmp;

	tmp = w;
	size_w = ft_lstsize(tmp);
	a = 0;
	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			j++;
			a++;
		}
		i++;
	}
	if (a != size_w)
		return (0);
	return (1);
}

int	cmd_wildcard(t_cmd *cmd, t_wil *wil)
{
	t_wildcard	*wildc;

	wildc = init_wildc(wil);
	if (!wildc) // c'est verifie
		return (1);
	if (!same_len(cmd->args, wil->w))
		return (free(wildc), 0);
	while (cmd->args[wildc->index])
	{
		re_init_wildc(wildc, cmd);
		if (has_asterisk(cmd->args[wildc->index]))
		{
			wildc->tmp_args = copy_args(cmd->args);
			if (!wildc->tmp_args) // c'est verifie
				return (cmd_wildcard_free(wildc));
			free_tab(cmd->args);
			cmd->args = wildcards(wildc->tmp_args, wildc);
			if (!cmd->args) // c'est verifie
				return (cmd_wildcard_free(wildc));
		}
		advance_in_lst(&wildc->wil->w, wildc->size);
		wildc->index = wildc->i;
		wildc->index++;
	}
	return (free(wildc), 0);
}
