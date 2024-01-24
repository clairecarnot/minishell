#include "../../include/exec.h"

// d_name = main.c   arg = *.c   int = 011
// d_name = minishe  arg = mini*   int = 11110
// d_name = main.c   arg = ma*.c  int = 11011
int	same_as_wildcard(char *d_name, char *arg, int *flag)
{
	if ((*d_name == '\0' && *arg =='\0') || (*arg == '*' && *flag == 0 && *(arg + 1) == '\0'))
		return (1);
	if (*arg == '\0' || *d_name == '\0')
		return (0);
	if (*arg == '*' && *flag == 0)
	{
		while (*(arg + 1) == '*' && *(flag + 1) == 0)
		{
			arg++;
			flag++;
		}
		if (!same_as_wildcard(d_name, arg + 1, flag + 1))
			return (same_as_wildcard(d_name + 1, arg, flag));
		else
			return (1);
	}
	if (*d_name == *arg)
		return (same_as_wildcard(d_name + 1, arg + 1, flag + 1));
	return (0);
}

// char	**wildcards_2(DIR *d, struct dirent *file, char *arg, int *w)
// {
// 	int		i;
// 	char	*new_arg;
// 	char	*tmp_arg;
// 	char	**split_arg;

// 	i = 0;
// 	new_arg = NULL;
// 	tmp_arg = NULL;
// 	while (file != NULL)
// 	{
// 		// dprintf(2, "d_name = %s\n", file->d_name);
// 		// dprintf(2, "arg = %s\n", arg);
// 		if (same_as_wildcard(file->d_name, arg, w) == 1)
// 		{
// 			// dprintf(2, "d_name same = %s\n", file->d_name);
// 			// if (!(file->d_name[0] == '.') || /*(ft_strcmp("ls", w->cmd_0) == 0 && is_flag_a(w->cmd_1) == 0 && file->d_type != 4) ||*/ (arg[0] == '.' && (arg[1] == '*')))
// 			// {
// 				if (i == 0)
// 				{
// 					new_arg = ft_strdup(file->d_name);
// 					if (!new_arg)
// 						return (NULL);// a verifier
// 				}
// 				else
// 				{
// 					tmp_arg = ft_strjoin(new_arg, "\v");
// 					free(new_arg);
// 					if (!tmp_arg)
// 						return (NULL);// a verifer 
// 					new_arg = ft_strjoin(tmp_arg, file->d_name);
// 					free(tmp_arg);
// 					if (!new_arg)
// 						return (NULL);// a verifier
// 				}
// 			i++;
// 			// }
// 		}
// 		file = readdir(d);
// 	}
// 	if (new_arg)
// 	{
// 		split_arg = ft_split(new_arg, '\v');// a proteger
// 		free(new_arg);
// 		if (!split_arg)
// 		{
// 			free(new_arg);
// 			return (NULL);
// 		}
// 		return (split_arg);
// 	}
// 	else
// 	{
// 		split_arg = ft_calloc(sizeof(char*), 1 + 1);
// 		split_arg[0] = ft_calloc(sizeof(char), 1 + 1);
// 		split_arg[0][1] = '\0';
// 		split_arg[1] = 0;
// 		if (!split_arg)// a verifier
// 			return (NULL);
// 		return (split_arg);
// 	}
// 	//new_arg = ft_calloc(sizeof(char *), 4)//on pourrait retourner ca a la place de
// 	// return (NULL);// attention aux retour de NULL possibles en cas de malloc fail
// }

