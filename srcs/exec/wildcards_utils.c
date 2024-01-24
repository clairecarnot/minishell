#include "../../include/exec.h"

int	*lstint_to_tab(t_list *lst)
{
	int				i;
	int				*tab_w;
	t_list 			*tmp;

	i = 0;
	tmp = lst;
	tab_w = ft_calloc(ft_lstsize(tmp), sizeof(int));
	if (!tab_w)// a verifier 
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
	int i;

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

int	cmd_wildcard_free(t_wildcard *wildc)
{
	if (wildc->tmp_args)
		free_tab(wildc->tmp_args);
	return (free(wildc), 1);
}

