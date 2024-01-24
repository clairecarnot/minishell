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

t_wildcard	*init_wildc(t_wil *wil)
{
	t_wildcard	*wildc;

	wildc = ft_calloc(sizeof(t_wildcard), 1);
	if (!wildc)
		return (NULL);
	wildc->i = 0;
	wildc->size = 0;
	wildc->index = 0;
	wildc->tmp_args = NULL;
	wildc->wil = wil;
	wildc->tab_w = NULL;
	wildc->added_args = 0;
	wildc->new_args = 0;
	wildc->j = -1;
	return (wildc);
}

int	cmd_wildcard(t_cmd *cmd, t_wil *wil)
{
	t_wildcard	*wildc;

	wildc = init_wildc(wil);
	if (!wildc) // c'est verifie
		return (1);
	while (cmd->args[wildc->index])
	{
		wildc->i = wildc->index;
		wildc->size = ft_strlen(cmd->args[wildc->index]);
		wildc->tmp_args = NULL;
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