char	**wildcards_2(DIR *d, struct dirent *file, char **args, t_wildcard *wildc)
{
	int		i;
	char	*new_arg;
	char	*tmp_arg;
	char	**split_arg;

	i = 0;
	new_arg = NULL;
	tmp_arg = NULL;
	while (file != NULL)
	{
		// dprintf(2, "d_name = %s\n", file->d_name);
		// dprintf(2, "arg = %s\n", arg);
		if (same_as_wildcard(file->d_name, args[wildc->i], wildc->tab_w) == 1)
		{
			// dprintf(2, "d_name same = %s\n", file->d_name);
			if (file->d_name[0] != '.' || (ft_strncmp("ls", args[0], 3) == 0 \
			&& ft_strncmp("-a", args[1], 3) == 0 && file->d_type != 4) \
			|| (args[wildc->i][0] == '.' && (args[wildc->i][1] == '*')))
				if (i == 0)
				{
					new_arg = ft_strdup(file->d_name);
					if (!new_arg)
						return (NULL);// a verifier
				}
				else
				{
					tmp_arg = ft_strjoin(new_arg, "\v");
					free(new_arg);
					if (!tmp_arg)
						return (NULL);// a verifer 
					new_arg = ft_strjoin(tmp_arg, file->d_name);
					free(tmp_arg);
					if (!new_arg)
						return (NULL);// a verifier
				}
			i++;
			// }
		}
		file = readdir(d);
	}
	if (new_arg)
	{
		split_arg = ft_split(new_arg, '\v');// a proteger
		free(new_arg);
		if (!split_arg)
		{
			free(new_arg);
			return (NULL);
		}
		return (split_arg);
	}
	else
	{
		split_arg = ft_calloc(sizeof(char*), 1 + 1);
		split_arg[0] = ft_calloc(sizeof(char), 1 + 1);
		split_arg[0][1] = '\0';
		split_arg[1] = 0;
		if (!split_arg)// a verifier
			return (NULL);
		return (split_arg);
	}
	//new_arg = ft_calloc(sizeof(char *), 4)//on pourrait retourner ca a la place de
	// return (NULL);// attention aux retour de NULL possibles en cas de malloc fail
}

void	print_added_args(char **a)// temporaire
{
	int	i;

	i = 0;
	while (a[i])
	{
		dprintf(2, "%s\n", a[i]);
		i++;
	}
}

// char	*wldc2_join(char *d_name, char *new_arg, t_wildcard *wildc)
// {
// 	char	*tmp_arg;
// 	char	*new_arg2;

// 	tmp_arg = NULL;
// 	new_arg2 = NULL;
// 	(void)wildc;
// 	tmp_arg = ft_strjoin(new_arg, "\v");
// 	free(new_arg);
// 	new_arg = NULL;
// 	if (!tmp_arg)
// 		return (NULL);// a verifer 
// 	new_arg2 = ft_strjoin(tmp_arg, d_name);
// 	free(tmp_arg);
// 	if (!new_arg2)
// 		return (NULL);// a verifier
// 	return (new_arg2);
// }

// char	**wildcards_2(DIR *d, struct dirent *file, char **args, t_wildcard *wildc)
// {
// 	char	*new_arg;

// 	new_arg = NULL;
// 	while (file != NULL)
// 	{
// 		if (same_as_wildcard(file->d_name, args[wildc->i], wildc->tab_w) == 1)
// 		{
// 			if (file->d_name[0] != '.' || (ft_strncmp("ls", args[0], 3) == 0 \
// 			&& ft_strncmp("-a", args[1], 3) == 0 && file->d_type != 4) \
// 			|| (args[wildc->i][0] == '.' && (args[wildc->i][1] == '*')))
// 			{
// 				if (++(wildc->j) == 0)
// 					if (!(new_arg = ft_strdup(file->d_name)))
// 						return (NULL);// a verifier
// 				if (wildc->j > 0)
// 					if (!(new_arg = wldc2_join(file->d_name, new_arg, wildc)))
// 						return (NULL); // a verfier 
// 			}
// 		}
// 		file = readdir(d);
// 	}
// 	char	**split_arg;

// 	split_arg = 0;
// 	// return (wldc2_split(new_arg, wildc));
// 	if (new_arg)
// 	{
// 		// dprintf(2, "new_arg : %s\n", new_arg);
// 		split_arg = ft_split(new_arg, '\v');// a proteger
// 		free(new_arg);
// 		if (!split_arg)
// 		{
// 			// dprintf(2, "hey\n");
// 			// free(wildc->new_arg);
// 			return (NULL);
// 		}
// 		dprintf(2, "hey 2\n");
// 		print_added_args(split_arg);
// 		return (split_arg);
// 	}
// 	else
// 	{
// 		split_arg = ft_calloc(sizeof(char*), 1 + 1);
// 		split_arg[0] = ft_calloc(sizeof(char), 1 + 1);
// 		split_arg[0][1] = '\0';
// 		split_arg[1] = 0;
// 		if (!split_arg)// a verifier
// 			return (NULL);
// 		return (split_arg);
// 	}
// }

int	wildcards_init(DIR **d, struct dirent **file)
{
	*d = opendir(".");
	if (!(*d))
	{
		perror("Erreur lors de l'ouverture du répertoire");//peut etre a changer
		return (1);
	}
	*file = readdir(*d);
	if (!(*file))
	{
		perror("Erreur lors de la lecture des fichiers du répertoire");//peut etre a changer
		return (1);
	}
	return (0);
}

// int	*lst_to_int_tab(t_list *lst)
// {
// 	int		i;
// 	int		*tab;
// 	t_list	*tmp_lst;

