#include "../../include/exec.h"

int	*lstint_to_tab(t_list *lst)
{
	int				i;
	int				*tab_w;
	t_list			*tmp;

	i = 0;
	tmp = lst;
	tab_w = ft_calloc(ft_lstsize(tmp), sizeof(int));
	if (!tab_w) // c'est verifie
		return (NULL);
	while (tmp)
	{
		tab_w[i] = tmp->n;
		tmp = tmp->next;
		i++;
	}
	return (tab_w);
}

int	has_asterisk(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

void	advance_in_lst(t_list **lst, int size)
{
	int	i;

	i = 0;
	while (*lst && (i < size))
	{
		*lst = (*lst)->next;
		i++;
	}
}

int	len_dchar(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
			i++;
	}
	return (i);
}

char	**copy_args(char **str)
{
	int		i;
	char	**new_str;

	i = 0;
	new_str = ft_calloc(sizeof(char *), len_dchar(str) + 1);
	if (!new_str) //c'est verifie
		return (NULL);
	while (str[i])
	{
		new_str[i] = ft_strdup(str[i]);
		i++;
	}
	new_str[i] = 0;
	return (new_str);
}
