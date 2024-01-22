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

char	**wildcards_2(DIR *d, struct dirent *file, char *arg, int *w)
{
	char	*new_arg;
	char	**split_arg;

	new_arg = NULL;
	while (file != NULL)
	{
		// dprintf(2, "d_name = %s\n", file->d_name);
		// dprintf(2, "arg = %s\n", arg);
		if (same_as_wildcard(file->d_name, arg, w) == 1)
		{
			// dprintf(2, "d_name same = %s\n", file->d_name);
			// if (!(file->d_name[0] == '.') || /*(ft_strcmp("ls", w->cmd_0) == 0 && is_flag_a(w->cmd_1) == 0 && file->d_type != 4) ||*/ (arg[0] == '.' && (arg[1] == '*')))
			// {
				if (!new_arg)
				{
					new_arg = ft_strdup(file->d_name);
					//a proteger
				}
				else
				{
					new_arg = ft_strjoin(new_arg, "\v");
					//a proteger + free
					new_arg = ft_strjoin(new_arg, file->d_name);
					//a proteger + free
				}
			// }
		}
		file = readdir(d);
	}
	if (new_arg)
	{
		split_arg = ft_split(new_arg, '\v');// a proteger
		return (split_arg);
	}
	//new_arg = ft_calloc(sizeof(char *), 4)//on pourrait retourner ca a la place de
	return (NULL);// attention aux retour de NULL possibles en cas de malloc fail
}

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



int	*lst_to_int_tab(t_list *lst)
{
	int		i;
	int		*tab;
	t_list	*tmp_lst;

	i = 0;
	tmp_lst = lst;
	tab = malloc((ft_lstsize(lst)) * sizeof(int));
	if (!tab)
		return (NULL);// a verifier
	while (tmp_lst)
	{
		tab[i] = tmp_lst->n;
		dprintf(2, "n-> %d \n", tmp_lst->n);
		tmp_lst = tmp_lst->next;
		i++;
	}
	return (tab);
}

int	len_dchar(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**make_new_args(char **args, char **ad, int w_i)
{
	int		k;
	int		i;
	int		j;
	char	**new;

	k = 0;
	i = 0;
	j = 0;
	new = malloc((len_dchar(args) + len_dchar(ad) + 1 - 1) * sizeof(char*));// a proteger
	// if (!new)
	while (i < w_i)
	{
		new[k++] = ft_strdup(args[i++]);// a proteger
	}
	while (ad[j])
	{
		new[k++] = ft_strdup(ad[j++]);// a proteger
	}
	i++;
	while (args[i])
	{
		new[k++] = ft_strdup(args[i++]);// a proteger
	}
	//free args et ad;
	new[k] = 0;
	return (new);
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

int	*lstint_to_tab(t_list *lst)
{
	int				i;
	int				*tab_w;
	t_list 			*tmp;

	i = 0;
	tmp = lst;
	tab_w = ft_calloc(ft_lstsize(tmp), sizeof(int));// a proteger
	while (tmp)
	{
		tab_w[i] = tmp->n;
		tmp = tmp->next;
		i++;
	}
	return (tab_w);
}

char	**wildcards(t_ms *ms, char **args, t_wildcard *wildc)
{
	(void)ms;
	(void)args;
	DIR				*d;
	struct dirent	*file;
	int				*tab_w;
	char 			**added_args;
	char			**new_args;
	
	if (wildcards_init(&d, &file) != 0)
		return (args);//a verifier
	// print_lst(wildc->wil->w);
	tab_w = lstint_to_tab(wildc->wil->w);
	if (!tab_w)
		return (NULL);// a verifier
	// print_tab_char(tab_w);
	added_args = wildcards_2(d, file, args[wildc->i], tab_w);
	if (added_args)
	{
		closedir(d);
		// dprintf(2, "ici2\n");
		// print_added_args(added_args);

		new_args = make_new_args(args, added_args, wildc->i);// a verifier	
		// print_added_args(new_args);
		wildc->i = wildc->i + len_dchar(added_args) - 1;
		// dprintf(2, "wildc->i = %d\n", wildc->i);
		return (new_args);
	}
	closedir(d);
	free(tab_w);
	// wildc->i = something();
	return (args);
}

// int	has_asterisk(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '*')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

void	advance_in_lst(t_list **lst, int size)
{
	int i;

	i = 0;
	while (*lst && (i < size))
	{
		*lst = (*lst)->next;
		i++;
	}
}

int	cmd_wildcard(t_ms *ms , t_cmd *cmd, t_wil *wil)
{
	int	i;
	int	size;
	t_wildcard	*wildc;
	
	wildc = ft_calloc(sizeof(t_wildcard), 1);//peut etre
	if (!wildc)// a verifier
		return (NULL);
	i = 0;
	size = 0;
	wildc->wil = wil;
	wildc->i = 0;
	while (cmd->args[i])
	{
		wildc->i = i;
		size = ft_strlen(cmd->args[i]);
		cmd->args = wildcards(ms, cmd->args, wildc);
		if (!cmd->args)// a verifier
		{
			free(wildc);
			return (NULL);
		}
		advance_in_lst(&wildc->wil->w, size);
		i = wildc->i;
		i++;
	}
	return (0);
}