// 	i = 0;
// 	tmp_lst = lst;
// 	tab = malloc((ft_lstsize(lst)) * sizeof(int));
// 	if (!tab)
// 		return (NULL);// a verifier
// 	while (tmp_lst)
// 	{
// 		tab[i] = tmp_lst->n;
// 		dprintf(2, "n-> %d \n", tmp_lst->n);
// 		tmp_lst = tmp_lst->next;
// 		i++;
// 	}
// 	return (tab);
// }


char	**cpy_end_args(char **new, char **args, int k, int i)
{
	i++;
	while (args[i])
	{
		new[k++] = ft_strdup(args[i++]);
		if (!new[k - 1])
			return (free_tab(new), NULL);// a verifier
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
	new = malloc((len_dchar(args) + len_dchar(ad) + 1 - 1) * sizeof(char*));
	if (!new)// c'est verifie
		return (free_tab(ad), NULL);
	while (i < w_i)
	{
		new[k++] = ft_strdup(args[i++]);
		if (!new[k - 1])
			return (free_tab(ad), free_tab(new), NULL);// a verifier
	}
	while (ad[j])
	{
		new[k++] = ft_strdup(ad[j++]);
		if (!new[k - 1])
			return (free_tab(ad), free_tab(new), NULL);// a verifier
	}
	if (!(new == cpy_end_args(new, args, k, i)))
		return (free_tab(ad), NULL);
	return (new);
}

// void	print_tab_char(int *tab)//temp
// {
// 	int i = 0;

// 	dprintf(2, "print int tab\n");
// 	while (i < 5)
// 	{
// 		dprintf(2, "tab = %d\n", tab[i]);
// 		i++;
// 	}
// }


char	**wildcards(char **args, t_wildcard *wildc)
{
	DIR				*d;
	struct dirent	*file;
	char			**added_args;

	added_args = NULL;
	if (wildcards_init(&d, &file) != 0)
		return (args);// a verifier
	wildc->tab_w = lstint_to_tab(wildc->wil->w);
	if (!wildc->tab_w)
		return (closedir(d), NULL);// c'est verifie
	added_args = wildcards_2(d, file, args, wildc);
	free(wildc->tab_w);
	dprintf(2, "added arg : \n");
	print_added_args(added_args);
	if (!added_args)
		return (closedir(d), NULL);// malloc fail a verifier
	if (added_args && added_args[0][0] != 0)
	{
		closedir(d);
		wildc->new_args = make_new_args(args, added_args, wildc->i, 0);// a verifier	
		if (!wildc->new_args)
		{
			closedir(d);
			return (NULL);//a verifier
		}
		wildc->i = wildc->i + len_dchar(added_args) - 1;
		free_tab(args);
		return (free_tab(added_args), wildc->new_args);
	}
	closedir(d);
	return (free_tab(added_args), args);//a verifier
}

char	**copy_args(char **str)
{
	int		i;
	char	**new_str;

	i = 0;
	new_str = ft_calloc(sizeof(char *), len_dchar(str) + 1);
	if (!new_str)//c'est verifie
		return (NULL);
	while (str[i])
	{
		new_str[i] = ft_strdup(str[i]);
		i++;
	}
	new_str[i] = 0;
	return (new_str);
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
	// wildc->new_arg = NULL;
	wildc->tmp_arg = NULL;
	wildc->split_arg = 0;
	wildc->join[0] = '\v';
	wildc->join[1] = '\0';
	wildc->j = -1;
	return (wildc);
}

int	cmd_wildcard(t_cmd *cmd, t_wil *wil)
{
	t_wildcard	*wildc;

	wildc = init_wildc(wil);
	if (!wildc)// c'est verifie
		return (1);
	while (cmd->args[wildc->index])
	{
		wildc->i = wildc->index;
		wildc->size = ft_strlen(cmd->args[wildc->index]);
		wildc->tmp_args = NULL;
		if (has_asterisk(cmd->args[wildc->index]))
		{
			wildc->tmp_args = copy_args(cmd->args);
			if (!wildc->tmp_args)// c'est verifie
				return (cmd_wildcard_free(wildc));
			free_tab(cmd->args);
			cmd->args = wildcards(wildc->tmp_args, wildc);
			if (!cmd->args)// c'est verifie
				return (cmd_wildcard_free(wildc));
		}
		advance_in_lst(&wildc->wil->w, wildc->size);
		wildc->index = wildc->i;
		wildc->index++;
	}
	return (free(wildc), 0);
}
